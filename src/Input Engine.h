// T3DLIB2.H - Header file for T3DLIB2.CPP game engine library

// watch for multiple inclusions
#ifndef T3DLIB2
#define T3DLIB2

// DEFINES ////////////////////////////////////////////////

// MACROS /////////////////////////////////////////////////



// TYPES //////////////////////////////////////////////////



// PROTOTYPES /////////////////////////////////////////////


// input
int DInput_Init(void);
void DInput_Shutdown(void);
int DInput_Init_Joystick(int min_x=-256, int max_x=256, int min_y=-256, int max_y=256, int dead_band = 10);
int DInput_Init_Mouse(void);
int DInput_Init_Keyboard(void);
int DInput_Read_Joystick(void);
int DInput_Read_Mouse(void);
int DInput_Read_Keyboard(void);
void DInput_Release_Joystick(void);
void DInput_Release_Mouse(void);
void DInput_Release_Keyboard(void);

bool DirectInputKeyDown(unsigned char DIK_KEY);
void ConcatinateKeyboardInputToString(char* cpStringToConcatinateTo, int iMaxCharactersOfString);

// GLOBALS ////////////////////////////////////////////////


// EXTERNALS //////////////////////////////////////////////

extern HWND hMainWindowHandle; // save the window handle
extern HINSTANCE hMainInstance; // save the instance

// directinput globals
extern LPDIRECTINPUT        lpdi;       // dinput object
extern LPDIRECTINPUTDEVICE  lpdikey;    // dinput keyboard
extern LPDIRECTINPUTDEVICE  lpdimouse;  // dinput mouse
extern LPDIRECTINPUTDEVICE2 lpdijoy;    // dinput joystick 
extern GUID                 joystickGUID; // guid for main joystick
extern char                 joyname[80];  // name of joystick

// these contain the target records for all di input packets
extern UCHAR keyboard_state[256]; // contains keyboard state table
extern DIMOUSESTATE mouse_state;  // contains state of mouse
extern DIJOYSTATE joy_state;      // contains state of joystick
extern int joystick_found;        // tracks if stick is plugged in
#endif

