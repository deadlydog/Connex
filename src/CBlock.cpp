// CBlock.cpp

#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "CBlock.h"

using namespace std;

CBlock::CBlock(int iBlockType, int iBlockStartCellRow, int iBlockStartCellColumn)
// Class Constructor
// Loads all the possible blocks and their rotation positions 
// into miBlockData and sets private data 
{
	// Make random numbers possible
	srand((unsigned int)time(NULL));

	int iHeight = 0;	// Looping variable
	int iWidth	= 0;	// Looping variable

	// If user wants a random block chosen
	if (iBlockType == iRANDOM_BLOCK)
	{
		// Set the block type to a random block type
		iBlockType = rand() % iNUMBER_OF_BLOCK_TYPES;
	}

	// Make sure iBlockType is a # between 0 and 6
	iBlockType = iBlockType % iNUMBER_OF_BLOCK_TYPES;

	// Set all of the private data
	msCurrentBlock.iBlockType				= iBlockType;
	msCurrentBlock.iRotatePosition			= 0;
	msCurrentBlock.iBlockStartCellRow		= iBlockStartCellRow;
	msCurrentBlock.iBlockStartCellColumn	= iBlockStartCellColumn;
	msCurrentBlock.iBlockCellRow			= msCurrentBlock.iBlockStartCellRow;
	msCurrentBlock.iBlockCellColumn			= msCurrentBlock.iBlockStartCellColumn;

	// Load the blocks data into the Block Data array
	LoadBlocksIntoBlockData();

	// Load the correct block and rotate position into miCurrent Block
	for (iHeight = 0; iHeight < iBLOCK_HEIGHT; iHeight++)
	{
		for (iWidth = 0; iWidth < iBLOCK_WIDTH; iWidth++)
		{
			msCurrentBlock.iPicture[iHeight][iWidth] =
					miBlockData[msCurrentBlock.iBlockType][msCurrentBlock.iRotatePosition][iHeight][iWidth];
		}
	}
	
	// Place our freshly made block onto the board
	PlaceEntireBlockOnBoard();
}

CBlock::~CBlock()
// Class Destructor
{}

void CBlock::ChangeBlockType(int iBlockType)
// Will change miBlockType to the specified block type
{
	int iHeight = 0;	// Looping variable
	int iWidth	= 0;	// Looping variable
	
	// If user wants a random block chosen
	if (iBlockType == iRANDOM_BLOCK)
	{
		// Set the block type to a random block type
		// Use the "<< 4" for better block distribution because rand() isn't the best for distribution 
		iBlockType = (rand() % (iNUMBER_OF_BLOCK_TYPES << 4));
	}

	// Make sure iBlockType is a # between 0 and 6
	iBlockType = iBlockType % iNUMBER_OF_BLOCK_TYPES;

	// Keep track of Current Blocks type and rotate position
	msCurrentBlock.iBlockType = iBlockType;
	msCurrentBlock.iRotatePosition = 0;
	
	// Load the correct block and rotate position into CheckBlock
	for (iHeight = 0; iHeight < iBLOCK_HEIGHT; iHeight++)
	{
		for (iWidth = 0; iWidth < iBLOCK_WIDTH; iWidth++)
		{
			msCurrentBlock.iPicture[iHeight][iWidth] =
					miBlockData[msCurrentBlock.iBlockType][msCurrentBlock.iRotatePosition][iHeight][iWidth];
		}
	}
}

