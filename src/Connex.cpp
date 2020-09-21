// Connex.cpp	Copyright 2003

//** INCLUDES ********************************************************

// Include all the Windows and Direct X stuff and our Clock class
#include "Win&DX7.h"
#include "CClock.h"
#include "CHighScores.h"

// Include our graphics, input, and sound engines
#include "Graphics Engine.h"
#include "Input Engine.h"
#include "Sound Engine.h"

// Inclue our class to hold our blocks and the board
#include "CBlock.h"

// Needed to read/write files
#include <fstream>

using namespace std;

//** GLOBALS AND CONSTANTS *******************************************

// The name of this game
char GcGameName[] = "Daniel Schroeders Connex";
char GcSettingsFileName[] = "Settings.dat";

// Screen and palette properties
const int iSCREEN_WIDTH			= 800;
const int iSCREEN_HEIGHT		= 600;
const int iSCREEN_BPP			= 8;

const int iMAX_PALETTE_COLORS	= 256;

// Globals to track our main window
HWND hMainWindowHandle		= NULL;	// Track the main window
HINSTANCE hMainInstance		= NULL;	// Track the program instance

// Global Music ID's
int GiLevel1Music	= 0;
int GiLevel2Music	= 0;
int GiLevel3Music	= 0;
int GiLevel4Music	= 0;
int GiLevel5Music	= 0;
int GiLevel6Music	= 0;
int GiLevel7Music	= 0;
int GiLevel8Music	= 0;
int GiLevel9Music	= 0;
int GiLevel10Music	= 0;

int GiCurrentMusic = 0;	// Globally keeps track of which Music is playing

// Global Sound ID's
int GiClear1LineSound			= 0;
int GiClear2LinesSound			= 1;
int GiClear3LinesSound			= 2;
int GiClear4LinesSound			= 3;
int GiConfirmColorChangeSound	= 4;
int GiCreateNewBlockSound		= 5;
int GiGameOverSound				= 6;
int GiGameStartSound			= 7;
int GiGameWonSound				= 8;
int GiNewLevelSound				= 9;
int GiPauseSound				= 10;
int GiRandomizeWarningSound		= 11;
int GiRandomizeBlockSound		= 11;


// Background, Text, and Block Colors
const int iI_COLOR				= 1;
const int iL_COLOR				= 2;
const int iBACKWARDS_L_COLOR	= 3;
const int iT_COLOR				= 4;
const int iS_COLOR				= 5;
const int iZ_COLOR				= 6;
const int iSQUARE_COLOR			= 7;

const int iPLAY_BACKGROUND_COLOR	= 8;
const int iPLAY_TEXT_COLOR			= 9;

const int iOPTIONS_TEXT_COLOR		= 10;

const int iBACKGROUND_COLOR			= 11;

const int iBOARD_COLOR				= 12;

const int iBLACK_COLOR				= 0;
const int iWHITE_COLOR				= (iMAX_PALETTE_COLORS - 1);


// Wait Times before accepting new input and creating new Blocks
const unsigned long ulWAIT_TIME_BEFORE_NEW_MOVEMENT_INPUT	= 110;
const unsigned long ulWAIT_TIME_BEFORE_OPTIONS_INPUT		= 300;
const unsigned long ulWAIT_TIME_BEFORE_ROTATING_BLOCK		= 200;
const unsigned long ulWAIT_TIME_BEFORE_CREATING_NEW_BLOCK	= 250;
const unsigned long ulWAIT_TIME_BEFORE_NEW_NAME_INPUT_VALID	= 100;

const unsigned long ulMIN_WAIT_TIME_BEFORE_RANDOMIZING_BLOCK= 7000;
const unsigned long ulMAX_WAIT_TIME_BEFORE_RANDOMIZING_BLOCK= 20000;

// How many Lines are needed to advance to the next Level, and how many Levels in Total
const int iLINES_PER_LEVEL									= 10;
const int iNUMBER_OF_LEVELS									= 10;

// The speed of the falling Block at each Level
const unsigned long ulLEVEL_1_BLOCK_SPEED	= 800;
const unsigned long ulLEVEL_2_BLOCK_SPEED	= 600;
const unsigned long ulLEVEL_3_BLOCK_SPEED	= 400;
const unsigned long ulLEVEL_4_BLOCK_SPEED	= 350;
const unsigned long ulLEVEL_5_BLOCK_SPEED	= 300;
const unsigned long ulLEVEL_6_BLOCK_SPEED	= 265;
const unsigned long ulLEVEL_7_BLOCK_SPEED	= 225;
const unsigned long ulLEVEL_8_BLOCK_SPEED	= 200;
const unsigned long ulLEVEL_9_BLOCK_SPEED	= 175;
const unsigned long ulLEVEL_10_BLOCK_SPEED	= 150;

// How many points are awarded depending on how many Lines are cleared
const int iPOINTS_FOR_1_LINE	= 10;
const int iPOINTS_FOR_2_LINES	= 40;
const int iPOINTS_FOR_3_LINES	= 75;
const int iPOINTS_FOR_4_LINES	= 250;
const int iBASE_POINTS_FOR_RANDOMIZE = 5; // The number gets multiplied by the current level the player is on

// Position where to display the Next Block
const int iNEXT_BLOCK_POSITION_X	= 500;
const int iNEXT_BLOCK_POSITION_Y	= 250;

// Holds all the game states
enum eGameStates
{
	eGAME_STATE_INTRODUCTION,
	eGAME_STATE_PLAY,
	eGAME_STATE_GAME_OVER,
	eGAME_STATE_WIN
};

unsigned int GuiScore						= 0;	// Holds the players Score
unsigned int GuiLevel						= 1;	// Holds which level player is on, starting on Level 1
unsigned int GuiTotalNumberOfCompletedLines	= 0;	// Holds the Total Number Of Completed Lines
unsigned int GuiLineStats[4];						// Holds how many 1, 2, 3, and 4 liners the player has gotten
unsigned int GuiBlocksGenerated				= 0;	// Holds how many Blocks have been generated so far in the game
unsigned int GuiTotalRandomizes				= 0;	// Holds how many times a Block was randomized on the player (in Extreme mode)

bool GbMusicOn	= true;
bool GbPaused	= false;
bool GbExtremeMode = false;
bool GbCheated = false;

// Timers to keep track of when things should occur
CClock GcBlockFallTimer;
CClock GcWaitBeforeCreatingNewBlockTimer;
CClock GcWaitBeforeRandomizingBlockTimer;

// Holds which state the game is currently in, and sets the 
// starting State to the Introduction
eGameStates GeGameState = eGAME_STATE_INTRODUCTION;

// The falling block 
// Also holds all the other blocks that have already fallen on the board
CBlock GcCurrentBlock;

CBlock GcNextBlock;		// The next Block that Current Block will be

CClock GcMovementKeyPressTimer;	// Holds how long it has been since the last player input
CClock GcRotationKeyPressTimer;	// Holds how long it has been since the last rotation input
CClock GcOptionsKeyPressTimer;	// Holds how long since the last Options input from player


//** Function Prototypes *********************************************

// Game Setup, Main, and Shutdown functions
void GameInitialize();
void GameMain();
void GameShutdown();

// Game State functions
void GameStatePlay();
void GameStateGameOver();
void GameStateWin();
void GameStateIntroduction();

bool DirectInputKeyDown(unsigned char DIK_KEY);	// Test for keys pressed

bool ProcessMovementInput(bool& bBottomHitWhilePressingDown);	// Process user input when in Game State Play. Returns true if there was input to process
bool ProcessRotationInput();	// Process input associated with Rotating the Block
bool ProcessOptionsInput();		// Process input associated with Options

PALETTEENTRY& BuildPalette();// Builds the Game's Palette and returns it

void LoopMusic();			// Makes sure which ever song GiCurrentMusic is pointing to, is playing

void Fps();					// Display the Frames Per Second to the screen


//** Windows Procedure Function **************************************

LRESULT CALLBACK WindowsMessageHandler(HWND hWindow, UINT uiMessage, 
							WPARAM wParameter, LPARAM lParameter)
// Specifies how to handle messages from my game to windows
{
	// What is the message 
	switch(uiMessage)
	{
		case WM_DESTROY: 
	
			// Kill the application by sending a WM_QUIT message 
			PostQuitMessage(0);

			// Return success
			return(0);
		break;

		default:
		break;
	}

	// Let windows take care of any other messages
	return (DefWindowProc(hWindow, uiMessage, wParameter, lParameter));
}


//** WinMain *********************************************************

