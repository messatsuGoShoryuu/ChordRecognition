#ifndef _WINDOWSAPP_H_
#define	_WINDOWSAPP_H_

#include <Windows.h>
#include "types.h"
class WindowsApp
{
public:
	WindowsApp();
	~WindowsApp();

	void initialize();
	void shutDown();

	void execute();

	void	paintLine(POINT start, POINT end, HDC hdc);
	void	paintWaveForm(i32 x, i32 y, i32 w, i32 h, HDC hdc);

	inline	HWND	selectInputDeviceWindow(){ return m_selectInputDeviceWindow; }
	inline	HWND	selectOutputDeviceWindow(){ return m_selectOutputDeviceWindow; }
	inline HWND		mainWindow(){ return m_mainWindow; }
	inline HWND		waveFormWindow(){ return m_waveFormWindow; }
	inline HWND		frequencyWindow(){ return m_frequencyWindow; }
	inline HWND		chromagramWindow(){ return m_chromagramWindow; }

	inline	void	setFrequencyRect(LONG x, LONG y, LONG w, LONG h)
	{
		m_frequencyRect.top = y;
		m_frequencyRect.left = x;
		m_frequencyRect.right = w;
		m_frequencyRect.bottom = h;
	}

	inline void		showFrequency(bool value){ m_showFrequency = value; }

	inline			RECT frequencyRect(){ return m_frequencyRect; }
private:
	HWND	m_mainWindow;
	HWND	m_selectInputDeviceWindow;
	HWND	m_selectOutputDeviceWindow;
	HWND	m_waveFormWindow;
	HWND	m_frequencyWindow;
	HWND	m_chromagramWindow;
	HINSTANCE m_hInstance;
	ULONG_PTR m_gdiPlusToken;

	bool	m_showFrequency;
	RECT	m_frequencyRect;
};
#endif // !_WINDOWSAPP_H_