bool CBlock::ChangeBlockTypeAndPutOnBoardIfValid(int iBlockType)
// Will change miBlockType to the specified block type if it is valid on the Board
{
	int iHeight = 0;	// Looping variable
	int iWidth	= 0;	// Looping variable
	
	bool bChangeSuccessful = false;		// Holds if it is a valid move
	
	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// If user wants a random block chosen
	if (iBlockType == iRANDOM_BLOCK)
	{
		// Set the block type to a random block type
		// Use the "<< 4" for better block distribution because rand() isn't the best for distribution 
		iBlockType = (rand() % (iNUMBER_OF_BLOCK_TYPES << 4));
	}

	// Make sure iBlockType is a # between 0 and 6
	iBlockType = iBlockType % iNUMBER_OF_BLOCK_TYPES;

	// Keep track of Current Blocks type and rotate position
	sCheckBlock.iBlockType = iBlockType;
	sCheckBlock.iRotatePosition = 0;
	
	// Load the correct block and rotate position into Check Block
	for (iHeight = 0; iHeight < iBLOCK_HEIGHT; iHeight++)
	{
		for (iWidth = 0; iWidth < iBLOCK_WIDTH; iWidth++)
		{
			sCheckBlock.iPicture[iHeight][iWidth] = 
				miBlockData[sCheckBlock.iBlockType][sCheckBlock.iRotatePosition][iHeight][iWidth];
		}
	}

	// Erase Current Block from board to check if move is valid
	EraseEntireBlockFromBoard();

	// Check if move is valid
	if (IsEntireBlockMoveValid(sCheckBlock))
	{
		// Move is valid so make Current Block equal to Check Block
		msCurrentBlock = sCheckBlock;

		// Store that the move was valid in bRoateSuccessful
		bChangeSuccessful = true;
	}

	// Place Current Block back on board in either it's old or new position
	PlaceEntireBlockOnBoard();

	return bChangeSuccessful;	// Return if the move was valid or not
}

int CBlock::ReturnBlockType() const
// Returns which Type of Block Current Block is
{
	return msCurrentBlock.iBlockType;
}

bool CBlock::RotateBlockClockwise()
// Will rotate the current block in miBlockData clockwise 
// 90 degrees if the move is valid
// Returns true if it is a valid move, false if not
{
	int iHeight = 0;	// Looping variable
	int iWidth	= 0;	// Looping variable
	
	bool bRotateSuccessful = false;		// Holds if it is a valid move
	
	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// Increase Check Block to the next rotated "picture" first so we can check if valid
	sCheckBlock.iRotatePosition++;

	// Make sure iRoatePosition is a valid # between 0 and 3
	sCheckBlock.iRotatePosition = sCheckBlock.iRotatePosition % 4;
	
	// Load the correct block and rotate position into Check Block
	for (iHeight = 0; iHeight < iBLOCK_HEIGHT; iHeight++)
	{
		for (iWidth = 0; iWidth < iBLOCK_WIDTH; iWidth++)
		{
			sCheckBlock.iPicture[iHeight][iWidth] = 
				miBlockData[sCheckBlock.iBlockType][sCheckBlock.iRotatePosition][iHeight][iWidth];
		}
	}

	// Erase Current Block from board to check if move is valid
	EraseEntireBlockFromBoard();

	// Check if move is valid
	if (IsEntireBlockMoveValid(sCheckBlock))
	{
		// Move is valid so make Current Block equal to Check Block
		msCurrentBlock = sCheckBlock;

		// Store that the move was valid in bRoateSuccessful
		bRotateSuccessful = true;
	}

	// Place Current Block back on board in either it's old or new position
	PlaceEntireBlockOnBoard();

	return bRotateSuccessful;	// Return if the move was valid or not
}

