// T3DLIB2.CPP - Game Engine Part II
 
// INCLUDES ///////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN  
// #define INITGUID

#include <windows.h>	// include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <objbase.h>
#include <conio.h>		// include important C/C++ stuff
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#define DIRECTINPUT_VERSION 0x0700

#include <dinput.h>
#include "Input Engine.h"

// DEFINES ////////////////////////////////////////////////

// TYPES //////////////////////////////////////////////////

// PROTOTYPES /////////////////////////////////////////////

// EXTERNALS /////////////////////////////////////////////

extern HWND hMainWindowHandle;     // access to main window handle in main module
extern HINSTANCE hMainInstance; // save the instance

// GLOBALS ////////////////////////////////////////////////

// directinput globals
LPDIRECTINPUT        lpdi      = NULL;    // dinput object
LPDIRECTINPUTDEVICE  lpdikey   = NULL;    // dinput keyboard
LPDIRECTINPUTDEVICE  lpdimouse = NULL;    // dinput mouse
LPDIRECTINPUTDEVICE2 lpdijoy   = NULL;    // dinput joystick
GUID                 joystickGUID;        // guid for main joystick
char                 joyname[80];         // name of joystick

// these contain the target records for all di input packets
UCHAR keyboard_state[256]; // contains keyboard state table
DIMOUSESTATE mouse_state;  // contains state of mouse
DIJOYSTATE joy_state;      // contains state of joystick
int joystick_found = 0;    // tracks if joystick was found and inited

// FUNCTIONS //////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK DInput_Enum_Joysticks(LPCDIDEVICEINSTANCE lpddi,
								LPVOID guid_ptr) 
{
// this function enumerates the joysticks, but
// stops at the first one and returns the
// instance guid of it, so we can create it

*(GUID*)guid_ptr = lpddi->guidInstance; 

// copy name into global
strcpy(joyname, (char *)lpddi->tszProductName);

// stop enumeration after one iteration
return(DIENUM_STOP);

} // end DInput_Enum_Joysticks

//////////////////////////////////////////////////////////////////////////////

int DInput_Init(void)
{
// this function initializes directinput

if (FAILED(DirectInputCreate(hMainInstance,DIRECTINPUT_VERSION,&lpdi,NULL)))
   return(0);

// return success
return(1);

} // end DInput_Init

///////////////////////////////////////////////////////////

void DInput_Shutdown(void)
{
// this function shuts down directinput

if (lpdi)
   lpdi->Release();

} // end DInput_Shutdown

///////////////////////////////////////////////////////////

