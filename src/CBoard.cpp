// CBoard.cpp

#include "CBoard.h"
#include "Graphics Engine.h"	// Needed for the draw function
#include <fstream>
using namespace std;

CBoard::CBoard(int iCellHeightInPixels, int iCellWidthInPixels,
			   int iLeftXCoordinateOfBoard, int iTopYCoordinateOfBoard,
			   int iBoardBackgroundColor)
// Class Constructor
// Sets all the private data and fills the board with the
// specified "empty" color
{
	// Set the private data to the specified values
	SetCellDimensions(iCellHeightInPixels, iCellWidthInPixels);
	SetBoardPosition(iLeftXCoordinateOfBoard, iTopYCoordinateOfBoard);
	SetBoardColor(iBoardBackgroundColor);
	SetGridAndBorderColor(iBoardBackgroundColor);
	
	// Set these private data members with default settings
	MakeBoardStyleSolid();
	TurnGridOff();
	TurnBorderOff();

	// Set the Board up
	ClearOutBoard();
}

CBoard::~CBoard()
// Class Destructor
{}

bool CBoard::BoardCellIsEmpty(int iRow, int iColumn) const
// Tells if the specified color is empty or not
// Returns true if the cell is empty, false if not
{
	// True if the specified row and column are valid on the board
	bool bRowIsValid	= (iRow < iBOARD_HEIGHT && iRow >= 0);
	bool bColumnIsValid = (iColumn < iBOARD_WIDTH && iColumn >= 0);
	
	bool bCellIsEmpty = false;	// Hold if Cell is empty or not

	// If the Cell is empty and the specified row and column are valid
	if (ReturnCellColor(iRow, iColumn) == miBoardColor && bRowIsValid && bColumnIsValid)
	{
		bCellIsEmpty = true;	// Tells that the cell is empty
	}

	return bCellIsEmpty;		// Returns if the cell is empty or not
}

void CBoard::PlaceBlockInBoardCell(int iRow, int iColumn, int iColor)
// Places the specified color into the specified cell
{
	// True if the specified row and column are valid on the board
	bool bRowIsValid	= (iRow < iBOARD_HEIGHT && iRow >= 0);
	bool bColumnIsValid = (iColumn < iBOARD_WIDTH && iColumn >= 0);

	// If specified Color is a negative number, make it positive
	if (iColor < 0)
	{
		iColor = -iColor;
	}

	// Make sure the specified Color is in our Color range
	iColor = iColor % MAX_COLORS_PALETTE;

	// If the specified row and column are valid on the board
	if (bRowIsValid && bColumnIsValid)
	{
		// Fill the specified cell with the specified color
		SetCellColor(iRow, iColumn, iColor);
	}
}

bool CBoard::SetCellDimensions(int iCellHeight, int iCellWidth)
// Sets the Cell Height and Cell Width to the specified height and width
// NOTE: There is no maximum error checking as it is dependant on the screen resolution
// Returns true if dimensions are valid and were changed, false if not
{
	// Holds if the specified dimensions are valid or not
	bool bValidDimensions = (iCellHeight > 0 && iCellWidth > 0);

	// If the dimensions are valid
	if (bValidDimensions)
	{
		// Set the cell dimensions to the specified dimensions
		miCellHeight	= iCellHeight;
		miCellWidth		= iCellWidth;
	}

	return bValidDimensions;	// Return if the Cell Dimensions were changed or not
}

int CBoard::ReturnCellHeight() const
// Returns the Height of a cell in pixels
{
	return miCellHeight;
}

int CBoard::ReturnCellWidth() const
// Returns the Width of a cell in pixels
{
	return miCellWidth;
}