int WINAPI WinMain(	HINSTANCE hInstance, HINSTANCE hPreviousInstance,
					LPSTR lpCommandLine, int iCommandShow)
{
	WNDCLASSEX	cMainWindow;	// This will hold the class we create
	HWND		hWindow;		// A generic window handle
	MSG			msgMessage;		// A generic message

	bool		bForever = true;// Used for main game loop

	// first fill in the window class stucture
	cMainWindow.cbSize			= sizeof(WNDCLASSEX);
	cMainWindow.style			= CS_DBLCLKS | CS_OWNDC | 
								  CS_HREDRAW | CS_VREDRAW;
	cMainWindow.lpfnWndProc		= WindowsMessageHandler;
	cMainWindow.cbClsExtra		= 0;
	cMainWindow.cbWndExtra		= 0;
	cMainWindow.hInstance		= hInstance;
	cMainWindow.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	cMainWindow.hCursor			= LoadCursor(NULL, IDC_ARROW); 
	cMainWindow.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	cMainWindow.lpszMenuName	= NULL;
	cMainWindow.lpszClassName	= "Full Screen Mode";
	cMainWindow.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	// Save the Main Window Instance in the global
	hMainInstance = hInstance;

	// Register the window class
	if (!RegisterClassEx(&cMainWindow))
		return(0);

	// Create the window
	if (!(hWindow = CreateWindowEx(NULL,                // Extended style
								"Full Screen Mode",		// Class name
								"Connex",				// Title of window
								WS_POPUP | WS_VISIBLE,	// Style of window
					 			0,0,					// Initial x,y of window
								iSCREEN_WIDTH,iSCREEN_HEIGHT,  // Initial width, height of window
								NULL,		// Handle to parent window
								NULL,		// Handle to menu
								hInstance,	// Instance of this application
								NULL)))		// Extra creation parameters
	return(0);

	// Save the Main Window Handle in the global
	hMainWindowHandle = hWindow;

	// Initialize game components
	GameInitialize();

	// Enter the main event loop
	while(bForever)
	{		
		// Test if there is a message in the queue. If so get it
		if (PeekMessage(&msgMessage, NULL, 0, 0, PM_REMOVE))
		{ 
		   // Test if this is a quit message
		   if (msgMessage.message == WM_QUIT)
		   {
			   break;
		   }
		
		   // Translate any accelerator keys
		   TranslateMessage(&msgMessage);

		   // Send the message to the window procedure
		   DispatchMessage(&msgMessage);
		}
    
		// main game processing goes here
		GameMain();
}

// Release all game components 
GameShutdown();

// Return to Windows
return(msgMessage.wParam);

} // end WinMain


//** Game Main *******************************************************

void GameMain()
// Main loop of the game
{
	// Check which State the game is in
	switch (GeGameState)
	{
		// If player should be playing game
		case eGAME_STATE_PLAY:
			
			// Play the game
			GameStatePlay();
		break;

		// If the player "died"
		case eGAME_STATE_GAME_OVER:

			// Run the Game Over sequence
			GameStateGameOver();
		break;

		// If player should be at the Introduction screen
		case eGAME_STATE_INTRODUCTION:

			// Show the Introduction screen
			GameStateIntroduction();
		break;

		// If player has won the game
		case eGAME_STATE_WIN:

			// Show the win screen
			GameStateWin();
		break;

		// If something went wrong just end game
		default:

			// Display the error occured
			MessageBox(hMainWindowHandle, "Will Now End Program", "Invalid Game State", MB_OK);

			// Exit the program
			SendMessage(hMainWindowHandle, WM_DESTROY, 0, 0);
		break;
	};
};


//** Game Initialize *************************************************

void GameInitialize()
// Sets up game resources including DirectDraw, DirectInput, etc
{
	// Local variables
	char cBuffer[30];
	ifstream ifSettings(GcSettingsFileName, ios_base::in);

	// Make random numbers possible
	srand((unsigned int)GetTickCount());

	// Hide the mouse
	ShowCursor(false);


	// Set up Direct Draw
	DDraw_Init(iSCREEN_WIDTH, iSCREEN_HEIGHT, iSCREEN_BPP);
	
	// Set the clipping region
	RECT rectClippingRegion = {0, 0, iSCREEN_WIDTH, iSCREEN_HEIGHT};

	// Attach the clipper to the back and primary surface
	lpddclipper = DDraw_Attach_Clipper(lpddsback, 1, &rectClippingRegion);

	// Create and Attach our Palette
	Set_Palette(&BuildPalette());


	// Set up Direct Input and Get the Devices
	DInput_Init();
	DInput_Init_Keyboard();


	// Set up Direct Sound and Direct Music
	// NOTE: Direct Sound must be called BEFORE Direct Music
	DSound_Init();
	DMusic_Init();

	// Load our Music into memory and store the ID's
	GiLevel1Music = DMusic_Load_MIDI("Music/Level 1.mid");
	GiLevel2Music = DMusic_Load_MIDI("Music/Level 2.mid");
	GiLevel3Music = DMusic_Load_MIDI("Music/Level 3.mid");
	GiLevel4Music = DMusic_Load_MIDI("Music/Level 4.mid");
	GiLevel5Music = DMusic_Load_MIDI("Music/Level 5.mid");
	GiLevel6Music = DMusic_Load_MIDI("Music/Level 6.mid");
	GiLevel7Music = DMusic_Load_MIDI("Music/Level 7.mid");
	GiLevel8Music = DMusic_Load_MIDI("Music/Level 8.mid");
	GiLevel9Music = DMusic_Load_MIDI("Music/Level 9.mid");
	GiLevel10Music = DMusic_Load_MIDI("Music/Level 10.mid");

	// Load our Sounds into memory and store the ID's	
	GiClear1LineSound			= DSound_Load_WAV("Sounds/Clear 1 Line.wav");
	GiClear2LinesSound			= DSound_Load_WAV("Sounds/Clear 2 Lines.wav");
	GiClear3LinesSound			= DSound_Load_WAV("Sounds/Clear 3 Lines.wav");
	GiClear4LinesSound			= DSound_Load_WAV("Sounds/Clear 4 Lines.wav");
	GiConfirmColorChangeSound	= DSound_Load_WAV("Sounds/Confirm Color Change.wav");
	GiCreateNewBlockSound		= DSound_Load_WAV("Sounds/Create New Block.wav");
	GiGameOverSound				= DSound_Load_WAV("Sounds/Game Over.wav");
	GiGameStartSound			= DSound_Load_WAV("Sounds/Game Start.wav");
	GiGameWonSound				= DSound_Load_WAV("Sounds/Game Won.wav");
	GiNewLevelSound				= DSound_Load_WAV("Sounds/New Level.wav");
	GiPauseSound				= DSound_Load_WAV("Sounds/Pause.wav");
	GiRandomizeWarningSound		= DSound_Load_WAV("Sounds/Randomize Warning.wav");
	GiRandomizeBlockSound		= DSound_Load_WAV("Sounds/Randomize Block.wav");

	// Sounds are much louder than music, so reduce sound volume
	DSound_Set_Volume(GiClear1LineSound, 50);
	DSound_Set_Volume(GiClear2LinesSound, 80);
	DSound_Set_Volume(GiClear3LinesSound, 80);
	DSound_Set_Volume(GiClear4LinesSound, 80);
	DSound_Set_Volume(GiConfirmColorChangeSound, 50);
	DSound_Set_Volume(GiCreateNewBlockSound, 40);
	DSound_Set_Volume(GiGameOverSound, 100);
	DSound_Set_Volume(GiGameStartSound, 80);
	DSound_Set_Volume(GiGameWonSound, 100);
	DSound_Set_Volume(GiNewLevelSound, 30);
	DSound_Set_Volume(GiPauseSound, 100);
	DSound_Set_Volume(GiRandomizeWarningSound, 100);
	DSound_Set_Volume(GiRandomizeBlockSound, 100);

	// Keep track of which song is playing
	GiCurrentMusic = GiLevel1Music;

	// Play the Game Starting Sound
	DSound_Play(GiGameStartSound);


	// Set up the Current and Next Blocks

	// Set Current and Next Blocks Board Colors
	GcCurrentBlock.mcBoard.SetBoardColor(iBOARD_COLOR);
	GcNextBlock.mcBoard.SetBoardColor(iPLAY_BACKGROUND_COLOR);

	// Place Next Block in the correct Position
	GcNextBlock.mcBoard.SetBoardPosition(iNEXT_BLOCK_POSITION_X, iNEXT_BLOCK_POSITION_Y);
	GcNextBlock.MoveEntireBlockToCell(0, 0);

	// Set up Next Block so we cannot see the Board (We only want to see the Next Block, not the Board)
	GcNextBlock.mcBoard.MakeBoardStyleTransparent();
	GcNextBlock.mcBoard.TurnBorderOff();
	GcNextBlock.mcBoard.TurnGridOff();

	// Set Next Block up with a different random Block, since the first
	// one is the same as Current Blocks random Block
	GcNextBlock.mcBoard.ClearOutBoard();
	GcNextBlock.ChangeBlockType();
	GcNextBlock.PlaceEntireBlockOnBoard();

	// Make sure there is nothing on Current Blocks Board, and place
	// Current Block on the Board
	GcCurrentBlock.mcBoard.ClearOutBoard();
	GcCurrentBlock.PlaceEntireBlockOnBoard();


	// Read in settings from file until we reach end of file
	while (ifSettings && ifSettings.getline(cBuffer, 99999, '\n'))
	{		
		// If Music should be On
		if (!strcmp(cBuffer, "Music=On"))
		{
			GbMusicOn = true;
		}
		// If Music should be Off
		else if (!strcmp(cBuffer, "Music=Off"))
		{
			GbMusicOn = false;
		}

		// If Board Grid should be On
		if (!strcmp(cBuffer, "BoardGrid=On"))
		{
			GcCurrentBlock.mcBoard.TurnGridOn();
		}
		// If Board Grid should be Off
		else if (!strcmp(cBuffer, "BoardGrid=Off"))
		{
			GcCurrentBlock.mcBoard.TurnGridOff();
		}

		// If Board Border should be On
		if (!strcmp(cBuffer, "BoardBorder=On"))
		{
			GcCurrentBlock.mcBoard.TurnBorderOn();
		}
		// If Board Border should be Off
		else if (!strcmp(cBuffer, "BoardBorder=Off"))
		{
			GcCurrentBlock.mcBoard.TurnBorderOff();
		}

		// If Board shoudl be Transparent
		if (!strcmp(cBuffer, "BoardTransparent=On"))
		{
			GcCurrentBlock.mcBoard.MakeBoardStyleTransparent();
		}
		// If Board should be Solid
		else if (!strcmp(cBuffer, "BoardTransparent=Off"))
		{
			GcCurrentBlock.mcBoard.MakeBoardStyleSolid();
		}
	}

	// Start our Key Pressed Timers
	GcOptionsKeyPressTimer.Start();
	GcMovementKeyPressTimer.Start();
	GcRotationKeyPressTimer.Start();
};


