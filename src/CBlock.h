// CBlock.h

#ifndef CBLOCK_H
#define CBLOCK_H

#include "CBoard.h"

const int iBLOCK_HEIGHT							= 4;	// Blocks max height in cells
const int iBLOCK_WIDTH							= 4;	// Blocks max width in cells
const int iNUMBER_OF_BLOCK_ROTATED_POSITIONS	= 4;	// Blocks rotated angle
const int iNUMBER_OF_BLOCK_TYPES				= 7;	// 7 possible different blocks
const int iBLOCKS_DEFAULT_COLOR					= -1;	// Flag to use the blocks default color
const int iRANDOM_BLOCK							= -1;	// Flag to use a random block

// The block types
const int iI			= 0;
const int iL			= 1;
const int iBACKWARDS_L	= 2;
const int iT			= 3;
const int iS			= 4;
const int iZ			= 5;
const int iSQUARE		= 6;

// Structure to hold our blocks and their info
struct sBlock
{
	int iPicture[iBLOCK_HEIGHT][iBLOCK_WIDTH];	// Holds the "picture" of the block
	int iBlockType;								// The block CurrentBlock is set to
	int iRotatePosition;						// The rotation position CurrentBlock is set to
	int iBlockCellRow;							// The cell row that the top-left integer of the block "picture" is in
	int iBlockCellColumn;						// The cell column that the top-left integer of the block "picture" is in
	int iBlockStartCellRow;						// The cell row the block first occupies
	int iBlockStartCellColumn;					// The cell column the block first occupies
};

class CBlock
{
	public:

		// Constructor with default settings
		// Must specify the video buffer and it's lpitch
		CBlock(int iBlockType = iRANDOM_BLOCK, int iBlockStartCellRow = 0, int iBlockStartCellColumn = 4);
		~CBlock();								

		void ChangeBlockType(int iBlockType0to6 = iRANDOM_BLOCK);// Change Current Block to the specified Type
		bool ChangeBlockTypeAndPutOnBoardIfValid(int iBlockType0to6 = iRANDOM_BLOCK);// Change Current Block to the specified Type only if it is valid on the Board
		int  ReturnBlockType() const;					// Return Current Blocks Type

		bool RotateBlockClockwise();					// Rotate Current Block Clockwise
		bool RotateBlockCounterClockwise();				// Rotate Current Block Counter-Clockwise

		bool MoveBlockLeft();							// Move block left one cell
		bool MoveBlockRight();							// Move block right one cell
		bool MoveBlockDown();							// Move block down one cell
		bool MoveBlockUp();								// Move block up one cell
		
		int  ReturnBlockCellRow() const;				// Return the Blocks current Cell Row
		int  ReturnBlockCellColumn() const;				// Return the Blocks current Cell Column
		int  ReturnBlockStartingCellRow() const;		// Return the Blocks starting Cell Row
		int  ReturnBlockStartingCellColumn() const;		// Return the Blocks starting Cell Column

		bool SetBlockCellRow(int iRow);					// Move the Block to the specified Cell Row
		bool SetBlockCellColumn(int iColumn);			// Move the Block to the specified Cell Column
		bool SetBlockCellStartRow(int iRow);			// Change the Blocks starting Cell Row to the specified row
		bool SetBlockCellStartColumn(int iColumn);		// Change the Blocks starting Cell Column to the specified column

		bool IsEntireBlockMoveValid(sBlock& sCheckBlock) const;				// Returns true if the block sent will not disturb anything else on the board
		void MoveEntireBlockToCell(int iRow, int iColumn);					// Moves Current Block to the specified cell even if the specified cell is already occupied
		bool MoveEntireBlockToCellIfAvailable(int iRow, int iColumn);		// Moves Current Block to the specified cell if the cells are not already occupied
		bool MoveEntireBlockToCellAndLeaveImageIfAvailable(int iRow, int iColumn);		// Moves Current Block to the specified cell and leaves an image of the block behind as well, if the specified cell is not already occupied

		void PlaceEntireBlockOnBoard(int iColor = iBLOCKS_DEFAULT_COLOR);	// Place Current Block onto the Board
		void EraseEntireBlockFromBoard();									// Erase Current Block from the Board

		void DrawAllBlocks();							// Draw every Block on the Board

		CBoard mcBoard;		// Holds the Board and all the information on it

	private:
		
		// Loads the block type data from the file into mcBlockData
		void LoadBlocksIntoBlockData();

		// Holds all the positions of every block
		int miBlockData[iNUMBER_OF_BLOCK_TYPES][iNUMBER_OF_BLOCK_ROTATED_POSITIONS][iBLOCK_HEIGHT][iBLOCK_WIDTH];
		
		// Holds the Current Blocks "picture"
		sBlock msCurrentBlock;
};

#endif