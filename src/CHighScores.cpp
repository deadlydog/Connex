// High Scores Implementaion File (CHighScores.cpp)

#include "CHighScores.h"
#include <fstream>	// Needed to save file
#include <string>	// Needed to copy Names

using namespace std;

/*	Private Data:
			sPlayer msPlayer[iMAX_NUMBER_OF_HIGH_SCORES];	// Holds all of our High Score info
*/

CHighScores::CHighScores()
// Default Constructor
// Sets all private data to default values
{
	PosInt iHighScoreIndex	= 0;	// Looping variable
	PosInt iNameLengthIndex	= 0;	// Looping variable

	// Cycle through private data and set it all to empty
	for (iHighScoreIndex = 0; iHighScoreIndex < iMAX_NUMBER_OF_HIGH_SCORES; iHighScoreIndex++)
	{
		for (iNameLengthIndex = 0; iNameLengthIndex < iMAX_NAME_LENGTH; iNameLengthIndex++)
		{
			// Set name to nothing
			msPlayer[iHighScoreIndex].cName[iNameLengthIndex] = 0;
		}
		
		// Set the last character of the name to end line
		msPlayer[iHighScoreIndex].cName[iMAX_NAME_LENGTH] = '\n';

		// Set Score and Extra to zero
		msPlayer[iHighScoreIndex].iScore = 0;
		msPlayer[iHighScoreIndex].iExtraInfo = 0;
		msPlayer[iHighScoreIndex].iExtraInfo2 = 0;
	}
}

CHighScores::~CHighScores()
// Class Destructor
{}

bool CHighScores::Insert(char* cpName, Score iScore, int iExtraInfo, int iExtraInfo2)
// Inserts a High Score into the list
// Returns True if player was inserted into High Score list, False if not
// NOTE: Player will only be inserted if their Score is better than
//		 any of the other Scores in the High Score list, AND their
//		 Score is GREATER than zero
{
	PosInt iSpotToInsertTo		= 0;	// Holds the High Score Position to Insert the new High Score in to
	bool bInsertSuccessful		= false;// Holds if Insert was successful or not
	PosInt iHighScoreIndex		= 0;	// Looping variable

	// Find which position to Insert the new High Score in to
	while (iScore <= msPlayer[iSpotToInsertTo].iScore && iSpotToInsertTo < iMAX_NUMBER_OF_HIGH_SCORES)
	{
		iSpotToInsertTo++;
	}

	// If the Position To Insert To is valid
	if (iSpotToInsertTo < iMAX_NUMBER_OF_HIGH_SCORES)
	{
		// Move all High Scores under Position To Insert To down one
		for (iHighScoreIndex = (iMAX_NUMBER_OF_HIGH_SCORES - 1); 
			 iHighScoreIndex > iSpotToInsertTo; iHighScoreIndex--)
		{
			// Move Name and Score down one spot
			//msPlayer[iHighScoreIndex] = msPlayer[iHighScoreIndex - 1];
			strncpy(msPlayer[iHighScoreIndex].cName, msPlayer[iHighScoreIndex - 1].cName, iMAX_NAME_LENGTH);
			msPlayer[iHighScoreIndex].iScore = msPlayer[iHighScoreIndex - 1].iScore;
			msPlayer[iHighScoreIndex].iExtraInfo = msPlayer[iHighScoreIndex - 1].iExtraInfo;
			msPlayer[iHighScoreIndex].iExtraInfo2 = msPlayer[iHighScoreIndex - 1].iExtraInfo2;
		}

		// Place new Name and Score into list
		strncpy(msPlayer[iSpotToInsertTo].cName, cpName, iMAX_NAME_LENGTH);
		msPlayer[iSpotToInsertTo].iScore = iScore;
		msPlayer[iSpotToInsertTo].iExtraInfo = iExtraInfo;
		msPlayer[iSpotToInsertTo].iExtraInfo2 = iExtraInfo2;

		bInsertSuccessful = true;	// Let us know the Insert was Successful
	}

	return bInsertSuccessful;		// Return if Successful or not
}

