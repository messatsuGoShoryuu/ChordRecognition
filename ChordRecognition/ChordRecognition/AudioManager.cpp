#include "globals.h"
#include "types.h"
#include <iostream>
#include "DFT.h"
#include "Filter.h"

void	AudioInputData::finalizeBin()
{
	initFFT = true;
	lastReadIndex = 0;
	f64 sum = 0.0;
	for (ui32 i = 0; i < INPUT_BUFFER_SIZE; i++)
	{
		sum += samples[i];
	}		
}


void noiseGate(f32* samples, ui32 count, f32 threshold)
{
	f64 sum = 0;
	for (ui32 i = 0; i < count; i++)
	{
		sum += abs(samples[i]);
	}

	f64 average = sum / count;
	if (average < threshold)
	{
		for (ui32 i = 0; i < count; i++)
		{
			samples[i] = 0;
		}
	}
}

static int inputCallback(const void* inputBuffer, void* outputBuffer,
						  unsigned long	framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
						  PaStreamCallbackFlags	statusFlags, void *userData)
{
	(void)outputBuffer;
	(void)timeInfo;
	(void)statusFlags;
	(void)userData;

	AudioInputData* audioInput = (AudioInputData*)userData;



	float* out = g_audioManager.directMonitoring() ? (float*)outputBuffer : NULL;
	float* in = (float*)inputBuffer;

	int size = framesPerBuffer * 2;
	bool second = false;
	int k = 0;

	int downSample = SAMPLE_RATE / DFT_SAMPLE_RATE;
	
	int j = 0;
	for (ui64 i = 0; i < size; i++)
	{
		
		if(out) out[i] = in[i];
		if (i == j)
		{
			if (!second)
			{
				audioInput->samples[audioInput->lastReadIndex + k] = in[i];
				k++;
			}
			j += downSample;
		}
		second = !second;
	}

	noiseGate(audioInput->samples + audioInput->lastReadIndex, framesPerBuffer, 0.002);
	
	audioInput->lastReadIndex += framesPerBuffer;
	if (audioInput->lastReadIndex >= INPUT_BUFFER_SIZE)
	{
		audioInput->finalizeBin();
		g_mainApp.scheduleDFT(audioInput->samples);
	}

	return paContinue;
}

AudioManager::AudioManager()
	:m_currentInputDevice(0),
	m_currentOutputDevice(0),
	m_directMonitoring(true),
	m_isStreaming(false)
{

}

AudioManager::~AudioManager()
{

}

bool	AudioManager::initialize()
{
	PaError err = Pa_Initialize();

	for (int i = 0; i < Pa_GetDeviceCount(); i++)
	{
		const PaDeviceInfo* info = Pa_GetDeviceInfo(i);

		if (info->maxInputChannels > 0)
		{
			m_inputDevices.push_back(i);
			g_inputDevices.push_back(info->name);
		}
		if (info->maxOutputChannels > 0)
		{
			m_outputDevices.push_back(i);
			g_outputDevices.push_back(info->name);
		}
	}

	INVERSE_SAMPLE_RATE = 1.0 / (f64)SAMPLE_RATE;

	m_directMonitoring = false;

	return true;
}

bool	AudioManager::shutDown()
{
	PaError err = Pa_Terminate();
	return err == paNoError;
}

void	AudioManager::startStream()
{
	PaStreamParameters inputParameters, outputParameters;


	memset(&m_audioInput, 0, sizeof(AudioInputData));

	inputParameters.device = m_inputDevices[m_currentInputDevice];
	inputParameters.channelCount = 2;                    /* stereo input */
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = m_outputDevices[m_currentOutputDevice];
	outputParameters.channelCount = 2;                     /* stereo output */
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	Pa_OpenStream(&m_inputStream, &inputParameters, &outputParameters, SAMPLE_RATE, STREAM_BUFFER_SIZE, 0, inputCallback, &m_audioInput);
	Pa_StartStream(m_inputStream);

	m_isStreaming = true;
}

void AudioManager::stopStream()
{
	if (m_isStreaming)
	{
		Pa_StopStream(m_inputStream);
		m_isStreaming = false;
	}
}

void	AudioManager::setCurrentInputDevice(ui32 index)
{
	if (m_currentInputDevice == index) return;
	if (m_isStreaming) stopStream();
	m_currentInputDevice = index;
	g_mainApp.resetDFTState();
	startStream();
}

void	AudioManager::setCurrentOutputDevice(ui32 index)
{
	if (m_currentOutputDevice == index) return;
	if (m_isStreaming) stopStream();
	m_currentOutputDevice = index;
	startStream();
}