bool CBlock::RotateBlockCounterClockwise()
// Will rotate the current block in miBlockData 
// counter-clockwise 90 degrees
// Returns true if it is a valid move, false if not
{
	int iHeight = 0;	// Looping variable
	int iWidth	= 0;	// Looping variable

	bool bRotateSuccessful = false;		// Holds if it is a valid move
	
	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// Decrease to the previous rotated "picture"
	sCheckBlock.iRotatePosition--;

	// If rotating counter-clockwise when in Rotated Position 0
	// set Rotated Position to highest possible Rotated Position #
	// which is 3, since the only possible positions are 0,1,2,3
	// to give impression block rotated correctly
	if (sCheckBlock.iRotatePosition < 0)
	{
		sCheckBlock.iRotatePosition = 3;
	}

	// Make sure miRoatePosition is a valid # between 0 and 3
	sCheckBlock.iRotatePosition = sCheckBlock.iRotatePosition % 4;
	
	// Load the correct block and rotate position into Check Block
	for (iHeight = 0; iHeight < iBLOCK_HEIGHT; iHeight++)
	{
		for (iWidth = 0; iWidth < iBLOCK_WIDTH; iWidth++)
		{
			sCheckBlock.iPicture[iHeight][iWidth] = 
				miBlockData[sCheckBlock.iBlockType][sCheckBlock.iRotatePosition][iHeight][iWidth];
		}
	}

	// Erase Current Block from board to check if move is valid
	EraseEntireBlockFromBoard();

	// Check if move is valid
	if (IsEntireBlockMoveValid(sCheckBlock))
	{
		// Move is valid so make Current Block equal to Check Block
		msCurrentBlock = sCheckBlock;

		// Store that the move was valid in bRoateSuccessful
		bRotateSuccessful = true;
	}

	// Place Current Block back on board in either it's old or new position
	PlaceEntireBlockOnBoard();

	return bRotateSuccessful;	// Return if it was a valid move or not
}

bool CBlock::MoveBlockLeft()
// Move Current Block left one cell
// Returns true if move is valid, false if not
{
	bool bMoveSuccessful = false;	// Holds if it is a valid move

	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// Move block left one cell
	sCheckBlock.iBlockCellColumn--;

	// Erase Current Block before checking if new block placement is valid
	EraseEntireBlockFromBoard();

	// Check if the move is valid
	if(IsEntireBlockMoveValid(sCheckBlock))
	{
		// Move Current Block to the left
		msCurrentBlock = sCheckBlock;

		// Store that the move was valid
		bMoveSuccessful = true;
	}

	// Put Current Block back on the board either in its new 
	// position, or in its old position
	PlaceEntireBlockOnBoard();

	return bMoveSuccessful;	// Return if the move was valid or not
}

bool CBlock::MoveBlockRight()
// Move Current Block right one cell
// Returns true if move is valid, false if not
{
	bool bMoveSuccessful = false;	// Holds if it is a valid move

	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// Move block right one cell
	sCheckBlock.iBlockCellColumn++;

	// Erase Current Block before checking if new block placement is valid
	EraseEntireBlockFromBoard();

	// Check if the move is valid
	if(IsEntireBlockMoveValid(sCheckBlock))
	{
		// Move Current Block to the right
		msCurrentBlock = sCheckBlock;
		// Store that the move was valid
		bMoveSuccessful = true;
	}
	
	// Put Current Block back on the board either in its new 
	// position, or in its old position
	PlaceEntireBlockOnBoard();

	return bMoveSuccessful;	// Return if the move was valid or not
}

bool CBlock::MoveBlockDown()
// Move Current Block down one cell
// Returns true if move was valid, false if not
{
	bool bMoveSuccessful = false;	// Holds if it is a valid mo

	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// Move block down one cell
	sCheckBlock.iBlockCellRow++;

	// Erase Current Block before checking if new block placement is valid
	EraseEntireBlockFromBoard();

	// Check if the move is valid
	if(IsEntireBlockMoveValid(sCheckBlock))
	{
		// Move Current Block to the left
		msCurrentBlock = sCheckBlock;

		// Store that the move was valid
		bMoveSuccessful = true;
	}

	// Put Current Block back on the board either in its new 
	// position, or in its old position
	PlaceEntireBlockOnBoard();

	return bMoveSuccessful;	// Return if the move was valid or not
}

bool CBlock::MoveBlockUp()
// Move Current Block up one cell
// Returns true if successful, false if not
{
	bool bMoveSuccessful = false;	// Holds if it is a valid move

	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// Move block up one cell
	sCheckBlock.iBlockCellRow--;

	// Erase Current Block before checking if new block placement is valid
	EraseEntireBlockFromBoard();

	// Check if the move is valid
	if(IsEntireBlockMoveValid(sCheckBlock))
	{
		// Move Current Block to the left
		msCurrentBlock = sCheckBlock;

		// Store that the move was valid
		bMoveSuccessful = true;
	}

	// Put Current Block back on the board either in its new 
	// position, or in its old position
	PlaceEntireBlockOnBoard();

	return bMoveSuccessful;	// Return if the move was valid or not
}