bool CHighScores::Delete(PosInt iHighScorePositionToDelete)
// Removes the specified High Score from the list using which 
// position the High Score is currently in
// Returns True if successful, False if not
{
	bool bDeleteSuccessful	= false;	// Holds if the Delete was successful or not
	bool bPositionIsValid	= false;	// Holds if the specified Position is valid or not
	PosInt iHighScoreIndex	= 0;		// Looping variable
	PosInt iNameLengthIndex	= 0;		// Looping varibale
	
	bPositionIsValid = ((iHighScorePositionToDelete <= iMAX_NUMBER_OF_HIGH_SCORES)
					 && (iHighScorePositionToDelete > 0));

	// If the list is not empty and the specified Position is valid
	if (!IsEmpty() && bPositionIsValid)
	{
		// Move all Scores below High Score Position To Delete, up one position
		for (iHighScoreIndex = (iHighScorePositionToDelete - 1);
			 iHighScoreIndex < (iMAX_NUMBER_OF_HIGH_SCORES - 1); iHighScoreIndex++)
		{
			// Move Name and Score up one spot
			msPlayer[iHighScoreIndex] = msPlayer[iHighScoreIndex + 1];
		}

		// Place a blank High Score in the end of the list

		// Make last place name empty
		for (iNameLengthIndex = 0; iNameLengthIndex < iMAX_NAME_LENGTH; iNameLengthIndex++)
		{
			msPlayer[iMAX_NUMBER_OF_HIGH_SCORES - 1].cName[iNameLengthIndex] = 0;
		}

		// Make last place Score and Extra Info zero
		msPlayer[iMAX_NUMBER_OF_HIGH_SCORES - 1].iScore = 0;
		msPlayer[iMAX_NUMBER_OF_HIGH_SCORES - 1].iExtraInfo = 0;
		msPlayer[iMAX_NUMBER_OF_HIGH_SCORES - 1].iExtraInfo2 = 0;

		bDeleteSuccessful = true;	// Let us know Delete was Successful
	}

	return bDeleteSuccessful;		// Return if the Delete was Successful or not
}

bool CHighScores::Delete(char* cpName, Score iScore)
// Deletes the specified High Score from the list
// Returns True if Successful, False if not
// NOTE: If there is more than one match, only Deletes the lowest
//		 positioned one
{
	bool bDeleteSuccessful	= false;	// Holds if the Delete was successful or not
	bool bNamesMatch		= false;	// Holds if the Names match or not
	bool bScoresMatch		= false;	// Holds if the Scores match or not
	const PosInt iSameNames	= 0;		// Used in comparing if the Names are the same or not
	PosInt iPosition		= 0;		// Holds the Position the High Score to Delete is in
	PosInt iHighScoreIndex	= 0;		// Looping variable

	// If the list is not empty
	if (!IsEmpty())
	{
		// Cycle through High Score list
		for (iHighScoreIndex = 0; iHighScoreIndex < iMAX_NUMBER_OF_HIGH_SCORES; iHighScoreIndex++)
		{
			// Compare the Names and Scores and hold if they are a match or not
			bNamesMatch = (strcmp(cpName, msPlayer[iHighScoreIndex].cName) == iSameNames);
			bScoresMatch = (msPlayer[iHighScoreIndex].iScore == iScore);

			// If both the Names and the Scores Match
			if (bNamesMatch && bScoresMatch)
			{
				// Hold the Position of the High Score to Delete
				iPosition = iHighScoreIndex + 1;
			}
		}
	}

	// If we found a match
	if (iPosition > 0)
	{
		// Delete the High Score and store if it was Successful
		bDeleteSuccessful = Delete(iPosition);
	}

	return bDeleteSuccessful;	// Return if the Delete was Successful or not
}

bool CHighScores::DeleteAllScores()
// Deletes all High Scores from the list
// Returns True if Successful, False if not
{
	const PosInt iFirstPlacePosition = 1;		// Holds the First Place Position
	
	// While the High Score list is not empty
	while (!IsEmpty())
	{
		// Delete the First Place High Score
		Delete(iFirstPlacePosition);
	}

	return (IsEmpty());		// Return if all the High Scores were Deleted from the list or not
}

bool CHighScores::StoreName(PosInt iHighScorePosition, char* cpName) const
// Places the specified Positions Name in cName[]
// Returns True if the specified Position was Valid and the Name was stored in cpName
// Returns False if the specified Position was Invalid and leaves cpName as is
{
	bool bPositionIsValid = false;	// Holds if the specified Position is Valid or not

	// Store if the specified Position is Valid or not
	bPositionIsValid = ((iHighScorePosition <= iMAX_NUMBER_OF_HIGH_SCORES) 
					 && (iHighScorePosition > 0));

	// If the specified Position is Valid
	if (bPositionIsValid)
	{
		// Copy the Positions Name into cpName
		strncpy(cpName, msPlayer[iHighScorePosition - 1].cName, iMAX_NAME_LENGTH);
	}

	return bPositionIsValid;	// Returns if the operation was Successful or not
}

