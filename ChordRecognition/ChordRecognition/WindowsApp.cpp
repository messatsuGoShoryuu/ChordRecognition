#include "WindowsApp.h"
#include <CommCtrl.h>
#include <iostream>
#include "globals.h"
#include "types.h"
#include <gdiplus.h>

WindowsApp::WindowsApp()
	:m_mainWindow(NULL),
	m_hInstance(NULL)
{

}

WindowsApp::~WindowsApp()
{

}

void WindowsApp::paintLine(POINT begin, POINT end, HDC hdc)
{
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Pen	pen(Gdiplus::Color(255, 255, 255, 255));
	graphics.DrawLine(&pen, (INT)begin.x, (INT)begin.y, (INT)end.x, (INT)end.y);
}


void WindowsApp::paintWaveForm(i32 x, i32 y, i32 w, i32 h, HDC hdc)
{

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Pen	pen(Gdiplus::Color(255, 255, 255, 255));

	Gdiplus::Point points[500];

	float k = 0;
	int j = 0;
	bool second = false;
	float step = (float)INPUT_BUFFER_SIZE / 500.0;
	for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
	{
		if (j >= 500) break;
		if (i < k) continue;
		else k += step;
		
		f32 x_ = (f32)i / (f32)INPUT_BUFFER_SIZE;
		x_ *= w + x;

		f32 y_ = 0;
		if(m_showFrequency)y_ = g_mainApp.inputf()[i] * ((f32)h / 2.0) + y;
		else y_ = g_mainApp.inputSamples()[i].real() * ((f32)h / 2.0) + y;
		
		points[j].X = x_;
		points[j].Y = y_;
		j++;
	}

	graphics.DrawCurve(&pen, points, 500);
}

bool updateChromagramText;

