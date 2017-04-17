#include "DFT.h"
#include <portaudio.h>
#include <iostream>
#include "types.h"
#include "WindowsApp.h"
#include "globals.h"
#include <pa_asio.h>
#include <thread>



typedef struct
{
	float left_phase;
	float right_phase;
}
paTestData;


void mainAppThread()
{
	while (g_mainApp.isRunning())
	{
		g_mainApp.update();
	}
	
}


int main()
{
	g_mainApp.initialize();

	g_audioManager.initialize();

	g_windowsApp.initialize();

	std::thread appThread = std::thread(mainAppThread);

	g_windowsApp.execute();

	g_windowsApp.shutDown();

	g_audioManager.shutDown();

	if (appThread.joinable()) appThread.join();

	g_mainApp.shutDown();
	system("pause");
	return 0;
}