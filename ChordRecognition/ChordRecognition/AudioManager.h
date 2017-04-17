#ifndef _AUDIOMANAGER_H_
#define _AUDIOMANAGER_H_

#include <portaudio.h>
#include <pa_asio.h>
#include <vector>
#include "types.h"
#include "define.h"



struct AudioInputData
{
	ui32	lastReadIndex;
	f32		samples[INPUT_BUFFER_SIZE];
	bool	initFFT;

	void	finalizeBin();
};


class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool initialize();
	bool shutDown();

public:
	void	setCurrentInputDevice(ui32 index);
	void	setCurrentOutputDevice(ui32 index);

	inline	bool	directMonitoring(){ return m_directMonitoring; }

	void	stopStream();
	void	startStream();

	inline	float*	inputSamples(){ return m_audioInput.samples; }

private:
	bool	m_directMonitoring;
	bool	m_isStreaming;

	PaDeviceIndex	m_currentInputDevice;
	PaDeviceIndex	m_currentOutputDevice;

	std::vector<PaDeviceIndex>	m_inputDevices;
	std::vector<PaDeviceIndex>	m_outputDevices;

	PaStream*					m_inputStream;

	AudioInputData m_audioInput;
	AudioInputData m_streamData;
};
#endif // !_AUDIOMANAGER_H_