int DInput_Init_Joystick(int min_x, int max_x, int min_y, int max_y, int dead_zone)
{
// this function initializes the joystick, it allows you to set
// the minimum and maximum x-y ranges 

// first find the fucking GUID of your particular joystick
lpdi->EnumDevices(DIDEVTYPE_JOYSTICK, 
                  DInput_Enum_Joysticks, 
                  &joystickGUID, 
                  DIEDFL_ATTACHEDONLY); 

// create a temporary IDIRECTINPUTDEVICE (1.0) interface, so we query for 2
LPDIRECTINPUTDEVICE lpdijoy_temp = NULL;

if (lpdi->CreateDevice(joystickGUID, &lpdijoy_temp, NULL)!=DI_OK)
   return(0);

// now that we have created the joystick device we need to
// obtain a new more advanced interface called 
// IDIRECTINPUTDEVICE2, this needs to be done using the low level
// COM method queryinterface(), unfortunetely createdevice
// still gives a 1.0 version interface, so we to do this manually

lpdijoy_temp->QueryInterface(IID_IDirectInputDevice2, 
                            (void **) &lpdijoy); 

// release the version 1.0 interface since we have converted
// it to 2.0
lpdijoy_temp->Release();

// set cooperation level
if (lpdijoy->SetCooperativeLevel(hMainWindowHandle, 
	                 DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
   return(0);

// set data format
if (lpdijoy->SetDataFormat(&c_dfDIJoystick)!=DI_OK)
   return(0);

// set the range of the joystick
DIPROPRANGE joy_axis_range;

// first x axis
joy_axis_range.lMin = min_x;
joy_axis_range.lMax = max_x;

joy_axis_range.diph.dwSize       = sizeof(DIPROPRANGE); 
joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
joy_axis_range.diph.dwObj        = DIJOFS_X;
joy_axis_range.diph.dwHow        = DIPH_BYOFFSET;

lpdijoy->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);

// now y-axis
joy_axis_range.lMin = min_y;
joy_axis_range.lMax = max_y;

joy_axis_range.diph.dwSize       = sizeof(DIPROPRANGE); 
joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
joy_axis_range.diph.dwObj        = DIJOFS_Y;
joy_axis_range.diph.dwHow        = DIPH_BYOFFSET;

lpdijoy->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);


// and now the dead band
DIPROPDWORD dead_band; // here's our property word

// scale dead zone by 100
dead_zone*=100;

dead_band.diph.dwSize       = sizeof(dead_band);
dead_band.diph.dwHeaderSize = sizeof(dead_band.diph);
dead_band.diph.dwObj        = DIJOFS_X;
dead_band.diph.dwHow        = DIPH_BYOFFSET;

// deadband will be used on both sides of the range +/-
dead_band.dwData            = dead_zone;

// finally set the property
lpdijoy->SetProperty(DIPROP_DEADZONE,&dead_band.diph);

dead_band.diph.dwSize       = sizeof(dead_band);
dead_band.diph.dwHeaderSize = sizeof(dead_band.diph);
dead_band.diph.dwObj        = DIJOFS_Y;
dead_band.diph.dwHow        = DIPH_BYOFFSET;

// deadband will be used on both sides of the range +/-
dead_band.dwData            = dead_zone;


// finally set the property
lpdijoy->SetProperty(DIPROP_DEADZONE,&dead_band.diph);

// acquire the joystick
if (lpdijoy->Acquire()!=DI_OK)
   return(0);

// set found flag
joystick_found = 1;

// return success
return(1);

} // end DInput_Init_Joystick

///////////////////////////////////////////////////////////