bool CBoard::SetBoardPosition(int iLeftXCoordinate, int iTopYCoordinate)
// Sets the boards top left x, y coordinates to the specified coordinates
// NOTE: There is no maximum error checking as it is dependant on the screen resolution
// Returns True if successful, false if not
{
	// Holds if the specified Position is valid or not
	bool bPositionIsValid = (iLeftXCoordinate > 0 && iTopYCoordinate > 0);

	// If the specified Position is valid
	if (bPositionIsValid)
	{
		// Set the Boards Position to the specified position
		miLeftEdgeOfBoard	= iLeftXCoordinate;
		miTopEdgeOfBoard	= iTopYCoordinate;
	}

	return bPositionIsValid;	// Return if the move was successful
}

void CBoard::SetBoardColor(int iBoardColor)
// Set the Boards background Color to the specified color
{
	// If the specified Color is a negative number
	if (iBoardColor < 0)
	{
		// Make the specified Color a positive number
		iBoardColor = -iBoardColor;
	}

	// Make sure the specified color is in the valid number range
	iBoardColor = iBoardColor % MAX_COLORS_PALETTE;

	// Set the Boards Color to the specified color
	miBoardColor = iBoardColor;
}

int CBoard::ReturnBoardColor() const
// Returns the boards default "empty" color
{
	return miBoardColor;	// Return the "empty" color
}

void CBoard::MakeBoardStyleSolid()
// Sets the Board Style to Solid
{
	meBoardStyle = eBOARD_STYLE_SOLID;			// Set the Board Style to Solic
}

void CBoard::MakeBoardStyleTransparent()
// Sets the Board Style to Transparent
{
	meBoardStyle = eBOARD_STYLE_TRANSPARENT;	// Set the Board Style to Transparent
}

int CBoard::ReturnBoardStyle() const
// Returns an integer identifying the currint Board Style
{
	return meBoardStyle;		// Return the Board Style
}

void CBoard::TurnGridOn()
// Turn the Board Grid On so that it is displayed
{
	mbShowGrid = true;
}

void CBoard::TurnGridOff()
// Turn the Board Grid Off so that it is displayed
{
	mbShowGrid = false;
}

bool CBoard::GridIsOn() const
// Returns if the Board Grid is on or not
// Returns true if grid is on, false if not
{
	return mbShowGrid;
}

void CBoard::SetGridAndBorderColor(int iGridAndBorderColor)
// Set the Grid Color to the specified Color
// Returns true if successful, false if not
{
	// If specified Color is not a negative number
	if (iGridAndBorderColor < 0)
	{
		// Make the specified Color a positive number
		iGridAndBorderColor = -iGridAndBorderColor;
	}

	// Make sure specified color is in the correct number range
	iGridAndBorderColor = iGridAndBorderColor % MAX_COLORS_PALETTE;
	
	// Set the Grid Color to the specified color
	miGridAndBorderColor = iGridAndBorderColor;
}

int CBoard::ReturnGridAndBorderColor() const
// Returns the Color of the Grid
{
	return miGridAndBorderColor;
}

void CBoard::TurnBorderOn()
// Turns the Border On so it is displayed
{
	mbShowBorder = true;
}

void CBoard::TurnBorderOff()
// Turns the Border Off so it is not displayed
{
	mbShowBorder = false;
}

bool CBoard::BorderIsOn() const
// Returns true if the Border is on
{
	return mbShowBorder;
}

