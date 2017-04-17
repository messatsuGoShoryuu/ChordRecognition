
#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include <vector>
#include <string>

#include "AudioManager.h"
#include "WindowsApp.h"
#include "MainApp.h"

extern	std::vector<std::string>	g_inputDevices;
extern	std::vector<std::string>	g_outputDevices;
extern	AudioManager				g_audioManager;
extern	WindowsApp					g_windowsApp;
extern  MainApp						g_mainApp;


extern	f64	g_inverseSampleRate;


#endif // !_GLOBALS_H_



