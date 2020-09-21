// Windows and DirectX 7 includes

/* Don't forget to include the DirectX and other libraries:
   ddraw.lib dsound.lib dinput.lib winmm.lib dxguid.lib 

   Also, make sure you have the DirectX 7 SDK installed */

#ifndef WIN_AND_DX
#define WIN_AND_DX

#define WIN32_LEAN_AND_MEAN  // No MFC

// Windows Includes
#include <windows.h>
#include <windowsx.h> 
#include <mmsystem.h>

// C++ Includes
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>

using namespace std;

// DirectX Includes

#define DIRECTINPUT_VERSION 0x0700

#include <ddraw.h>  
#include <dinput.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>


/*	Just call this function from the game main to test FPS
	Need Main Window Handle (HWND) and #include <windows.h> to work
	
void Fps()
{
	static unsigned long StartTime = GetTickCount();
	static unsigned int Fps = 0;
	static char Buffer[12];
	unsigned int TimeDifference = 0;
	unsigned int OneSecond = 1000;

	Fps++;

	HDC hDeviceContext = GetDC(hMainWindowHandle);
	TimeDifference = GetTickCount() - StartTime;

	if (TimeDifference >= OneSecond)
	{
		sprintf(Buffer,"Fps = %d", Fps);
		StartTime = GetTickCount();
		Fps = 0;
	}

	TextOut(hdc,8,8,Buffer,strlen(Buffer));
	ReleaseDC(hMainWindowHandle, hDeviceContext);
};
*/
#endif