LRESULT CALLBACK mainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
			if (!g_windowsApp.mainWindow())
			{
				CreateWindow(TEXT("button"), TEXT("Show Frequency"),
							 WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
							 0, 100, 185, 30,
							 hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
				CheckDlgButton(hWnd, 1, BST_CHECKED);
			}
			std::cout << "Window Creation Sucessful" << std::endl;
			return 0;

		case WM_TIMER:
		{
			g_mainApp.fundamental();
			RECT rect;
			GetClientRect(g_windowsApp.waveFormWindow(), &rect);
			InvalidateRect(g_windowsApp.waveFormWindow(), &rect,true);

			GetClientRect(g_windowsApp.frequencyWindow(), &rect);
			InvalidateRect(g_windowsApp.frequencyWindow(), &rect, true);

			GetClientRect(g_windowsApp.chromagramWindow(), &rect);
			InvalidateRect(g_windowsApp.chromagramWindow(), &rect, true);
			updateChromagramText = true;
		}
			return 0;

		case WM_PAINT:
		{
			HDC hdc = GetDC(hWnd);
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			
			RECT rect;
			GetClientRect(hWnd, &rect);

			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, RGB(255, 255, 255));

			if (hWnd == g_windowsApp.mainWindow())
			{
				std::string string = "Select Input Device:";
				TextOut(hdc, 0, 0, string.c_str(), string.length());

				string = "Select Output Device:";
				TextOut(hdc, 0, 50, string.c_str(), string.length());

				g_windowsApp.setFrequencyRect(0, 100, 500, 100);
			}
			else if (hWnd == g_windowsApp.frequencyWindow())
			{
				char buffer[64];
				sprintf_s(buffer, "Fundamental = %.2f", g_mainApp.fundamental());
				TextOut(hdc, 0, 0, buffer, strlen(buffer));
			}
			else if (hWnd == g_windowsApp.waveFormWindow())
			{
				g_windowsApp.paintWaveForm(0, 100, 500, 200, hdc);
			}
			else if (hWnd == g_windowsApp.chromagramWindow())
			{
				if (updateChromagramText)
				{
					RECT rect;
					GetClientRect(g_windowsApp.chromagramWindow(), &rect);
					char buffer[64];

					f64 chromagram[12];
					for(ui32 i = 0; i<12;i++) chromagram[i] = g_mainApp.chromagram()[i];
					buffer[64];
					
					f64 max = 0.0;
		

					for (ui32 i = 0; i < 12; i++)
					{
						if (chromagram[i] > max) max = chromagram[i];
					}

					max = max * 0.7f;

					sprintf_s(buffer, "C : %.2f", chromagram[0]);
					if (chromagram[0] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 10, buffer, strlen(buffer));
				
					sprintf_s(buffer, "C# : %.2f", chromagram[1]);
					if (chromagram[1] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 30, buffer, strlen(buffer));

					sprintf_s(buffer, "D : %.2f", chromagram[2]);
					if (chromagram[2] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 50, buffer, strlen(buffer));

					sprintf_s(buffer, "D# : %.2f", chromagram[3]);
					if (chromagram[3] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 70, buffer, strlen(buffer));

					sprintf_s(buffer, "E : %.2f", chromagram[4]);
					if (chromagram[4] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 90, buffer, strlen(buffer));

					sprintf_s(buffer, "F : %.2f", chromagram[5]);
					if (chromagram[5] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 110, buffer, strlen(buffer));

					sprintf_s(buffer, "F# : %.2f", chromagram[6]);
					if (chromagram[6] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 130, buffer, strlen(buffer));

					sprintf_s(buffer, "G : %.2f", chromagram[7]);
					if (chromagram[7] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 150, buffer, strlen(buffer));

					sprintf_s(buffer, "G# : %.2f", chromagram[8]);
					if (chromagram[8] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 170, buffer, strlen(buffer));

					sprintf_s(buffer, "A : %.2f", chromagram[9]);
					if (chromagram[9] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 190, buffer, strlen(buffer));

					sprintf_s(buffer, "A# : %.2f", chromagram[10]);
					if (chromagram[10] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 210, buffer, strlen(buffer));

					sprintf_s(buffer, "B : %.2f", chromagram[11]);
					if (chromagram[11] > max) SetTextColor(hdc, RGB(255, 0, 0));
					else SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, 0, 230, buffer, strlen(buffer));					
				}
			}

			EndPaint(hWnd, &ps);

			RECT WaveRect;
			WaveRect.top = 200;
			WaveRect.left = 0;
			WaveRect.right = 500;
			WaveRect.bottom = 200;
			ReleaseDC(hWnd, hdc);
		}
			return 0;

		case WM_COMMAND:
		{
			if (hWnd == g_windowsApp.mainWindow())
			{
				BOOL checked = IsDlgButtonChecked(hWnd, 1);
				if (checked) 
				{
					g_windowsApp.showFrequency(false);
					CheckDlgButton(hWnd, 1, BST_UNCHECKED);	
				}
				else 
				{
					g_windowsApp.showFrequency(true);
					CheckDlgButton(hWnd, 1, BST_CHECKED);
				}
			}
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				HWND h = (HWND)lParam;
				int index = SendMessage(h, (UINT)CB_GETCURSEL,
											(WPARAM)0, (LPARAM)0);

				if (h == g_windowsApp.selectInputDeviceWindow()) g_audioManager.setCurrentInputDevice(index);
				if (h == g_windowsApp.selectOutputDeviceWindow()) g_audioManager.setCurrentOutputDevice(index);

			}
		}
			return 0;

		case WM_DESTROY:
			if (hWnd == g_windowsApp.mainWindow())g_mainApp.stop();
			PostQuitMessage(0);
			
			return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void	WindowsApp::shutDown()
{
	Gdiplus::GdiplusShutdown(m_gdiPlusToken);
}