int CBoard::ReturnAndEraseHorizontalLines()
// Returns the number of Horizontal Lines on the Board (Every cell in the row filled) and
// remove the complete lines from the board, dropping the above rows down
{
	int iRow				= 0;	// Looping variable
	int iColumn				= 0;	// Looping variable
	int iDropLine			= 0;	// Looping variable
	int iTotalNumberOfLines = 0;	// Holds the total number of horizontal lines
	int iCellColor			= 0;	// Holds the Color of the current Cell
	bool bEmptyCellFound = false;	// Tells if the Horizontal line is complete or not

	// Cycle through Board and look for full horizonal lines
	// Cycle through Board Rows
	for (iRow = 0; iRow < iBOARD_HEIGHT; iRow++)
	{
		// Start this row off with No Empty Cell Found
		bEmptyCellFound = false;

		// Cycle through Board Columns
		for (iColumn = 0; iColumn < iBOARD_WIDTH; iColumn++)
		{
			// Set Cell Color to the color of this Cell
			iCellColor = ReturnCellColor(iRow, iColumn);

			// If this Cell is empty
			if (iCellColor == miBoardColor)
			{
				// Show this Horizontal line is not complete
				bEmptyCellFound = true;
				//break;
			}
		}

		// If the line was a complete Horizontal line, remove it and
		// move all the blocks above down one cell
		if (!bEmptyCellFound)
		{
			// Increase our completed Horizontal line counter
			iTotalNumberOfLines++;

			// Move all cells above this line down one row
			for (iDropLine = iRow; iDropLine > 0; iDropLine--)
			{
				// Move through each Cell column
				for (iColumn = 0; iColumn < iBOARD_WIDTH; iColumn++)
				{
					// Make this Row equal to the one above it
					SetCellColor(iDropLine, iColumn, ReturnCellColor((iDropLine - 1), iColumn));
				}
			}

			// Cycle through the top row of cells and set them to empty, since they've
			// been moved down a row and have no row above them to grab cells from
			for (iColumn = 0; iColumn < iBOARD_WIDTH; iColumn++)
			{
				// Set the cell to "empty"
				SetCellColor(0, iColumn, miBoardColor);
			}
		}
	}
	return iTotalNumberOfLines;		// Return the number of completed Horizontal lines removed
}

bool CBoard::SetCellColor(int iRow, int iColumn, int iColor)
{
	bool bSuccessful	= false;	// Holds if the Cell was Set or not

	// Hold if the specified Cell is valid
	bool bRowIsValid	= (iRow >= 0 && iRow < iBOARD_HEIGHT);
	bool bColumnIsValid	= (iColumn >= 0 && iColumn < iBOARD_WIDTH);

	// If the specified Color is a negative number make it positive
	if (iColor < 0)
	{
		iColor = -iColor;
	}

	// Make sure the specified Color is within the valid range
	iColor = iColor % MAX_COLORS_PALETTE;

	// If the specified Cell is valid
	if (bRowIsValid && bColumnIsValid)
	{
		miBoardCell[iRow][iColumn] = iColor;
		
		bSuccessful = true;
	}

	return bSuccessful;
}

int CBoard::ReturnCellColor(int iRow, int iColumn) const
// Returns the Color of the specified Cell
{
	return miBoardCell[iRow][iColumn];
}

void CBoard::ClearOutBoard()
// Clear everything out of the Board and set all 
// cells to the Board Color
{
	int iRow	= 0;		// Looping variable
	int iColumn = 0;		// Looping variable

	// Cycle through the board data
	for (iRow = 0; iRow < iBOARD_HEIGHT; iRow++)
	{
		for (iColumn = 0; iColumn < iBOARD_WIDTH; iColumn++)
		{
			// Set the data to the specified "empty" color
			SetCellColor(iRow, iColumn, miBoardColor);
		}
	}
}