Score CHighScores::ReturnScore(PosInt iHighScorePosition) const
// Returns the specified Positions Score
// NOTE: Returns zero if the specified Position is not valid, or there
//		 isn't a High Score in the Position yet
{
	bool bPositionIsValid	= false;	// Holds if the specified Position is Valid or not
	Score iScore			= 0;		// Holds the specified Positions Score

	// Store if the specified Position is Valid or not
	bPositionIsValid = ((iHighScorePosition <= iMAX_NUMBER_OF_HIGH_SCORES) 
					 && (iHighScorePosition > 0));

	// If the specified Position is Valid
	if (bPositionIsValid)
	{
		// Store the Positions Score in iScore
		iScore = msPlayer[iHighScorePosition - 1].iScore;
	}

	return iScore;		// Returns the Score of the specified Position
}

int CHighScores::ReturnExtraInfo(PosInt iHighScorePosition) const
// Returns the specified Positions Extra Info
// NOTE: Returns zero if no Extra Info was specified
{
	bool bPositionIsValid	= false;	// Holds if the specified Position is Valid or not
	int iExtra				= 0;		// Holds the specified Positions Extra Info

	// Store if the specified Position is Valid or not
	bPositionIsValid = ((iHighScorePosition <= iMAX_NUMBER_OF_HIGH_SCORES) 
					 && (iHighScorePosition > 0));

	// If the specified Position is Valid
	if (bPositionIsValid)
	{
		// Store the Positions Extra Info
		iExtra = msPlayer[iHighScorePosition - 1].iExtraInfo;
	}

	return iExtra;		// Returns the Extra Info of the specified Position
}

int CHighScores::ReturnExtraInfo2(PosInt iHighScorePosition) const
// Returns the specified Positions Extra Info
// NOTE: Returns zero if no Extra Info was specified
{
	bool bPositionIsValid	= false;	// Holds if the specified Position is Valid or not
	int iExtra2				= 0;		// Holds the specified Positions Extra Info

	// Store if the specified Position is Valid or not
	bPositionIsValid = ((iHighScorePosition <= iMAX_NUMBER_OF_HIGH_SCORES) 
					 && (iHighScorePosition > 0));

	// If the specified Position is Valid
	if (bPositionIsValid)
	{
		// Store the Positions Extra Info
		iExtra2 = msPlayer[iHighScorePosition - 1].iExtraInfo2;
	}

	return iExtra2;		// Returns the Extra Info2 of the specified Position
}

Score CHighScores::ReturnLastPlaceScore() const
// Returns the Score of the Last Place Position
// NOTE: Returns zero if there is no High Score in the Last Place Position
{
	return (msPlayer[iMAX_NUMBER_OF_HIGH_SCORES - 1].iScore);
}

bool CHighScores::IsEmpty() const
// Checks if the High Score List is Emtpy
// Returns True if it is Emtpy, False if it's not
{
	const PosInt iFirstPlacePosition = 0;	// Holds the Position of the First Place player

	// Returns True if there is no Score in the First Place Position
	return (msPlayer[iFirstPlacePosition].iScore == 0);
}

bool CHighScores::IsFull() const
// Checks if the High Score List if Full
// Returns True if List is Full, False if not
{
	const PosInt iLastPlacePosition = (iMAX_NUMBER_OF_HIGH_SCORES - 1);	// Holds the Position of the Last Place player
	
	// Returns True if there is a Score in the Last Place Position
	return (msPlayer[iLastPlacePosition].iScore > 0);
}

bool CHighScores::Save(char* cpFileName)
// Saves the High Scores to the specified File Name
// Returns True if Successful, False if not
// NOTE: Automatically overwrites other files with the same File Name
{
	bool bSaveSuccessful = false;	// Holds if the Save was Successful or not
	
	// Opens the specified File to write to
	ofstream ofHighScoreSaveFile;
	ofHighScoreSaveFile.open(cpFileName);

	// If the File was opened Successfully
	if (ofHighScoreSaveFile)
	{
		// Write this class info out to the File
		ofHighScoreSaveFile.write((char*)this, sizeof(CHighScores));

		// Store that the Save was Successful
		bSaveSuccessful = true;
	}

	ofHighScoreSaveFile.close();	// Close the File

	return bSaveSuccessful;			// Return if the Save was Successful or not
}

bool CHighScores::Load(char* cpFileName)
// Loads a High Score File from the specified File Name
// Returns True if the Load was Successful, False if not
{
	bool bLoadSuccessful = false;	// Holds if the Load was Successful or not

	// Opens the specified File to read from
	ifstream ifHighScoreLoadFile;
	ifHighScoreLoadFile.open(cpFileName, ios_base::in);

	// If the File was opened Successfully
	if (ifHighScoreLoadFile)
	{
		// Read in the class info from the File
		ifHighScoreLoadFile.read((char*) this, sizeof(CHighScores));

		// Store that the Load was Successful
		bLoadSuccessful = true;
	}

	ifHighScoreLoadFile.close();	// Close the File

	return bLoadSuccessful;			// Return if the Load was Successful or not
}