//** Game Shutdown ***************************************************

void GameShutdown()
// Deallocates main games resources
{
	// Local Variables
	ofstream ofSettings(GcSettingsFileName);

	// Release all of our Midi's and shutdown Direct Music
	DMusic_Delete_All_MIDI();
	DMusic_Shutdown();

	// Stop any playing Wav's
	DSound_Stop_All_Sounds();

	// Release all of the Wav's and shutdown Direct Sound
	DSound_Delete_All_Sounds();
	DSound_Shutdown();
	
	// Shut down Direct Input and release devices
	DInput_Release_Keyboard();
	DInput_Shutdown();

	// Shut down Direct Draw
	DDraw_Shutdown();

	// Save settings to file
	if (ofSettings)
	{
		// Save if Music is on or not
		if (GbMusicOn)
		{
			ofSettings << "Music=On" << endl;
		}else
		{
			ofSettings << "Music=Off" << endl;
		}

		// Save if Board Grid is on or not
		if (GcCurrentBlock.mcBoard.GridIsOn())
		{
			ofSettings << "BoardGrid=On" << endl;
		}else
		{
			ofSettings << "BoardGrid=Off" << endl;
		}

		// Save if Border is on or not
		if (GcCurrentBlock.mcBoard.BorderIsOn())
		{
			ofSettings << "BoardBorder=On" << endl;
		}else
		{
			ofSettings << "BoardBorder=Off" << endl;
		}
		
		// Save if Board is Transparent or not
		if (GcCurrentBlock.mcBoard.ReturnBoardStyle() == eBOARD_STYLE_TRANSPARENT)
		{
			ofSettings << "BoardTransparent=On" << endl;
		}else
		{
			ofSettings << "BoardTransparent=Off" << endl;
		}
	}
};


//** Play The Game ***************************************************