void WindowsApp::initialize()
{
	m_hInstance = GetModuleHandle(0);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;

	GdiplusStartup(&m_gdiPlusToken, &gdiplusStartupInput, NULL);

	WNDCLASSEX	mainWinClass;

	memset(&mainWinClass, 0, sizeof(WNDCLASSEX));

	mainWinClass.cbSize = sizeof(WNDCLASSEX);
	mainWinClass.hInstance = m_hInstance;
	mainWinClass.lpfnWndProc = mainWndProc;
	mainWinClass.style = CS_HREDRAW | CS_VREDRAW;
	mainWinClass.lpszClassName = TEXT("Main Window Class");
	mainWinClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));

	if (!RegisterClassEx(&mainWinClass))
	{
		MessageBox(NULL,
				   TEXT("Call to RegisterClassEx failed!"),
				   TEXT("Win32 Guided Tour"),
				   NULL);

		return;
	}

	m_mainWindow = CreateWindowEx(
		WS_EX_CLIENTEDGE, 
		mainWinClass.lpszClassName, 
		TEXT("Chord Recognition Software"),
		WS_OVERLAPPEDWINDOW,
		0,0,
		500,800,
		NULL, NULL, m_hInstance, 0);

	if (!m_mainWindow)
	{
		MessageBox(NULL,
				   TEXT("Call to CreateWindow failed!"),
				   TEXT("Win32 Guided Tour"),
				   NULL);

		return;
	}

	
	

	m_selectInputDeviceWindow = CreateWindow(WC_COMBOBOX, TEXT(""),
									 CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
									 0, 20, 400, 50 * g_outputDevices.size(), m_mainWindow, NULL, m_hInstance,
									 NULL);

	m_selectOutputDeviceWindow = CreateWindow(WC_COMBOBOX, TEXT(""),
									 CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
									 0, 70, 400, 50 * g_outputDevices.size(), m_mainWindow, NULL, m_hInstance,
									 NULL);

	WNDCLASSEX	waveFormClass;

	memset(&waveFormClass, 0, sizeof(WNDCLASSEX));

	waveFormClass.cbSize = sizeof(WNDCLASSEX);
	waveFormClass.hInstance = m_hInstance;
	waveFormClass.lpfnWndProc = mainWndProc;
	waveFormClass.style = CS_HREDRAW | CS_VREDRAW;
	waveFormClass.lpszClassName = TEXT("Waveform Window Class");
	waveFormClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));;

	if (!RegisterClassEx(&waveFormClass))
	{
		MessageBox(NULL,
				   TEXT("Call to RegisterClassEx failed!"),
				   TEXT("Win32 Guided Tour"),
				   NULL);

		return;
	}

	WNDCLASSEX	chromagramClass;

	memset(&chromagramClass, 0, sizeof(WNDCLASSEX));

	chromagramClass.cbSize = sizeof(WNDCLASSEX);
	chromagramClass.hInstance = m_hInstance;
	chromagramClass.lpfnWndProc = mainWndProc;
	chromagramClass.style = CS_HREDRAW | CS_VREDRAW;
	chromagramClass.lpszClassName = TEXT("Chromagram Window Class");
	chromagramClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));;

	if (!RegisterClassEx(&chromagramClass))
	{
		MessageBox(NULL,
				   TEXT("Call to RegisterClassEx failed!"),
				   TEXT("Win32 Guided Tour"),
				   NULL);

		return;
	}


	m_waveFormWindow = CreateWindowEx(WS_EX_CLIENTEDGE, waveFormClass.lpszClassName,
									  TEXT(""),WS_CHILD,0,200,500,200,m_mainWindow,NULL,
									  m_hInstance, 0);

	m_frequencyWindow = CreateWindowEx(WS_EX_CLIENTEDGE, waveFormClass.lpszClassName, TEXT(""), WS_CHILD, 0, 150, 500, 40, m_mainWindow, NULL, m_hInstance, 0);

	m_chromagramWindow =  CreateWindowEx(WS_EX_CLIENTEDGE, chromagramClass.lpszClassName,
														   TEXT(""), WS_CHILD, 0, 400, 250, 300, m_mainWindow, NULL,
														   m_hInstance, 0);

	for (ui32 i = 0; i < g_inputDevices.size(); i++)
	{
		SendMessage(m_selectInputDeviceWindow, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)g_inputDevices[i].c_str());
	}

	for (ui32 i = 0; i < g_outputDevices.size(); i++)
	{
		SendMessage(m_selectOutputDeviceWindow, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)g_outputDevices[i].c_str());
	}
	ShowWindow(m_mainWindow, SW_SHOWNORMAL);
	UpdateWindow(m_mainWindow);

	ShowWindow(m_waveFormWindow, SW_SHOWNORMAL);
	UpdateWindow(m_waveFormWindow);

	ShowWindow(m_frequencyWindow, SW_SHOWNORMAL);
	UpdateWindow(m_frequencyWindow);

	ShowWindow(m_chromagramWindow, SW_SHOWNORMAL);
	UpdateWindow(m_chromagramWindow);

	SetTimer(m_waveFormWindow, NULL, 50, NULL);
	
}

void WindowsApp::execute()
{
	MSG msg;
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}