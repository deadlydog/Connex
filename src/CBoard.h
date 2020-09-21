// CBoard.h

#ifndef CBOARD_H
#define CBOARD_H

const int iBOARD_HEIGHT				= 20;	// Board Height in Cells
const int iBOARD_WIDTH				= 10;	// Board Width in Cells

enum eBoardStyles
{
	eBOARD_STYLE_SOLID				= 0,	// Flag for board style to be solid
	eBOARD_STYLE_TRANSPARENT		= 1		// Flag for board style to be transparent
};

class CBoard
{
	public:

		// Constructor with default settings
		CBoard(int iCellHeightInPixels = 20, int iCellWidthInPixels = 20,
			   int iLeftXCoordinateOfBoard = 275, int iTopYCoordinateOfBoard = 75,
			   int iBoardColor = 0);
		~CBoard();
		
		bool SetCellDimensions(int iCellHeight, int iCellWidth);		// Sets the Height and Width of a cell in pixels
		int  ReturnCellHeight() const;			// Returns the Height of a Cell in pixels
		int  ReturnCellWidth() const;			// Returns the Width of a Cell in pixels

		bool SetBoardPosition(int iLeftXCoordinate, int iTopYCoordinate);
		int  ReturnBoardLeftXCoordinate() const;
		int  ReturnBoardTopYCoordinate() const;

		void SetBoardColor(int iBoardColor);// Sets the Board Color (Can be overriden by inherited classes)
		int  ReturnBoardColor() const;			// Returns the current Board Color

		void MakeBoardStyleTransparent();		// Sets the Board Style to Transparent
		void MakeBoardStyleSolid();				// Sets the Board Style to Solid
		int  ReturnBoardStyle() const;			// Returns the current Board Style

		void TurnGridOn();						// Turn the Board Grid on so it is displayed			
		void TurnGridOff();						// Turn the Board Grid off so it is not displayed
		bool GridIsOn() const;					// Returns true if the Grid is on
		
		void TurnBorderOn();					// Turns the Border On so it is displayed
		void TurnBorderOff();					// Turns the Border Off so it is not displayed
		bool BorderIsOn() const;				// Returns true if the Border is on

		void SetGridAndBorderColor(int iGridAndBorderColor);	// Set the Grid to the specified Color
		int  ReturnGridAndBorderColor() const;		// Return the Color of the Grid

		bool BoardCellIsEmpty(int iRow, int iColumn) const;				// Returns true if cell is not occupied
		void PlaceBlockInBoardCell(int iRow, int iColumn, int iColor);	// Fills one cell with the specified color

		int  ReturnAndEraseHorizontalLines();	// Returns the number of Horizontal lines that are filled and removes them from the Board(Every Cell in the row must be filled)

		bool SetCellColor(int iRow, int iColumn, int iColor);	// Sets the specified Cell to the specified Color
		int	 ReturnCellColor(int iRow, int iColumn) const;		// Returns the Color of the specified Cell

		void ClearOutBoard();					// Remove everything from the Board

		void DrawBoard() const;					// Draws the board and all the blocks within it

	private:
				
		int miCellHeight;			// A cells height in pixels
		int miCellWidth;			// A cells width in pixels

		int miLeftEdgeOfBoard;		// X Coordinate of the left edge of the board
		int miTopEdgeOfBoard;		// Y Coordinate of the top edge of the board

		int miBoardColor;			// Boards background color for empty cells

		eBoardStyles meBoardStyle;	// Holds the style the board should be drawn with
		
		bool mbShowGrid;			// Holds if a grid should be displayed on the board or not
		bool mbShowBorder;			// Holds if the boarder should be displayed or not
		int miGridAndBorderColor;	// Holds the color of the grid

		int miBoardCell[iBOARD_HEIGHT][iBOARD_WIDTH];	// Holds board data
};

#endif