void GameStatePlay()
{
	static bool SbGamePlayStateHasBeenRanBefore	= false;	// Keeps track of if Game Play State is just starting for the first time
	static bool SbBlockHasHitSomething			= false;	// Keeps track of if the block has fallen as far as it can
	static bool SbRandomizeWarningHasPlayed		= false;	// Set warning to not have played yet
	
	// Holds how fast the block should fall, starting Level 1 speed
	static unsigned long SulBlockFallWaitTime = ulLEVEL_1_BLOCK_SPEED;

	// Holds how long to wait before randomizing block
	static unsigned long SulBlockRandomizeDuration = ulMIN_WAIT_TIME_BEFORE_RANDOMIZING_BLOCK;
	static unsigned long SulTimeBeforeNextBlockRandomize = 0;

	// How long it has been since each timer was last reset
	unsigned long ulBlockFallTimeDifference = 0;
	unsigned long ulWaitBeforeCreatingNewBlockTimeDifference = 0;
	
	int iNumberOfCompletedLines				= 0;	// Holds how many lines the player removed with this block, if any
	unsigned int uiOldLevel					= 0;	// Holds what Level player was on before obtaining more Completed Lines

	// Holds the Text to be written to the screen, and the Text Color
	char cScore[20]							= " ";
	char cLevel[10]							= " ";
	char cTotalNumberOfCompletedLines[25]	= " ";
	char c1Liners[20]						= " ";
	char c2Liners[20]						= " ";
	char c3Liners[20]						= " ";
	char c4Liners[20]						= " ";
	char cBlocksGenerated[30]				= " ";
	char cRandomizes[20]					= " ";
	char cRandomizeTimeRemaining[40]		= " ";


	// If we have not ran the Play State before
	if (!SbGamePlayStateHasBeenRanBefore)
	{
		// Reset game data for a new game
		GuiScore						= 0;	// Holds the players Score
		GuiLevel						= 1;	// Holds which level player is on, starting on Level 1
		GuiTotalNumberOfCompletedLines	= 0;	// Holds the Total Number Of Completed Lines
		GuiLineStats[0] = GuiLineStats[1] = GuiLineStats[2] = GuiLineStats[3] = 0;	// Number of 1, 2, 3, and 4 liners player has gotten
		GuiBlocksGenerated				= 1;	// Holds how many blocks have been generated so far (Initialized to 1 to include starting block)
		GuiTotalRandomizes				= 0;	// Holds number of Randomizes so far
		GbCheated						= false;// Record that playe has not cheated yet

		// Reset how long before first Block Randomize
		SulBlockRandomizeDuration = ulMIN_WAIT_TIME_BEFORE_RANDOMIZING_BLOCK;
		SulTimeBeforeNextBlockRandomize = 0;

		// Make sure there is nothing on the Board, and place the Current Block on the Board
		GcCurrentBlock.mcBoard.ClearOutBoard();
		GcCurrentBlock.PlaceEntireBlockOnBoard();

		// Record that Block is not hitting anything, since this is the first block produced
		SbBlockHasHitSomething = false;

		// Set that warning sound hasn't played yet
		SbRandomizeWarningHasPlayed	= false;

		// Set to Block Fall speed to Level 1
		SulBlockFallWaitTime = ulLEVEL_1_BLOCK_SPEED;

		// Set music to Level 1 Music if not already
		if (GiCurrentMusic != GiLevel1Music)
		{
			// Stop current music from playing
			DMusic_Stop(GiCurrentMusic);

			// If Music is turned on
			if (GbMusicOn)
			{
				// Start playing Level 1's music
				DMusic_Play(GiLevel1Music);
			}

			// Keep track of which song is playing
			GiCurrentMusic = GiLevel1Music;
		}

		// Start our Timers
		GcBlockFallTimer.Start();
		GcWaitBeforeCreatingNewBlockTimer.Start();
		GcWaitBeforeRandomizingBlockTimer.Start();

		// Make sure we do not accidentally start it again
		SbGamePlayStateHasBeenRanBefore = true;
	}

	// Read the keyboard input
	DInput_Read_Keyboard();

	// If user is pressing Escape, then quit
	if (DirectInputKeyDown(DIK_ESCAPE))
	{
		PostMessage(hMainWindowHandle, WM_DESTROY, 0, 0);
	}

	// If the game is not Paused
	if (!GbPaused)
	{
		// If Block has not hit anything
		if (!SbBlockHasHitSomething)
		{
			// If it is time to check for new Movement input
			if ((GcMovementKeyPressTimer.GetTime() - GcMovementKeyPressTimer.TellStartTime()) > ulWAIT_TIME_BEFORE_NEW_MOVEMENT_INPUT)
			{
				// If there is input then process it
				if (ProcessMovementInput(SbBlockHasHitSomething))
				{
					// Restart input timer
					GcMovementKeyPressTimer.Start();
				}
			}

			// If it is time to check for new Rotation input
			if ((GcRotationKeyPressTimer.GetTime() - GcRotationKeyPressTimer.TellStartTime()) > ulWAIT_TIME_BEFORE_ROTATING_BLOCK)
			{
				// If there is input then process it
				if (ProcessRotationInput())
				{
					// Restart input timer
					GcRotationKeyPressTimer.Start();
				}
			}
		}
	}

	// If it is time to check for new Options input
	if ((GcOptionsKeyPressTimer.GetTime() - GcOptionsKeyPressTimer.TellStartTime()) > ulWAIT_TIME_BEFORE_OPTIONS_INPUT)
	{
		// If there is input then process it
		if (ProcessOptionsInput())
		{
			// Restart the input timer
			GcOptionsKeyPressTimer.Start();
		}
	}

	// If game is not Paused
	if (!GbPaused)
	{
		// Holds how long our Block Fall Timer has been running for without being restarted
		ulBlockFallTimeDifference = GcBlockFallTimer.GetTime() - GcBlockFallTimer.TellStartTime();

		// If it is time to make block fall and block has not hit anything
		if (ulBlockFallTimeDifference > SulBlockFallWaitTime && !SbBlockHasHitSomething)
		{
			// If possible move block down one cell
			if (!GcCurrentBlock.MoveBlockDown())
			{
				// Block has hit something so let us know
				SbBlockHasHitSomething = true;

				// Start the Create a New Block timer
				GcWaitBeforeCreatingNewBlockTimer.Start();
			}

			// Restart the Block Fall timer
			GcBlockFallTimer.Start();
		}

		// If block has hit something
		if (SbBlockHasHitSomething)
		{			
			// Holds how long our Wait Before Creating a New Block Timer has been running for without being restarted
			ulWaitBeforeCreatingNewBlockTimeDifference = GcWaitBeforeCreatingNewBlockTimer.GetTime() - GcWaitBeforeCreatingNewBlockTimer.TellStartTime();

			// If we've waited long enough to create a new block
			if (ulWaitBeforeCreatingNewBlockTimeDifference > ulWAIT_TIME_BEFORE_CREATING_NEW_BLOCK)
			{
				// Remove any completed Horizontal lines and move other lines down and store the
				// number of lines removed in iNumber Of Lines Completed
				iNumberOfCompletedLines = GcCurrentBlock.mcBoard.ReturnAndEraseHorizontalLines();

				// Add the number of lines obtained to the total number of lines
				GuiTotalNumberOfCompletedLines += iNumberOfCompletedLines;

				// Increase the players Score depending on how many Lines were cleared with this Block
				switch (iNumberOfCompletedLines)
				{
					// If player cleared 1 Line
					case 1:
						GuiScore += iPOINTS_FOR_1_LINE;		// Increase total points
						GuiLineStats[0]++;					// Increment number of 1 liners achieved
						DSound_Play(GiClear1LineSound);		// Play 1 Line Cleared Sound
					break;

					// If player cleared 2 Lines
					case 2:
						GuiScore += iPOINTS_FOR_2_LINES;	// Increase total points
						GuiLineStats[1]++;					// Increment number of 1 liners achieved
						DSound_Play(GiClear2LinesSound);	// Play 2 Lines Cleared Sound
					break;

					// If player cleared 3 Lines
					case 3:
						GuiScore += iPOINTS_FOR_3_LINES;	// Increase total points
						GuiLineStats[2]++;					// Increment number of 1 liners achieved
						DSound_Play(GiClear3LinesSound);	// Play 3 Lines Cleared Sound
					break;

					// If player cleared 4 Lines
					case 4:
						GuiScore += iPOINTS_FOR_4_LINES;	// Increase total points
						GuiLineStats[3]++;					// Increment number of 1 liners achieved
						DSound_Play(GiClear4LinesSound);	// Play 4 Lines Cleared Sound
					break;

					// If player did not clear any Lines or something went wrong, do nothing
					default:
					break;
				}

				// If player has won the game
				if (GuiTotalNumberOfCompletedLines >= (iNUMBER_OF_LEVELS * iLINES_PER_LEVEL))
				{
					// Set the Game State to Win
					GeGameState = eGAME_STATE_WIN;

					// Reset that this function has already been ran once, in case the player decides to play again
					SbGamePlayStateHasBeenRanBefore = false;
				}

				// Keep track of Old Level to see if player advanced to next level or not
				uiOldLevel = GuiLevel;

				// Increase to next Level if enough lines are completed
				// The + 1 is to componsate for starting at Level 1, not 0, because int devision truncates
				GuiLevel = (GuiTotalNumberOfCompletedLines / iLINES_PER_LEVEL) + 1;

				// If player advanced a Level, make the blocks fall faster
				if (uiOldLevel < GuiLevel)
				{
					// Change the Game Colors the make Level Up more noticable
					Set_Palette(&BuildPalette());

					// Play the advance to New Level sound
					DSound_Play(GiNewLevelSound);

					// Check which Level player is on, and increase speed to that Levels speed
					switch (GuiLevel)
					{
						// If Player is on Level 1
						case 1:
							// Increase Block Fall speed to Level 1 speed
							SulBlockFallWaitTime = ulLEVEL_1_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel1Music;
						break;
						
						// If Player is on Level 2
						case 2:
							// Increase Block Fall speed to Level 2 speed
							SulBlockFallWaitTime = ulLEVEL_2_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel2Music;
						break;

						// If Player is on Level 3
						case 3:
							// Increase Block Fall speed to Level 3 speed
							SulBlockFallWaitTime = ulLEVEL_3_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel3Music;
						break;

						// If Player is on Level 4
						case 4:
							// Increase Block Fall speed to Level 4 speed
							SulBlockFallWaitTime = ulLEVEL_4_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel4Music;
						break;

						// If Player is on Level 5
						case 5:
							// Increase Block Fall speed to Level 5 speed
							SulBlockFallWaitTime = ulLEVEL_5_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel5Music;
						break;

						// If Player is on Level 6
						case 6:
							// Increase Block Fall speed to Level 6 speed
							SulBlockFallWaitTime = ulLEVEL_6_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel6Music;
						break;

						// If Player is on Level 7
						case 7:
							// Increase Block Fall speed to Level 7 speed
							SulBlockFallWaitTime = ulLEVEL_7_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel7Music;
						break;

						// If Player is on Level 8
						case 8:
							// Increase Block Fall speed to Level 8 speed
							SulBlockFallWaitTime = ulLEVEL_8_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel8Music;
						break;

						// If Player is on Level 9
						case 9:
							// Increase Block Fall speed to Level 9 speed
							SulBlockFallWaitTime = ulLEVEL_9_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel9Music;
						break;

						// If Player is on Level 10
						case 10:
							// Increase Block Fall speed to Level 10 speed
							SulBlockFallWaitTime = ulLEVEL_10_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel10Music;
						break;
						
						// If player won game, or something went wrong 
						default:
							// Set the Block Fall speed to Level 10 speed
							SulBlockFallWaitTime = ulLEVEL_10_BLOCK_SPEED;

							// Change Current Music to this levels music
							GiCurrentMusic = GiLevel10Music;
						break;
					};	

					// If Music is turned on
					if (GbMusicOn)
					{
						// Play this Level's Music
						DMusic_Play(GiCurrentMusic);
					}
				}

				// Try moving the new block to the starting block position
				if (!GcCurrentBlock.MoveEntireBlockToCellAndLeaveImageIfAvailable(GcCurrentBlock.ReturnBlockStartingCellRow(), GcCurrentBlock.ReturnBlockStartingCellColumn()))
				{
					// Since the Move Block And Leave Image function 
					// failed we must set the starting position manually
					GcCurrentBlock.SetBlockCellRow(GcCurrentBlock.ReturnBlockStartingCellRow());
					GcCurrentBlock.SetBlockCellColumn(GcCurrentBlock.ReturnBlockStartingCellColumn());
					
					// If not possible to move block to starting position, the position must be
					// occupied already by other blocks, so game over
					GeGameState = eGAME_STATE_GAME_OVER;

					// Reset that this has already ran once in case player decides to play again
					SbGamePlayStateHasBeenRanBefore = false;
				}
				
				// If the Move Block And Leave Image funciton failed we
				// must manually leave the image behind by not erasing it
				if (GeGameState != eGAME_STATE_GAME_OVER)
				{
					// Erase the Current Block from the Board
					GcCurrentBlock.EraseEntireBlockFromBoard();
				}

				// Erase Next Block from it's Board whether the Game is Over or not
				GcNextBlock.EraseEntireBlockFromBoard();
				
				// Create set Current Block to the Next Block and make Next Block a new random Block
				GcCurrentBlock.ChangeBlockType(GcNextBlock.ReturnBlockType());
				GcNextBlock.ChangeBlockType();

				// Place our new Blocks on the Board
				GcCurrentBlock.PlaceEntireBlockOnBoard();
				GcNextBlock.PlaceEntireBlockOnBoard();

				// Increment number of Generated blocks
				GuiBlocksGenerated++;
				
				// Play the Sound of Creating a New Block
				DSound_Play(GiCreateNewBlockSound);

				// Since we reset the block it should not be hitting 
				// anything yet, unless it is Game Over
				SbBlockHasHitSomething = false;
			}
		}

		// If playing in Extreme Mode
		if (GbExtremeMode)
		{
			// If it is time to randomize the block
			if ((GcWaitBeforeRandomizingBlockTimer.GetTime() - GcWaitBeforeRandomizingBlockTimer.TellStartTime()) > SulBlockRandomizeDuration)
			{
				// Calculate random time until next block change
				SulBlockRandomizeDuration = (rand() % (ulMAX_WAIT_TIME_BEFORE_RANDOMIZING_BLOCK - ulMIN_WAIT_TIME_BEFORE_RANDOMIZING_BLOCK)) + ulMIN_WAIT_TIME_BEFORE_RANDOMIZING_BLOCK;

				// Reset timer
				GcWaitBeforeRandomizingBlockTimer.Start();

				// Transform the current block
				if (!GcCurrentBlock.ChangeBlockTypeAndPutOnBoardIfValid())
				{
					// If we can't change the Current block, change the Next block instead
					GcNextBlock.ChangeBlockTypeAndPutOnBoardIfValid();
				}

				// Increase player Score for Randomization
				GuiScore += iBASE_POINTS_FOR_RANDOMIZE * GuiLevel;

				// Increase number or Randomizes
				GuiTotalRandomizes++;

				// Stop Randomize Warning sound and play Randomize Block sound
				DSound_Stop_Sound(GiRandomizeWarningSound);
				DSound_Play(GiRandomizeBlockSound);

				// Set warning to not have played yet
				SbRandomizeWarningHasPlayed = false;
			}
			
			// Calculate time before next randomize
			SulTimeBeforeNextBlockRandomize = SulBlockRandomizeDuration - (GcWaitBeforeRandomizingBlockTimer.GetTime() - GcWaitBeforeRandomizingBlockTimer.TellStartTime());

			// If the Randomize Warning sound should play
			if (SulTimeBeforeNextBlockRandomize < 2000 && !SbRandomizeWarningHasPlayed)
			{
				// Play the warning sound
				DSound_Play(GiRandomizeWarningSound);

				// Mark that the sound has played
				SbRandomizeWarningHasPlayed = true;
			}
		}
	}

	// Fill the Score, Level, Completed Lines, and Line Stats buffers in with the correct info
	sprintf(cScore, "Score = %d", GuiScore);
	sprintf(cLevel, "Level %d", GuiLevel);
	sprintf(cTotalNumberOfCompletedLines, "Lines Completed - %d", GuiTotalNumberOfCompletedLines);
	sprintf(c1Liners, "1 Liners - %d x %d", iPOINTS_FOR_1_LINE, GuiLineStats[0]);
	sprintf(c2Liners, "2 Liners - %d x %d", iPOINTS_FOR_2_LINES, GuiLineStats[1]);
	sprintf(c3Liners, "3 Liners - %d x %d", iPOINTS_FOR_3_LINES, GuiLineStats[2]);
	sprintf(c4Liners, "Connexs - %d x %d", iPOINTS_FOR_4_LINES, GuiLineStats[3]);
	sprintf(cBlocksGenerated, "Blocks Generated - %d", GuiBlocksGenerated);

	// Clear out the back buffer
	DDraw_Fill_Surface(lpddsback, iPLAY_BACKGROUND_COLOR);

	// Display the Game's Name
	Draw_Text_GDI(GcGameName, 285, 20, iPLAY_TEXT_COLOR, lpddsback);

	// Draw the Level, Score, and "Next Block" words to the back buffer
	Draw_Text_GDI(cLevel, 350, 50, iPLAY_TEXT_COLOR, lpddsback);
	Draw_Text_GDI(cScore, 100, 80, iPLAY_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("Next Block", iNEXT_BLOCK_POSITION_X, (iNEXT_BLOCK_POSITION_Y - 20), iPLAY_TEXT_COLOR, lpddsback);

	// Display Other Stats
	Draw_Text_GDI("Stats", 110, 150, iPLAY_TEXT_COLOR, lpddsback);
	Draw_Text_GDI(c1Liners, 80, 170, iPLAY_TEXT_COLOR, lpddsback);
	Draw_Text_GDI(c2Liners, 80, 190, iPLAY_TEXT_COLOR, lpddsback);
	Draw_Text_GDI(c3Liners, 80, 210, iPLAY_TEXT_COLOR, lpddsback);
	Draw_Text_GDI(c4Liners, 80, 230, iPLAY_TEXT_COLOR, lpddsback);
	Draw_Text_GDI(cBlocksGenerated, 80, 280, iPLAY_TEXT_COLOR, lpddsback);
	Draw_Text_GDI(cTotalNumberOfCompletedLines, 80, 300, iPLAY_TEXT_COLOR, lpddsback);

	// Display Toggle Options
	Draw_Text_GDI("Options", 600, 80, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("G - Toggle Board Grid: ", 500, 100, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("B - Toggle Board Border: ", 500, 120, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("T - Toggle Board Transparency: ", 500, 140, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("M - Toggle Music: ", 500, 170, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("C - Change game Colors", 500, 190, iOPTIONS_TEXT_COLOR, lpddsback);
	
	// Display the state of the Game Options to the screen

	// If the Board Grid is On
	if (GcCurrentBlock.mcBoard.GridIsOn())
	{
		// Display that the Grid is On
		Draw_Text_GDI("On", 725, 100, iOPTIONS_TEXT_COLOR, lpddsback);
	}else
	{
		// Display that the Grid is Off
		Draw_Text_GDI("Off", 725, 100, iOPTIONS_TEXT_COLOR, lpddsback);
	}

	// If the Board Border is On
	if (GcCurrentBlock.mcBoard.BorderIsOn())
	{
		// Display that the Border is On
		Draw_Text_GDI("On", 725, 120, iOPTIONS_TEXT_COLOR, lpddsback);
	}else
	{
		// Display that the Border is Off
		Draw_Text_GDI("Off", 725, 120, iOPTIONS_TEXT_COLOR, lpddsback);
	}

	// If the Board Transparency is On
	if (GcCurrentBlock.mcBoard.ReturnBoardStyle() == eBOARD_STYLE_TRANSPARENT)
	{
		// Display that the Board Transparency is On
		Draw_Text_GDI("On", 725, 140, iOPTIONS_TEXT_COLOR, lpddsback);
	}else
	{
		// Display that the Board Transparency is Off
		Draw_Text_GDI("Off", 725, 140, iOPTIONS_TEXT_COLOR, lpddsback);
	}

	// If the Music is On
	if (GbMusicOn)
	{
		// Display that the Music is On
		Draw_Text_GDI("On", 725, 170, iOPTIONS_TEXT_COLOR, lpddsback);
	}
	else
	{
		// Display that the Music is Off
		Draw_Text_GDI("Off", 725, 170, iOPTIONS_TEXT_COLOR, lpddsback);
	}

	
	// If playing in Extreme Mode
	if (GbExtremeMode)
	{
		// Store Randomize Time Remaining in a string for display
		sprintf(cRandomizeTimeRemaining, "%d - Time Before Next Randomize", (int)(SulTimeBeforeNextBlockRandomize / 1000) + 1);

		// Store number of Randomizes in a string for display
		sprintf(cRandomizes, "Randomizes - %d", GuiTotalRandomizes);

		// Display that game is in Extreme mode
		Draw_Text_GDI("Extreme Mode", 500, 350, iOPTIONS_TEXT_COLOR, lpddsback);

		// Display time before next Block Randomize
		Draw_Text_GDI(cRandomizeTimeRemaining, 500, 370, iPLAY_TEXT_COLOR, lpddsback);

		// Display number of Randomizes so far
		Draw_Text_GDI(cRandomizes, 80, 320, iPLAY_TEXT_COLOR, lpddsback);
	}


	// If game is Paused
	if (GbPaused)
	{
		// Display that game is paused
		Draw_Text_GDI("Paused", 350, 550, iOPTIONS_TEXT_COLOR, lpddsback);
	}

	// Tell player which key to press to get to Pause the game
	Draw_Text_GDI("Press the Space Bar to Pause the game", 245, 520, iPLAY_TEXT_COLOR, lpddsback);

	// Draw all the blocks to the screen
	GcCurrentBlock.DrawAllBlocks();
	GcNextBlock.DrawAllBlocks();

	// Display the Frames Per Second
	Fps();

	// Flip the front and back surfaces to display picture
	DDraw_Flip();

	// Make sure Music does not stop
	LoopMusic();
};

//** Game Over *******************************************************

void GameStateGameOver()
{
	static bool SbFirstTimeRunningGameOver	= true;	// Holds if the Game Over Sound has played yet or not
	static CHighScores ScHighScores;				// Holds the High Scores
	static bool SbEnteringName				= false;// Holds if the Player is entering their name or not
	static char ScName[iMAX_NAME_LENGTH + 1];		// Holds the Players Name they enter, and used to store Names in so they can be displayed
	Score iScore							= 0;	// Holds a High Score Scores
	int iLevel								= 0;	// Holds a Level
	int iMode								= 0;	// Holds the Mode the High Score was achieved in
	PosInt iHighScoreIndex					= 0;	// Looping variable
	
	// Display string buffers
	char cScore[12];
	char cPosition[3];
	char cLevel[15];
	char cMode[20];

	// Holds Positions where Text should appear on the screen
	const int iHighScoresTextPositionY		= 135;
	int iTextPositionY						= 0;
	
	unsigned long ulKeyPressTimeDifference	= 0;	// Holds the time different of our Key Press Timer

	// If the Game Over Sound has not been played
	if (SbFirstTimeRunningGameOver)
	{
		// Play the Game Over Sound
		DSound_Play(GiGameOverSound);

		// Load the previous High Scores
		ScHighScores.Load();

		// If the Player made it into the High Score List
		if (GuiScore > ScHighScores.ReturnLastPlaceScore())
		{
			// Let us know the Player needs to enter their name
			SbEnteringName = true;
		}

		// Place End Line character at the end of cName
		ScName[iMAX_NAME_LENGTH] = '\n';

		// Keep track that the sound has played
		SbFirstTimeRunningGameOver = false;
	}

	// Read the keyboard input
	DInput_Read_Keyboard();

	// If player is pressing Escape
	if (DirectInputKeyDown(DIK_ESCAPE))
	{
		// Quit the game
		SendMessage(hMainWindowHandle, WM_DESTROY,0,0);
	}

	// Holds how long our Key Press Timer has been running for without being restarted
	ulKeyPressTimeDifference  = GcOptionsKeyPressTimer.GetTime() - GcOptionsKeyPressTimer.TellStartTime();

	// If it is time to get the Players input for their Name
	if (ulKeyPressTimeDifference > ulWAIT_TIME_BEFORE_NEW_NAME_INPUT_VALID && SbEnteringName)
	{
		// Concatinate players input to end of cName
		ConcatinateKeyboardInputToString(ScName, iMAX_NAME_LENGTH);

		// If player presses "Enter"
		if (DirectInputKeyDown(DIK_RETURN))
		{
			// If player cheated
			if (GbCheated)
			{
				strcpy(ScName, "Cheater");
			}

			// Insert the name into our High Scores List
			ScHighScores.Insert(ScName, GuiScore, GuiLevel, (int)GbExtremeMode);

			// Save our new High Scores List
			ScHighScores.Save();

			// Let us know player is done entering in their Name
			SbEnteringName = false;
		}

		// Restart our Key Press Timer
		GcOptionsKeyPressTimer.Start();
	}
	
	// Clear out the back buffer
	DDraw_Fill_Surface(lpddsback, iBLACK_COLOR);

	// If Player is still entering their name
	if (SbEnteringName)
	{
		// Display "Enter Your Name: "
		Draw_Text_GDI("Enter Your Name: ", 200, 200, iOPTIONS_TEXT_COLOR, lpddsback);
	
		// Display the players input
		Draw_Text_GDI(ScName, 350, 200, iOPTIONS_TEXT_COLOR, lpddsback);
	}
	// If Player is not entering their name
	else
	{
		// Display "High Scores"
		Draw_Text_GDI("High Scores", 350, 75, iOPTIONS_TEXT_COLOR, lpddsback);

		Draw_Text_GDI("Rank", 135, 125, iOPTIONS_TEXT_COLOR, lpddsback);
		Draw_Text_GDI("Name", 180, 125, iOPTIONS_TEXT_COLOR, lpddsback);
		Draw_Text_GDI("Score", 425, 125, iOPTIONS_TEXT_COLOR, lpddsback);
		Draw_Text_GDI("Level", 500, 125, iOPTIONS_TEXT_COLOR, lpddsback);
		Draw_Text_GDI("Mode", 580, 125, iOPTIONS_TEXT_COLOR, lpddsback);
		
		// Display the High Scores
		for (iHighScoreIndex = 1; iHighScoreIndex <= iMAX_NUMBER_OF_HIGH_SCORES; iHighScoreIndex++)
		{
			// Store this High Scores Score in iScore
			iScore = ScHighScores.ReturnScore(iHighScoreIndex);
			iLevel = ScHighScores.ReturnExtraInfo(iHighScoreIndex);
			iMode = ScHighScores.ReturnExtraInfo2(iHighScoreIndex);
			
			// If there is a Score in this High Score Position
			// Also stores the High Scores Name in cName
			if ((ScHighScores.StoreName(iHighScoreIndex, ScName)) && (iScore > 0))
			{
				// Calculate the Y position to display this High Score
				iTextPositionY = (iHighScoresTextPositionY + (iHighScoreIndex * 25));
				
				// Store the int's in char*'s so we can display them
				itoa(iHighScoreIndex, cPosition, 10);
				itoa(iScore, cScore, 10);
				itoa(iLevel, cLevel, 10);

				// If the Player beat the game
				if (iLevel > iNUMBER_OF_LEVELS)
				{
					strcpy(cLevel, "Completed");
				}

				// If Mode was Normal
				if (iMode == 0)
				{
					strcpy(cMode, "Normal");
				}
				// Else Mode was Extreme
				else
				{
					strcpy(cMode, "Extreme");
				}
				

				// Display High Score info
				Draw_Text_GDI(cPosition, 145, iTextPositionY, iBACKGROUND_COLOR, lpddsback);
				Draw_Text_GDI(ScName, 180, iTextPositionY, iBACKGROUND_COLOR, lpddsback);
				Draw_Text_GDI(cScore, 425, iTextPositionY, iBACKGROUND_COLOR, lpddsback);
				Draw_Text_GDI(cLevel, 500, iTextPositionY, iBACKGROUND_COLOR, lpddsback);
				Draw_Text_GDI(cMode, 575, iTextPositionY, iBACKGROUND_COLOR, lpddsback);
			}
		}

		// If player cheated
		if (GbCheated)
		{
			Draw_Text_GDI("Cheaters remain anonymous SUCKA", 270, 450, iOPTIONS_TEXT_COLOR, lpddsback);
		}

		// Show player how to continue playing
		Draw_Text_GDI("Press the Space Bar to play again", 280, 500, iOPTIONS_TEXT_COLOR, lpddsback);

		// Show player how to exit game
		Draw_Text_GDI("Press Escape To Exit", 325, 525, iOPTIONS_TEXT_COLOR, lpddsback);

		// If the player presses the Space Bar
		if (DirectInputKeyDown(DIK_SPACE) && 
		(GcOptionsKeyPressTimer.GetTime() - GcOptionsKeyPressTimer.TellStartTime()) > ulWAIT_TIME_BEFORE_OPTIONS_INPUT)
		{
			// Switch the Game State back to Introduction
			GeGameState = eGAME_STATE_INTRODUCTION;

			// Reset to make sure initialization stuff is ran next time
			SbFirstTimeRunningGameOver = true;

			// Restart the input timer
			GcOptionsKeyPressTimer.Start();
		}
	}

	// Display the Frames Per Second
	Fps();

	// Flip the back and primary surface
	DDraw_Flip();

	// Make sure Music does not stop
	LoopMusic();
};

//** Game Introduction ***********************************************

void GameStateIntroduction()
{
	// Tells if this function has been ran yet or not
	static int SiFirstTimeRunning = true;

	// If this is the first time the function has ran
	if (SiFirstTimeRunning)
	{
		// If Music is turned on
		if (GbMusicOn)
		{
			// Start playing the Level 1 music
			DMusic_Play(GiLevel1Music);
		}

		// Record that this function has ran once
		SiFirstTimeRunning = false;
	}

	// Read the keyboard input
	DInput_Read_Keyboard();

	// If the player presses Escape
	if (DirectInputKeyDown(DIK_ESCAPE))
	{
		// Quit the game
		SendMessage(hMainWindowHandle, WM_DESTROY, 0, 0);
	}

	// If the player wants to start the game
	if ((DirectInputKeyDown(DIK_SPACE) || DirectInputKeyDown(DIK_E)) && 
		((GcOptionsKeyPressTimer.GetTime() - GcOptionsKeyPressTimer.TellStartTime()) > ulWAIT_TIME_BEFORE_OPTIONS_INPUT))
	{
		// Assume not playing extreme mode
		GbExtremeMode = false;

		// If player selected Extreme game
		if (DirectInputKeyDown(DIK_E))
		{
			GbExtremeMode = true;
		}

		// Set FirstTimeRunning to false so initial setup is ran next time we enter this function
		SiFirstTimeRunning = true;

		// Switch the Game State to Play
		GeGameState = eGAME_STATE_PLAY;

		// Restart the input timer
		GcOptionsKeyPressTimer.Start();
	}

	// If user is pressing M
	if (DirectInputKeyDown(DIK_M) && 
		((GcOptionsKeyPressTimer.GetTime() - GcOptionsKeyPressTimer.TellStartTime()) > ulWAIT_TIME_BEFORE_OPTIONS_INPUT))
	{
		// Toggle the music on/off
		if (GbMusicOn)
		{
			// Turn music off
			DMusic_Stop(GiCurrentMusic);
			GbMusicOn = false;
		}
		else
		{
			// Turn music on
			DMusic_Play(GiCurrentMusic);
			GbMusicOn = true;
		}

		// Restart the input timer
		GcOptionsKeyPressTimer.Start();
	}

	// Erase the back buffer
	DDraw_Fill_Surface(lpddsback, iBACKGROUND_COLOR);

	// Draw our Game Name
	Draw_Text_GDI(GcGameName, 300, 50, iOPTIONS_TEXT_COLOR, lpddsback);

	// Display Game Controls to the screen
	Draw_Text_GDI("Controls: ", 100, 130, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("Left / Right - Move Block Left / Right", 200, 150, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("Down - Make Block Fall Faster", 200, 170, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("Up / Z - Rotate Block Clockwise", 200, 190, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("A - Rotate Block Counter-Clockwise", 200, 210, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("Space Bar - Pause Game", 200, 250, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("C - Change The Game Colors", 200, 270, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("M - Toggle Music On / Off", 200, 290, iOPTIONS_TEXT_COLOR, lpddsback);

	// Add a little humour into our game
	Draw_Text_GDI("The game you could swear you've played before", 250, 75, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("(Please Note: This is a new release from Schroeder Enterprises. You have not played it before)", 100, 90, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("(Please Note: This game has been known to cause nostalgia in older players. Play at your own emotional risk)", 30, 550, iOPTIONS_TEXT_COLOR, lpddsback);
	
	// Display how to start the game
	Draw_Text_GDI("Press E to start an Extreme Game", 275, 400, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("Press the Space Bar to start a Normal Game", 245, 425, iOPTIONS_TEXT_COLOR, lpddsback);

	// Make sure Music does not stop
	LoopMusic();

	// Display the Frames Per Second
	Fps();

	// Flip the back and primary surface
	DDraw_Flip();
}

//** Game Won ********************************************************

void GameStateWin()
{
	// Keeps track of if the Game Won Sound has played or not
	static bool SbGameWonSoundHasPlayed = false;

	// Read the keyboard input
	DInput_Read_Keyboard();

	// If the player presses Escape
	if (DirectInputKeyDown(DIK_ESCAPE))
	{
		// Quit the game
		SendMessage(hMainWindowHandle, WM_DESTROY, 0, 0);
	}

	// If player presses Space Bar
	if (DirectInputKeyDown(DIK_SPACE) && 
		(GcOptionsKeyPressTimer.GetTime() - GcOptionsKeyPressTimer.TellStartTime()) > ulWAIT_TIME_BEFORE_OPTIONS_INPUT)
	{
		// Switch the Game State to Game Over
		GeGameState = eGAME_STATE_GAME_OVER;

		// Mark that the sound has not been played for next time
		SbGameWonSoundHasPlayed = false;

		// Restart the input timer
		GcOptionsKeyPressTimer.Start();
	}

	// If the Game Won Sound has not been played
	if (!SbGameWonSoundHasPlayed)
	{
		// Play the Game Won Sound
		DSound_Play(GiGameWonSound);

		// Keep track that the sound has been played
		SbGameWonSoundHasPlayed = true;
	}

	// Erase the back buffer
	DDraw_Fill_Surface(lpddsback, iBACKGROUND_COLOR);
	
	// Display Text that player won
	Draw_Text_GDI("YOU WON!! YOU RULE!!!", 300, 200, iOPTIONS_TEXT_COLOR , lpddsback);
	Draw_Text_GDI("What did you expect? A girl in a bikini presenting you an award?", 180, 400, iOPTIONS_TEXT_COLOR, lpddsback);
	Draw_Text_GDI("Press Space Bar to continue", 290, 500, iOPTIONS_TEXT_COLOR, lpddsback);

	// Make sure Music does not stop
	LoopMusic();

	// Display the Frames Per Second
	Fps();

	// Flip the back and primary surface
	DDraw_Flip();
};

//** Other Functions *************************************************

bool ProcessMovementInput(bool& bBottomHitWhilePressingDown)
// Processes any movement input
// Returns true if there was input to process, false if not
{
	bool bThereWasInputToProcess = false;	// Holds if there was input to process or not

	// Test if user is pressing a directional key
	// If user is pressing left arrow key
	if (DirectInputKeyDown(DIK_LEFT))
	{
		// Move the Current Block one cell left
		GcCurrentBlock.MoveBlockLeft();

		// Show that there was input to process	
		bThereWasInputToProcess = true;
	}
	// If user is pressing right arrow key
	else if (DirectInputKeyDown(DIK_RIGHT))
	{
		// Move the Current Block one cell right
		GcCurrentBlock.MoveBlockRight();

		// Show that there was input to process	
		bThereWasInputToProcess = true;
	}
	
	// If user is pressing down arrow key
	if (DirectInputKeyDown(DIK_DOWN))
	{
		// Move the Current Block two cells down, and save if it has hit something below it or not
		GcCurrentBlock.MoveBlockDown();
		bBottomHitWhilePressingDown = !GcCurrentBlock.MoveBlockDown();

		// Show that there was input to process
		bThereWasInputToProcess = true;	
	}

	return bThereWasInputToProcess;		// Return if there was input to process or not
}

// Process input associated with Rotating the Block
bool ProcessRotationInput()
{
	bool bThereWasInputToProcess = false;	// Holds if there was input to process or not

	// Test if user is trying to rotate the Current Block
	// If user is pressing 'a'
	if (DirectInputKeyDown(DIK_A))
	{
		// Rotate the Current Block counter clockwise 90 degrees
		GcCurrentBlock.RotateBlockCounterClockwise();

		// Show that there was input to process
		bThereWasInputToProcess = true;	
	}
	// If user is pressing 'z' or the up arrow key
	else if (DirectInputKeyDown(DIK_Z) || DirectInputKeyDown(DIK_UP))
	{
		// Rotate the Current Block clockwise 90 degrees
		GcCurrentBlock.RotateBlockClockwise();

		// Show that there was input to process
		bThereWasInputToProcess = true;	
	}

	return bThereWasInputToProcess;		// Return if there was input to process or not
}

// Processes all Options input
bool ProcessOptionsInput()
{
	bool bThereWasInputToProcess = false;	// Holds if there was input to process or not

	// If user is pressing 'G'
	if (DirectInputKeyDown(DIK_G))
	{
		// Toggle the Board Grid
		if (GcCurrentBlock.mcBoard.GridIsOn())
		{
			GcCurrentBlock.mcBoard.TurnGridOff();
		}
		else
		{
			GcCurrentBlock.mcBoard.TurnGridOn();

			// Board must be Transparent to see Grid
			GcCurrentBlock.mcBoard.MakeBoardStyleTransparent();
		}

		// Show that there was input to process
		bThereWasInputToProcess = true;
	}

	// If user is pressing 'B'
	if (DirectInputKeyDown(DIK_B))
	{
		// Toggle the Board Border
		if (GcCurrentBlock.mcBoard.BorderIsOn())
		{
			GcCurrentBlock.mcBoard.TurnBorderOff();
		}
		else
		{
			GcCurrentBlock.mcBoard.TurnBorderOn();

			// Board must be Transparent to see Border
			GcCurrentBlock.mcBoard.MakeBoardStyleTransparent();
		}

		// Show that there was input to process
		bThereWasInputToProcess = true;
	}

	// If user is pressing 'T'
	if (DirectInputKeyDown(DIK_T))
	{
		// Toggle the Board Style
		if (GcCurrentBlock.mcBoard.ReturnBoardStyle() == eBOARD_STYLE_TRANSPARENT)
		{
			GcCurrentBlock.mcBoard.MakeBoardStyleSolid();
		}
		else
		{
			GcCurrentBlock.mcBoard.MakeBoardStyleTransparent();
		}
		
		// Show that there was input to process
		bThereWasInputToProcess = true;
	}

	// If user is pressing M
	if (DirectInputKeyDown(DIK_M))
	{
		// Toggle the music on/off
		if (GbMusicOn)
		{
			// Turn music off
			DMusic_Stop(GiCurrentMusic);
			GbMusicOn = false;
		}
		else
		{
			// Turn music on
			DMusic_Play(GiCurrentMusic);
			GbMusicOn = true;
		}

		// Show that there was input to process
		bThereWasInputToProcess = true;
	}

	// If user is pressing 'C'
	if (DirectInputKeyDown(DIK_C))
	{
		// Change the Game's Colors
		Set_Palette(&BuildPalette());

		// Play the Confirm Color Change sound
		DSound_Play(GiConfirmColorChangeSound);

		// Show that there was input to process
		bThereWasInputToProcess = true;
	}

	// If user is pressing the Space Bar, then go to menu
	if (DirectInputKeyDown(DIK_SPACE))
	{
		// Play the Pause Sound
		DSound_Play(GiPauseSound);

		if (GbPaused)
		{
			// UnPause game Timers
			GcBlockFallTimer.Pause(false);
			GcWaitBeforeCreatingNewBlockTimer.Pause(false);
			GcWaitBeforeRandomizingBlockTimer.Pause(false);

			// Record that game is unpaused
			GbPaused = false;
		}
		else
		{
			// Pause game Timers
			GcBlockFallTimer.Pause(true);
			GcWaitBeforeCreatingNewBlockTimer.Pause(true);
			GcWaitBeforeRandomizingBlockTimer.Pause(true);

			// Record that game is paused
			GbPaused = true;
		}

		// Show that there was input to process
		bThereWasInputToProcess = true;
	}


	// Developer cheats

	// If pressing Q and K (Quick Clear)(C already in use)
	if (DirectInputKeyDown(DIK_Q) && DirectInputKeyDown(DIK_K))
	{
		// Clear entire board
		GcCurrentBlock.mcBoard.ClearOutBoard();

		// Show that there was input to process
		bThereWasInputToProcess = true;

		// Record that player cheated
		GbCheated = true;
	}

	// If pressing Q and P (Quick Points)
	if (DirectInputKeyDown(DIK_Q) && DirectInputKeyDown(DIK_P))
	{
		// Clear entire board
		GuiScore += 1000;

		// Show that there was input to process
		bThereWasInputToProcess = true;
		
		// Record that player cheated
		GbCheated = true;
	}

	// If pressing Q and L (Quick Level -> 10 Lines)
	if (DirectInputKeyDown(DIK_Q) && DirectInputKeyDown(DIK_L))
	{
		// Clear entire board
		GuiTotalNumberOfCompletedLines += 10;

		// Show that there was input to process
		bThereWasInputToProcess = true;

		// Record that player cheated
		GbCheated = true;
	}

	return bThereWasInputToProcess;		// Return if there was input to process or not
}


//////////////////////////////////////////////////////////////////////

PALETTEENTRY& BuildPalette()
// Builds the Games Palette and returns it
{
	PALETTEENTRY sPalette[iMAX_PALETTE_COLORS];	// Holds the Palette we build
	int iColorRange	= 50;						// The Color Range the Blocks must be away from the Board Color
	int iIndex		= 0;						// Looping variable


	// Build the Palette
	for (iIndex = 0; iIndex < iMAX_PALETTE_COLORS; iIndex++)
	{
		sPalette[iIndex].peRed		= rand() % iMAX_PALETTE_COLORS;
		sPalette[iIndex].peGreen	= rand() % iMAX_PALETTE_COLORS;
		sPalette[iIndex].peBlue		= rand() % iMAX_PALETTE_COLORS;
		sPalette[iIndex].peFlags	= PC_NOCOLLAPSE;
	}

	// Fill in our default Colors

	// Set Black Color
	sPalette[iBLACK_COLOR].peRed				= 0;
	sPalette[iBLACK_COLOR].peGreen				= 0;
	sPalette[iBLACK_COLOR].peBlue				= 0;
	sPalette[iBLACK_COLOR].peFlags				= PC_NOCOLLAPSE;

	// Set White Color
	sPalette[iWHITE_COLOR].peRed				= (iMAX_PALETTE_COLORS - 1);
	sPalette[iWHITE_COLOR].peGreen				= (iMAX_PALETTE_COLORS - 1);
	sPalette[iWHITE_COLOR].peBlue				= (iMAX_PALETTE_COLORS - 1);
	sPalette[iWHITE_COLOR].peFlags				= PC_NOCOLLAPSE;

	// Set the general Background Color
	sPalette[iBACKGROUND_COLOR].peRed			= 0;
	sPalette[iBACKGROUND_COLOR].peGreen			= 0;
	sPalette[iBACKGROUND_COLOR].peBlue			= 220;
	sPalette[iBACKGROUND_COLOR].peFlags			= PC_NOCOLLAPSE;

	// Set the Options Text Color
	sPalette[iOPTIONS_TEXT_COLOR].peRed			= 0;
	sPalette[iOPTIONS_TEXT_COLOR].peGreen		= 250;
	sPalette[iOPTIONS_TEXT_COLOR].peBlue		= 0;
	sPalette[iOPTIONS_TEXT_COLOR].peFlags		= PC_NOCOLLAPSE;

	// Set the Board Background Color
	sPalette[iBOARD_COLOR].peRed				= iBLACK_COLOR;
	sPalette[iBOARD_COLOR].peGreen				= iBLACK_COLOR;
	sPalette[iBOARD_COLOR].peBlue				= iBLACK_COLOR;
	sPalette[iBOARD_COLOR].peFlags				= PC_NOCOLLAPSE;

	// Cycle through our Blocks
	for (iIndex = iI_COLOR; iIndex <= iSQUARE_COLOR; iIndex++)
	{
		// If any of the Blocks are close to the same color as the
		// Board, then change the Block color to a new random color
		while (sPalette[iIndex].peRed < iColorRange)
		{
			sPalette[iIndex].peRed = rand() % iMAX_PALETTE_COLORS;
		}

		while (sPalette[iIndex].peGreen	< iColorRange)
		{
			sPalette[iIndex].peGreen = rand() % iMAX_PALETTE_COLORS;
		}

		while (sPalette[iIndex].peBlue < iColorRange)
		{
			sPalette[iIndex].peBlue = rand() % iMAX_PALETTE_COLORS;
		}
	}

	return *sPalette;		// Return the Palette we just made
}

//////////////////////////////////////////////////////////////////////

void LoopMusic()
// Makes sure which ever song GiCurrentMusic is pointing to, is playing
{
	// If the song GiCurrentMusic is pointing to is not playing, and the Music is turned on
	if (DMusic_Status_MIDI(GiCurrentMusic) == MIDI_STOPPED && GbMusicOn)
	{
		// Play the song GiCurrentMusic is pointing to
		DMusic_Play(GiCurrentMusic);
	}
}

//////////////////////////////////////////////////////////////////////

void Fps()
// Displays the current Frames Per Second to the top left 
// corner of the screen
// Needs a GLOBAL Handle to the Main Window to work
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

	// Draw Frames Per Second to the back surface
	Draw_Text_GDI(Buffer, 8, 8, 25, lpddsback);
	ReleaseDC(hMainWindowHandle, hDeviceContext);
};