void CBlock::MoveEntireBlockToCell(int iRow, int iColumn)
// If the specified Cell Row and Column are valid on the Board
// then Current Block is moved to the specified Cell Row and Column
{
	int iHeight			= 0;	// Looping variable
	int iWidth			= 0;	// Looping variable
	int iBlockHeight	= 0;	// Holds the Height of this particular block
	int iBlockWidth		= 0;	// Holds the Width of this particular block
	int iRightmostCell	= 0;	// Holds the Rightmost Cell of this block
	int iBottommostCell	= 0;	// Holds the Bottommost Cell of this block
	bool bRowIsValid	= false;// Holds if the specified Row is valid
	bool bColumnIsValid = false;// Holds if the specified Column is valid

	// Cycle through Current Blocks entire "picture"
	for (iHeight = 0; iHeight < iBLOCK_HEIGHT; iHeight++)
	{
		for (iWidth = 0; iWidth < iBLOCK_WIDTH; iWidth++)
		{
			// If this part of the picture contains actual data 
			if (msCurrentBlock.iPicture[iHeight][iWidth] != 0)
			{
				// Check if this Height is larger than our Block Height
				if (iBlockHeight < iHeight)
				{
					// Set Block Height to this Height
					iBlockHeight = iHeight;
				}

				// Check if this Width is larger than our Block Width
				if (iBlockWidth < iWidth)
				{
					// Set Block Width to this Width
					iBlockWidth = iWidth;
				}
			}
		}
	}

	// Set the right and bottom most positions of this particular Block
	iRightmostCell	= iColumn + iBlockWidth;
	iBottommostCell = iRow + iBlockHeight;

	// Hold if the specified Row and Column are valid, respectively
	bRowIsValid = (iRow >= 0 && iBottommostCell < iBOARD_HEIGHT);
	bColumnIsValid = (iColumn >= 0 && iRightmostCell < iBOARD_WIDTH);

	// If the specified cell is valid
	if (bRowIsValid && bColumnIsValid)
	{
		// Erase Current Block from the Board
		EraseEntireBlockFromBoard();

		// Set Current Blocks position to the specified position
		SetBlockCellRow(iRow);
		SetBlockCellColumn(iColumn);

		// Place Current Block on the Board in it's new position
		PlaceEntireBlockOnBoard();
	}
}

bool CBlock::MoveEntireBlockToCellIfAvailable(int iRow, int iColumn)
// Move Current Block to the specified cell
// Returns true if the spot is not already occupied, false if it is
{
	bool bMoveSuccessful = false;	// Holds if it is a valid move

	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// Move Check Block to the specified cell to see
	// if this is a valid move
	sCheckBlock.iBlockCellRow		= iRow;
	sCheckBlock.iBlockCellColumn	= iColumn;

	// Check if the move is valid
	if(IsEntireBlockMoveValid(sCheckBlock))
	{
		// Erase Current Block off board
		EraseEntireBlockFromBoard();

		// Move Current Block to the left
		msCurrentBlock = sCheckBlock;

		// Put Current Block back on the board in the new position
		PlaceEntireBlockOnBoard();

		// Store that the move was valid
		bMoveSuccessful = true;
	}

	return bMoveSuccessful;	// Return if the move was valid or not
}

