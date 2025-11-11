#include "tttgame.h"
#include "tttgameai.h"

TicTacToeGame::TicTacToeGame()
{
	for (int i = 0; i < 3; i++) { //iterating through the rows
		for (int j = 0; j < 3; j++) { //iterating through the columns for each row
			currentState.mBoard[i][j] = GameState::Empty; //setting each square empty
		}
	}
}

GameState TicTacToeGame::getBoard() const
{
	return currentState;
}

bool TicTacToeGame::setSquareState(unsigned short row, unsigned short col, GameState::SquareState state)
{
	// checking if the square is empty
	if (currentState.mBoard[row][col] == GameState::Empty) {
		//setting the square to the state
		currentState.mBoard[row][col] = state;
		return true; //successfully set the state
	}
	//if the square was not empty then return false directly
	return false;
}

bool TicTacToeGame::setSquareState(unsigned short square, GameState::SquareState state)
{
	// logic: to get row: first subtract 1 (because this starts from 1 but row col starts from 0)
	// row -> (square - 1) /3      column -> (square - 1) % 3
	int row = (square - 1) / 3;
	int col = (square - 1) % 3;
	return setSquareState(row, col, state); //call this version with row and col
}

char TicTacToeGame::getWinner()
{
	//call getscore to chck the board state
	float score = GetScore(currentState);

	//determine the result
	if (score == 1.0) {
		return 'O';
	}
	else if (score == -1.0) {
		return 'X';
	}
	
    //check if the game is still ongoing and no winners yet
	if (hasEmptySquares(currentState)) {
		return ' '; //game is still ongoing
	}
    
	//if no winner and no empty square, it is a tie
	return 'N';
	
}

