// High Scores header file (HighScores.h)

#ifndef CHIGH_SCORES_H
#define CHIGH_SCORES_H

typedef unsigned int Score;
typedef unsigned int PosInt;

const PosInt iMAX_NUMBER_OF_HIGH_SCORES	= 10;	// Max number of High Scores to save
const PosInt iMAX_NAME_LENGTH			= 30;	// Max lengh a players name can be

struct sPlayer
// Holds a players info
{
	char cName[iMAX_NAME_LENGTH + 1];	// Holds the Names of all the players in the High Score list (One extra spot for End Line character)
	Score iScore;						// Holds the Scores of all the players in the High Score list
	int iExtraInfo;						// Holds extra information (i.e. Level, etc)
	int iExtraInfo2;					// Holds extra information (i.e. Level, etc)
};

class CHighScores
// Holds Names and Scores of players with a High Score
{
	public:

		CHighScores();			// Default Constructor
		~CHighScores();			// Destructor

		// NOTE: High Score is only Inserted if it is GREATER than zero, and greater than the other Scores in the list
		bool Insert(char* cpName, Score iScore, int iExtraInfo = 0, int iExtraInfo2 = 0);	// Inserts a High Score into the list
	
		bool Delete(PosInt iHighScorePositionToDelete);	// Deletes a High Score from the list using which position the High Score is in
		bool Delete(char* cpName, Score iScore);		// Deletes a High Score from the list using the players Name and Score
		bool DeleteAllScores();							// Deletes all the High Scores from the list

		bool StoreName(PosInt iHighScorePosition, char* cpName) const;	// Returns the Name of the player in the specified High Score Position
		Score ReturnScore(PosInt iHighScorePosition) const;		// Returns the Score of the player in the specified High Score Position
		int ReturnExtraInfo(PosInt iHighScorePosition) const;	// Returns the Extra Info of the player in the specified High Score Position
		int ReturnExtraInfo2(PosInt iHighScorePosition) const;	// Returns the Extra Info2 of the player in the specified High Score Position
		Score ReturnLastPlaceScore() const;						// Returns the Score of the player in last place

		bool IsEmpty() const;			// Returns true if there are no High Scores in the list
		bool IsFull() const;			// Returns true if the High Scores list is full

		bool Save(char* cpFileName = "HighScores.dat");	// Saves the High Scores to the specified File
		bool Load(char* cpFileName = "HighScores.dat");	// Load the High Scores from the specified File

	private:

		sPlayer msPlayer[iMAX_NUMBER_OF_HIGH_SCORES];	// Holds all of our High Score info
};

#endif