int DInput_Init_Mouse(void)
{
// this function intializes the mouse

// create a mouse device 
if (lpdi->CreateDevice(GUID_SysMouse, &lpdimouse, NULL)!=DI_OK)
   return(0);

// set cooperation level
if (lpdimouse->SetCooperativeLevel(hMainWindowHandle, 
                       DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
   return(0);

// set data format
if (lpdimouse->SetDataFormat(&c_dfDIMouse)!=DI_OK)
   return(0);

// acquire the mouse
if (lpdimouse->Acquire()!=DI_OK)
   return(0);

// return success
return(1);

} // end DInput_Init_Mouse

///////////////////////////////////////////////////////////

int DInput_Init_Keyboard(void)
{
// this function initializes the keyboard device

// create the keyboard device  
if (lpdi->CreateDevice(GUID_SysKeyboard, &lpdikey, NULL)!=DI_OK)
   return(0);

// set cooperation level
if (lpdikey->SetCooperativeLevel(hMainWindowHandle, 
                 DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
    return(0);

// set data format
if (lpdikey->SetDataFormat(&c_dfDIKeyboard)!=DI_OK)
   return(0);

// acquire the keyboard
if (lpdikey->Acquire()!=DI_OK)
   return(0);

// return success
return(1);

} // end DInput_Init_Keyboard

///////////////////////////////////////////////////////////

int DInput_Read_Joystick(void)
{
// this function reads the joystick state

// make sure the joystick was initialized
if (!joystick_found)
   return(0);

if (lpdijoy)
    {
    // this is needed for joysticks only    
    if (lpdijoy->Poll()!=DI_OK)
        return(0);

    if (lpdijoy->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joy_state)!=DI_OK)
        return(0);
    }
else
    {
    // joystick isn't plugged in, zero out state
    memset(&joy_state,0,sizeof(joy_state));

    // return error
    return(0);
    } // end else

// return sucess
return(1);

} // end DInput_Read_Joystick

///////////////////////////////////////////////////////////

int DInput_Read_Mouse(void)
{
// this function reads  the mouse state

if (lpdimouse)    
    {
    if (lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state)!=DI_OK)
        return(0);
    }
else
    {
    // mouse isn't plugged in, zero out state
    memset(&mouse_state,0,sizeof(mouse_state));

    // return error
    return(0);
    } // end else

// return sucess
return(1);

} // end DInput_Read_Mouse

///////////////////////////////////////////////////////////

int DInput_Read_Keyboard(void)
{
// this function reads the state of the keyboard

if (lpdikey)
    {
    if (lpdikey->GetDeviceState(256, (LPVOID)keyboard_state)!=DI_OK)
       return(0);
    }
else
    {
    // keyboard isn't plugged in, zero out state
    memset(keyboard_state,0,sizeof(keyboard_state));

    // return error
    return(0);
    } // end else

// return sucess
return(1);

} // end DInput_Read_Keyboard

///////////////////////////////////////////////////////////

void DInput_Release_Joystick(void)
{
// this function unacquires and releases the joystick

if (lpdijoy)
    {    
    lpdijoy->Unacquire();
    lpdijoy->Release();
    } // end if

} // end DInput_Release_Joystick

///////////////////////////////////////////////////////////

void DInput_Release_Mouse(void)
{
// this function unacquires and releases the mouse

if (lpdimouse)
    {    
    lpdimouse->Unacquire();
    lpdimouse->Release();
    } // end if

} // end DInput_Release_Mouse

///////////////////////////////////////////////////////////

void DInput_Release_Keyboard(void)
{
// this function unacquires and releases the keyboard

if (lpdikey)
    {
    lpdikey->Unacquire();
    lpdikey->Release();
    } // end if

} // end DInput_Release_Keyboard

///////////////////////////////////////////////////////////

bool DirectInputKeyDown(unsigned char DIK_KEY)
// Tests if a particular key is being pressed on the 
// keyboard using Direct Input
// Returns true if being pressed, false if not
{
	bool KeyIsDown = false;

	if (keyboard_state[DIK_KEY] & 0x80)
	{
		KeyIsDown = true;
	}

	return KeyIsDown;
};

///////////////////////////////////////////////////////////

void ConcatinateKeyboardInputToString(char* cpStringToConcatinateTo, int iMaxCharactersOfString)
// Concatinate whatever is in keyboard_state onto the end of
// the given char pointer
{
	char* cpNewString = NULL;

	// If string is full, then return
	if (strlen(cpStringToConcatinateTo) >= iMaxCharactersOfString)
	{
		return;		// Exit the function
	}
	
	// If Shift key is being pressed
	if (DirectInputKeyDown(DIK_LSHIFT) || DirectInputKeyDown(DIK_RSHIFT))
	{
		// Test which letter is being pressed and concatinate
		// that capital letter to the end of cpStringToConcatinateTo

		if (DirectInputKeyDown(DIK_A))
		{
			strncat(cpStringToConcatinateTo, "A", 1);
		}else if (DirectInputKeyDown(DIK_B))
		{
			strncat(cpStringToConcatinateTo, "B", 1);
		}else if (DirectInputKeyDown(DIK_C))
		{
			strncat(cpStringToConcatinateTo, "C", 1);
		}else if (DirectInputKeyDown(DIK_D))
		{
			strncat(cpStringToConcatinateTo, "D", 1);
		}else if (DirectInputKeyDown(DIK_E))
		{
			strncat(cpStringToConcatinateTo, "E", 1);
		}else if (DirectInputKeyDown(DIK_F))
		{
			strncat(cpStringToConcatinateTo, "F", 1);
		}else if (DirectInputKeyDown(DIK_G))
		{
			strncat(cpStringToConcatinateTo, "G", 1);
		}else if (DirectInputKeyDown(DIK_H))
		{
			strncat(cpStringToConcatinateTo, "H", 1);
		}else if (DirectInputKeyDown(DIK_I))
		{
			strncat(cpStringToConcatinateTo, "I", 1);
		}else if (DirectInputKeyDown(DIK_J))
		{
			strncat(cpStringToConcatinateTo, "J", 1);
		}else if (DirectInputKeyDown(DIK_K))
		{
			strncat(cpStringToConcatinateTo, "K", 1);
		}else if (DirectInputKeyDown(DIK_L))
		{
			strncat(cpStringToConcatinateTo, "L", 1);
		}else if (DirectInputKeyDown(DIK_M))
		{
			strncat(cpStringToConcatinateTo, "M", 1);
		}else if (DirectInputKeyDown(DIK_N))
		{
			strncat(cpStringToConcatinateTo, "N", 1);
		}else if (DirectInputKeyDown(DIK_O))
		{
			strncat(cpStringToConcatinateTo, "O", 1);
		}else if (DirectInputKeyDown(DIK_P))
		{
			strncat(cpStringToConcatinateTo, "P", 1);
		}else if (DirectInputKeyDown(DIK_Q))
		{
			strncat(cpStringToConcatinateTo, "Q", 1);
		}else if (DirectInputKeyDown(DIK_R))
		{
			strncat(cpStringToConcatinateTo, "R", 1);
		}else if (DirectInputKeyDown(DIK_S))
		{
			strncat(cpStringToConcatinateTo, "S", 1);
		}else if (DirectInputKeyDown(DIK_T))
		{
			strncat(cpStringToConcatinateTo, "T", 1);
		}else if (DirectInputKeyDown(DIK_U))
		{
			strncat(cpStringToConcatinateTo, "U", 1);
		}else if (DirectInputKeyDown(DIK_V))
		{
			strncat(cpStringToConcatinateTo, "V", 1);
		}else if (DirectInputKeyDown(DIK_W))
		{
			strncat(cpStringToConcatinateTo, "W", 1);
		}else if (DirectInputKeyDown(DIK_X))
		{
			strncat(cpStringToConcatinateTo, "X", 1);
		}else if (DirectInputKeyDown(DIK_Y))
		{
			strncat(cpStringToConcatinateTo, "Y", 1);
		}else if (DirectInputKeyDown(DIK_Z))
		{
			strncat(cpStringToConcatinateTo, "Z", 1);
		}
	}
	// If Shift key is not being pressed
	else
	{
		if (DirectInputKeyDown(DIK_A))
		{
			strncat(cpStringToConcatinateTo, "a", 1);
		}else if (DirectInputKeyDown(DIK_B))
		{
			strncat(cpStringToConcatinateTo, "b", 1);
		}else if (DirectInputKeyDown(DIK_C))
		{
			strncat(cpStringToConcatinateTo, "c", 1);
		}else if (DirectInputKeyDown(DIK_D))
		{
			strncat(cpStringToConcatinateTo, "d", 1);
		}else if (DirectInputKeyDown(DIK_E))
		{
			strncat(cpStringToConcatinateTo, "e", 1);
		}else if (DirectInputKeyDown(DIK_F))
		{
			strncat(cpStringToConcatinateTo, "f", 1);
		}else if (DirectInputKeyDown(DIK_G))
		{
			strncat(cpStringToConcatinateTo, "g", 1);
		}else if (DirectInputKeyDown(DIK_H))
		{
			strncat(cpStringToConcatinateTo, "h", 1);
		}else if (DirectInputKeyDown(DIK_I))
		{
			strncat(cpStringToConcatinateTo, "i", 1);
		}else if (DirectInputKeyDown(DIK_J))
		{
			strncat(cpStringToConcatinateTo, "j", 1);
		}else if (DirectInputKeyDown(DIK_K))
		{
			strncat(cpStringToConcatinateTo, "k", 1);
		}else if (DirectInputKeyDown(DIK_L))
		{
			strncat(cpStringToConcatinateTo, "l", 1);
		}else if (DirectInputKeyDown(DIK_M))
		{
			strncat(cpStringToConcatinateTo, "m", 1);
		}else if (DirectInputKeyDown(DIK_N))
		{
			strncat(cpStringToConcatinateTo, "n", 1);
		}else if (DirectInputKeyDown(DIK_O))
		{
			strncat(cpStringToConcatinateTo, "o", 1);
		}else if (DirectInputKeyDown(DIK_P))
		{
			strncat(cpStringToConcatinateTo, "p", 1);
		}else if (DirectInputKeyDown(DIK_Q))
		{
			strncat(cpStringToConcatinateTo, "q", 1);
		}else if (DirectInputKeyDown(DIK_R))
		{
			strncat(cpStringToConcatinateTo, "r", 1);
		}else if (DirectInputKeyDown(DIK_S))
		{
			strncat(cpStringToConcatinateTo, "s", 1);
		}else if (DirectInputKeyDown(DIK_T))
		{
			strncat(cpStringToConcatinateTo, "t", 1);
		}else if (DirectInputKeyDown(DIK_U))
		{
			strncat(cpStringToConcatinateTo, "u", 1);
		}else if (DirectInputKeyDown(DIK_V))
		{
			strncat(cpStringToConcatinateTo, "v", 1);
		}else if (DirectInputKeyDown(DIK_W))
		{
			strncat(cpStringToConcatinateTo, "w", 1);
		}else if (DirectInputKeyDown(DIK_X))
		{
			strncat(cpStringToConcatinateTo, "x", 1);
		}else if (DirectInputKeyDown(DIK_Y))
		{
			strncat(cpStringToConcatinateTo, "y", 1);
		}else if (DirectInputKeyDown(DIK_Z))
		{
			strncat(cpStringToConcatinateTo, "z", 1);
		}else if (DirectInputKeyDown(DIK_SPACE))
		{
			strncat(cpStringToConcatinateTo, " ", 1);
		}else if (DirectInputKeyDown(DIK_0))
		{
			strncat(cpStringToConcatinateTo, "0", 1);
		}else if (DirectInputKeyDown(DIK_1))
		{
			strncat(cpStringToConcatinateTo, "1", 1);
		}else if (DirectInputKeyDown(DIK_2))
		{
			strncat(cpStringToConcatinateTo, "2", 1);
		}else if (DirectInputKeyDown(DIK_3))
		{
			strncat(cpStringToConcatinateTo, "3", 1);
		}else if (DirectInputKeyDown(DIK_4))
		{
			strncat(cpStringToConcatinateTo, "4", 1);
		}else if (DirectInputKeyDown(DIK_5))
		{
			strncat(cpStringToConcatinateTo, "5", 1);
		}else if (DirectInputKeyDown(DIK_6))
		{
			strncat(cpStringToConcatinateTo, "6", 1);
		}else if (DirectInputKeyDown(DIK_7))
		{
			strncat(cpStringToConcatinateTo, "7", 1);
		}else if (DirectInputKeyDown(DIK_8))
		{
			strncat(cpStringToConcatinateTo, "8", 1);
		}else if (DirectInputKeyDown(DIK_9))
		{
			strncat(cpStringToConcatinateTo, "9", 1);
		}else if (DirectInputKeyDown(DIK_MULTIPLY))
		{
			strncat(cpStringToConcatinateTo, "*", 1);
		}else if (DirectInputKeyDown(DIK_SEMICOLON))
		{
			strncat(cpStringToConcatinateTo, ";", 1);
		}else if (DirectInputKeyDown(DIK_COMMA))
		{
			strncat(cpStringToConcatinateTo, ",", 1);
		}else if (DirectInputKeyDown(DIK_BACKSLASH))
		{
			strncat(cpStringToConcatinateTo, "\ ", 1);
		}else if (DirectInputKeyDown(DIK_EQUALS))
		{
			strncat(cpStringToConcatinateTo, "=", 1);
		}else if (DirectInputKeyDown(DIK_ADD))
		{
			strncat(cpStringToConcatinateTo, "+", 1);
		}else if (DirectInputKeyDown(DIK_PERIOD) || DirectInputKeyDown(DIK_DECIMAL))
		{
			strncat(cpStringToConcatinateTo, ".", 1);
		}else if (DirectInputKeyDown(DIK_MINUS) || DirectInputKeyDown(DIK_SUBTRACT))
		{
			strncat(cpStringToConcatinateTo, "-", 1);
		}else if (DirectInputKeyDown(DIK_SLASH) || DirectInputKeyDown(DIK_DIVIDE))
		{
			strncat(cpStringToConcatinateTo, "/", 1);
		}else if (DirectInputKeyDown(DIK_COLON))
		{
			strncat(cpStringToConcatinateTo, ":", 1);
		}else if (DirectInputKeyDown(DIK_BACKSPACE))
		{
			// Erase last character in the string
			cpStringToConcatinateTo[strlen(cpStringToConcatinateTo)-1] = NULL;
		}
	}
}