bool CBlock::MoveEntireBlockToCellAndLeaveImageIfAvailable(int iRow, int iColumn)
// Moves Current Block to the specified location and leaves the image
// of where Current Block was behind on the board
{
	bool bMoveSuccessful = false;	// Holds if it is a valid move

	// Create a temporary block to hold Current Blocks next move so we
	// can check if the next move is valid without modifying Current Block
	sBlock sCheckBlock = msCurrentBlock;

	// Move Check Block to the specified cell to see
	// if this is a valid move
	sCheckBlock.iBlockCellRow		= iRow;
	sCheckBlock.iBlockCellColumn	= iColumn;

	// Check if the move is valid
	if(IsEntireBlockMoveValid(sCheckBlock))
	{
		// Move Current Block to the left
		msCurrentBlock = sCheckBlock;

		// Put Current Block back on the board in the new position
		PlaceEntireBlockOnBoard();

		// Store that the move was valid
		bMoveSuccessful = true;
	}

	return bMoveSuccessful;	// Return if the move was valid or not
}

int CBlock::ReturnBlockCellRow() const
// Returns the cell row that the top left integer of the 
// Current Block "picture" is in
{
	return msCurrentBlock.iBlockCellRow;
}

int CBlock::ReturnBlockCellColumn() const
// Returns the cell column that the top left integer of the 
// Current Block "picture" is in
{
	return msCurrentBlock.iBlockCellColumn;
}

int  CBlock::ReturnBlockStartingCellRow() const
// Returns the cell row the block starts in
{
	return msCurrentBlock.iBlockStartCellRow;
}

int  CBlock::ReturnBlockStartingCellColumn() const
// Returns the cell column the block starts in
{
	return msCurrentBlock.iBlockStartCellColumn;
}

bool CBlock::IsEntireBlockMoveValid(sBlock& sCheckBlock) const
// Return true if there is enough room on the board
// for the desired move
{
	int iHeight = 0;			// Looping variable
	int iWidth	= 0;			// Looping variable
	bool bMoveIsValid = true;	// Holds if move is valid or not

	// Loop through entire block "picture"
	for (iHeight = 0; iHeight < iBLOCK_HEIGHT; iHeight++)
	{
		for (iWidth = 0; iWidth < iBLOCK_WIDTH; iWidth++)
		{
			// If current part of picture is not transparent (it's actuall data)
			if (sCheckBlock.iPicture[iHeight][iWidth] != 0)
			{
				// Check to see if the spot our data is to go in is already occupied
				if (!mcBoard.BoardCellIsEmpty(sCheckBlock.iBlockCellRow + iHeight,
											  sCheckBlock.iBlockCellColumn + iWidth))
				{
					// If the spot is occupied then return that the move is invalid without changing anything
					bMoveIsValid = false;
					break;
				}
			}					
		}
	}

	return bMoveIsValid;	// Return if the move was valid or not
}

void CBlock::PlaceEntireBlockOnBoard(int iColor)
// Places Current Block on the board
// Does not check if position is already occupied, just overwrites it
{
	int iHeight = 0;			// Looping variable
	int iWidth	= 0;			// Looping variable

	// Loop through entire block "picture"
	for (iHeight = 0; iHeight < iBLOCK_HEIGHT; iHeight++)
	{
		for (iWidth = 0; iWidth < iBLOCK_WIDTH; iWidth++)
		{
			// If this part of the picture is not transparent (contains actuall data)
			if (msCurrentBlock.iPicture[iHeight][iWidth] != 0)
			{
				if (iColor == iBLOCKS_DEFAULT_COLOR)
				{
					// Place the data onto the board using the "pictures" color
					mcBoard.PlaceBlockInBoardCell(msCurrentBlock.iBlockCellRow + iHeight,
												  msCurrentBlock.iBlockCellColumn + iWidth,
												  msCurrentBlock.iPicture[iHeight][iWidth]);
				}
				else
				{
					// Place the data onto the board using specified color
					mcBoard.PlaceBlockInBoardCell(msCurrentBlock.iBlockCellRow + iHeight,
												  msCurrentBlock.iBlockCellColumn + iWidth,
												  iColor);
				}
			}
		}
	}
}

void CBlock::EraseEntireBlockFromBoard()
// Draw over Current Blocks cells with the boards empty cell color
{
	PlaceEntireBlockOnBoard(mcBoard.ReturnBoardColor());
}