void CBoard::DrawBoard() const
// Draw our board with all of the info in it
{
	int iRow				= 0;	// Looping variable
	int iColumn				= 0;	// Looping variable

	int iColor				= 0;	// Holds the color of the current cell
	int iLeftCoordinate		= 0;	// X coordinate of left edge of cell
	int iTopCoordinate		= 0;	// Y coordinate of top edge of cell
	int iRightCoordinate	= 0;	// X coordinate of right edge of cell
	int iBottomCoordinate	= 0;	// Y coordinate of bottom edge of cell

	// X or Y coordinates of each edge of the board
	int iLeftEdgeOfBoard	= miLeftEdgeOfBoard;
	int iTopEdgeOfBoard		= miTopEdgeOfBoard;
	int iRightEdgeOfBoard	= (miLeftEdgeOfBoard + (iBOARD_WIDTH * miCellWidth));
	int iBottomEdgeOfBoard	= (miTopEdgeOfBoard + (iBOARD_HEIGHT * miCellHeight));

	// If the Board Style is Solid
	if (ReturnBoardStyle() == eBOARD_STYLE_SOLID)
	{
		// Draw the board background box with it's color
		Draw_Rectangle(iLeftEdgeOfBoard, iTopEdgeOfBoard,
					   iRightEdgeOfBoard, iBottomEdgeOfBoard,
					   miBoardColor, lpddsback);
	}

	// If the Board Grid is on, draw it (Does not draw the bounding boarder)
	if (GridIsOn())
	{
		// Lock the back buffer so we can draw to it
		DDraw_Lock_Back_Surface();

		// Draw all of the Horizontal lines
		for (iRow = 1; iRow < iBOARD_HEIGHT; iRow++)
		{
			HLine(iLeftEdgeOfBoard, iRightEdgeOfBoard, (iTopEdgeOfBoard + (iRow * miCellHeight)),
				  ReturnGridAndBorderColor(), back_buffer, back_lpitch);
		}

		// Draw all of the Vertical lines
		for (iColumn = 1; iColumn < iBOARD_WIDTH; iColumn++)
		{
			VLine(iTopEdgeOfBoard, iBottomEdgeOfBoard, (iLeftEdgeOfBoard + (iColumn * miCellWidth)),
				  ReturnGridAndBorderColor(), back_buffer, back_lpitch);
		}

		// Unlock the back buffer now that we're done with it
		DDraw_Unlock_Back_Surface();
	}

	// If the Board Border is on, draw the bounding box
	if (BorderIsOn())
	{
		// Lock the back buffer so we can draw to it
		DDraw_Lock_Back_Surface();

		// Draw the Border around the Board
		// Draw the Vertical Lines
		VLine(iTopEdgeOfBoard, iBottomEdgeOfBoard, iLeftEdgeOfBoard,
			  miGridAndBorderColor, back_buffer, back_lpitch);
		VLine(iTopEdgeOfBoard, iBottomEdgeOfBoard, iRightEdgeOfBoard,
			  miGridAndBorderColor, back_buffer, back_lpitch);

		// Draw the Horizontal Lines
		HLine(iLeftEdgeOfBoard, iRightEdgeOfBoard, iTopEdgeOfBoard,
			  miGridAndBorderColor, back_buffer, back_lpitch);
		HLine(iLeftEdgeOfBoard, iRightEdgeOfBoard, iBottomEdgeOfBoard,
			  miGridAndBorderColor, back_buffer, back_lpitch);

		// Unlock the back buffer now that we're done with it
		DDraw_Unlock_Back_Surface();
	}

	// Cycle through every cell on the board and draw the ones that are occupied
	for (iRow = 0; iRow < iBOARD_HEIGHT; iRow++)
	{
		for (iColumn = 0; iColumn < iBOARD_WIDTH; iColumn++)
		{
			// Set iColor to the color of the cell
			iColor = ReturnCellColor(iRow, iColumn);
			
			// Only Draw this Cell if it is not "empty"
			if (iColor != miBoardColor)
			{
				// Find all the edge positions of this cell
				iLeftCoordinate		= iLeftEdgeOfBoard + (iColumn * miCellWidth) + 1;
				iRightCoordinate	= iLeftCoordinate + miCellWidth - 1;
				iTopCoordinate		= iTopEdgeOfBoard + (iRow * miCellHeight) + 1;
				iBottomCoordinate	= iTopCoordinate + miCellHeight - 1;

				// Draw the cell
				Draw_Rectangle(iLeftCoordinate, iTopCoordinate, 
							   iRightCoordinate, iBottomCoordinate,
							   iColor, lpddsback);	
			}
		}
	}
}