#include "MainApp.h"
#include "globals.h"
#include "Complex.h"
#include "DFT.h"
#include <algorithm>
#include <iostream>
#include "Chromagram.h"

//#include "define.h"
void	MainApp::initialize()
{
	m_DFT.resize(INPUT_BUFFER_SIZE);
	m_input.resize(INPUT_BUFFER_SIZE);
	m_inputf.resize(INPUT_BUFFER_SIZE);
	m_temp.resize(INPUT_BUFFER_SIZE);
	m_wantsToDFT = false;
	m_requestSamples = true;

	initializeDFT();
	run();
}

void MainApp::scheduleDFT(f32* input)
{
	m_wantsToDFT = true;
	m_requestSamples = false;
	for (ui32 i = 0; i < INPUT_BUFFER_SIZE; i++)
	{
		m_input[i].setReal(input[i]);
		m_input[i].setIm(0);
	}
}

bool	MainApp::DFT()
{
	if (!m_wantsToDFT) return false;	
	::DFT(m_input, m_DFT);	
	for (ui32 i = 0; i < INPUT_BUFFER_SIZE; i++)
	{
		m_inputf[i] = m_DFT[i].mod();
	}
	m_requestSamples = true;
	m_wantsToDFT = false;
	return true;
}

void MainApp::update()
{
	Sleep(3);
	if (DFT())
	{
		calculateFundamental();
		calculateChromagram();
	}
		
}

void MainApp::calculateChromagram()
{
	for (ui32 i = 0; i < 12; i++)m_chromagram[i] = 0.0;
	::chromagram(m_inputf.data(), m_chromagram, m_inputf.size(), 440.0);
}

void	MainApp::calculateFundamental()
{
	ui32 count = m_DFT.size();
	f64 amplitude = 0.0;
	ui32	max = 0;
	for (ui32 i = 0; i < count; i++)
	{
		f64 tempAmplitude = m_inputf[i];

		if (tempAmplitude > amplitude)
		{
			amplitude = tempAmplitude;
			max = i;
		}
	}

	//A bin represent Sampling frequency / DFT length * bin index frequency
	f64 bin = (f64)DFT_SAMPLE_RATE / (f64)INPUT_BUFFER_SIZE;

	//Return loudest.
	m_fundamental = bin * max;
}

void	MainApp::shutDown()
{

}