bool CBlock::SetBlockCellRow(int iRow)
// Sets Current Blocks cell row
{
	bool bSuccessful = false;	// Hold if move is successful or not

	// Make sure row is valid
	if (iRow >= 0 && iRow < iBOARD_HEIGHT)
	{
		// Set the blocks current cell row
		msCurrentBlock.iBlockCellRow = iRow;

		// Store that the move was successful
		bSuccessful = true;
	}
	
	return bSuccessful;			// Return if move was successful or not
}

bool CBlock::SetBlockCellColumn(int iColumn)
// Sets Current Blocks cell column
{
	bool bSuccessful = false;	// Hold if move is successful or not

	// Make sure columm is valid
	if (iColumn >= 0 && iColumn < iBOARD_WIDTH)
	{
		// Set the blocks current cell column
		msCurrentBlock.iBlockCellColumn = iColumn;

		// Store that the move was successful
		bSuccessful = true;
	}
	
	return bSuccessful;			// Return if move was successful or not
}

bool CBlock::SetBlockCellStartRow(int iRow)
// Sets Current Blocks starting cell row
{
	bool bSuccessful = false;	// Hold if move is successful or not

	// Make sure row is valid
	if (iRow >= 0 && iRow < iBOARD_HEIGHT)
	{
		// Set the blocks starting cell row
		msCurrentBlock.iBlockStartCellRow = iRow;

		// Store that the move was successful
		bSuccessful = true;
	}
	
	return bSuccessful;			// Return if move was successful or not
}

bool CBlock::SetBlockCellStartColumn(int iColumn)
// Sets Current Blocks starting column
{
	bool bSuccessful = false;	// Hold if move is successful or not

	// Make sure columm is valid
	if (iColumn >= 0 && iColumn < iBOARD_WIDTH)
	{
		// Set the blocks starting cell column
		msCurrentBlock.iBlockStartCellColumn = iColumn;

		// Store that the move was successful
		bSuccessful = true;
	}
	
	return bSuccessful;			// Return if move was successful or not
}

void CBlock::DrawAllBlocks()
// Draw all of the blocks on the board to the screen
{
	mcBoard.DrawBoard();
}

////////////////////////// Private Functions /////////////////////////////////////////////////////

void CBlock::LoadBlocksIntoBlockData()
// Loads the block data from the file into miBlockData
{
	ifstream ifBlockData;					// The file with all the block data
	char cBuffer			= ' ';		// Buffer to grab info from the file
	char* cpPtrToBuffer		= &cBuffer;	// Pointer to cBuffer needed for the char to int conversion
	int iBlockType			= 0;		// Looping variable
	int iRotatedPosition	= 0;		// Looping variable
	int iRow				= 0;		// Looping variable
	int iColumn				= 0;		// Looping variable

	// Open the file to grab the block info from
	ifBlockData.open("BlockTypes.dat");

	// If file did not open
	if (!ifBlockData)
	{
		return;
	}

	// While not at the end of the file
	while (ifBlockData)
	{
		ifBlockData.get(cBuffer);	// Grab the next char in the file

		// If cBuffer is not holding a valid number
		if (cBuffer != ' ' && cBuffer != '\n')
		{
			// Place the file info into the Block Data array
			miBlockData[iBlockType][iRotatedPosition][iRow][iColumn] 
				= atoi(cpPtrToBuffer);

			// Move to the next char in array
			iColumn++;

			// If all of the current row has been read
			if (iColumn == iBLOCK_WIDTH)
			{
				// Reset to column zero and move down to the next row
				iColumn = 0;
				iRow++;
			}

			// If a whole blocks rotation picture has been read
			if (iRow == iBLOCK_HEIGHT)
			{
				// Reset to row zero and move to the next rotated
				// position of the block
				iRow = 0;
				iRotatedPosition++;
			}

			// If every rotate position for the block has been read
			// move onto the next block type
			if (iRotatedPosition == iNUMBER_OF_BLOCK_ROTATED_POSITIONS)
			{
				iRotatedPosition = 0;
				iBlockType++;
			}
		}
	};
} 
