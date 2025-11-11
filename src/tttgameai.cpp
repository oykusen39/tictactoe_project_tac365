// You can use this for the std::min / std::max functions
#include <algorithm>
#include "tttgameai.h"

bool IsGameOver(const GameState& state)
{
	//step 1: check if someone has won or tie
	float score = GetScore(state);
	if (score == 1.0 || score == -1.0) {
		return true;
	}

	//check if no squares left for a tie
	if (!hasEmptySquares(state)) {
		return true;
	}
	
	//step 3: otherwise, game still continues
	return false;
	
}

void GenStates(GTNode* root, bool xPlayer)
{
	//base case would be when the game is over 
	if (IsGameOver(root->mState)) {
		return;
	}

	//following the slide deck: iterate thorugh all poisitons on the board
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			// check if theposition is empty
			if (root->mState.mBoard[row][col] == GameState::Empty) {
				//1. generate a new child node
				GTNode* child = new GTNode();
				//2. set the child's board to the root's board
				child->mState = root->mState;
				//3. set the empty position to the player's symbol
				if (xPlayer) {
					child->mState.mBoard[row][col] = GameState::X;
				}
				else {
					child->mState.mBoard[row][col] = GameState::O;
				}
				//4. add the child node to root's children
				root->mChildren.push_back(child);
				//5. generate the child nodes for this child (recursion)
				GenStates(child, !xPlayer);
			}
		}
	}
}

float GetScore(const GameState& state)
{
	//check rows for a winner
	for (int row = 0; row < 3; ++row) {
        if (state.mBoard[row][0] == state.mBoard[row][1] &&
            state.mBoard[row][1] == state.mBoard[row][2]) {
            if (state.mBoard[row][0] == GameState::O) {
                return 1.0; // O wins
            } else if (state.mBoard[row][0] == GameState::X) {
                return -1.0; // X wins
            }
        }
    }

	//check columns for a winner
	for (int col = 0; col < 3; ++col) {
        if (state.mBoard[0][col] == state.mBoard[1][col] &&
            state.mBoard[1][col] == state.mBoard[2][col]) {
            if (state.mBoard[0][col] == GameState::O) {
                return 1.0; // O wins
            } else if (state.mBoard[0][col] == GameState::X) {
                return -1.0; // X wins
            }
        }
    }

	//check diagonals (2 ways) 
	if (state.mBoard[0][0] == state.mBoard[1][1] && //1 5 9
        state.mBoard[1][1] == state.mBoard[2][2]) {
        if (state.mBoard[0][0] == GameState::O) {
            return 1.0; // O wins
        } else if (state.mBoard[0][0] == GameState::X) {
            return -1.0; // X wins
        }
    }

	if (state.mBoard[0][2] == state.mBoard[1][1] && //3 5 7 
        state.mBoard[1][1] == state.mBoard[2][0]) {
        if (state.mBoard[0][2] == GameState::O) {
            return 1.0; // O wins
        } else if (state.mBoard[0][2] == GameState::X) {
            return -1.0; // X wins
        }
    }

	//if no winner, then it is a tie
	return 0.0;
}

float MinPlayer(const GTNode* node)
{
	//step 1: if the node is a leaf, retun its score
	if (node->mChildren.empty()) {  //if it has no children then its leaf
		return GetScore(node->mState);
	}
	//step 2: find the largest minimum score among child nodes
	float minScore = GetScore(node->mChildren[0]->mState); //starting of with the first child score
	for (int i = 0; i < node->mChildren.size(); i++) {
		float childScore = MaxPlayer(node->mChildren[i]);//recursion
		minScore = std::min(minScore, childScore); //min in algorithm (in assignment instructions)
	}
	return minScore;
}

float MaxPlayer(const GTNode* node)
{
	//step 1: if the node is a leaf, retun its score
	if (node->mChildren.empty()) { //if it has no children then its leaf
		return GetScore(node->mState);
	}
	//step 2: find the largest minimum score among child nodes
	float maxScore = GetScore(node->mChildren[0]->mState); //starting of with the first child score
	for (int i = 0; i < node->mChildren.size(); i++) {
		float childScore = MinPlayer(node->mChildren[i]); //recursion
		maxScore = std::max(maxScore, childScore); //max in algorithm (in assignment instructions)
	}
	return maxScore;
}

const GTNode* MinimaxDecide(const GTNode* root)
{
	//step 1 : starting off with the first child of root as the "best node"
	const GTNode* bestNode = root->mChildren[0];
	float bestScore = MinPlayer(bestNode); //checking the score with MinPlayer (recursion)

	//step 2: iterate thorugh all other child nodes of the root
	for (int i = 1; i < root->mChildren.size(); i++) { //im starting with the second child as first is initialized outside the loop
		const GTNode* child = root->mChildren[i];
		//step 3: checking the score for this child using MinPlayer (recursion)
		float childScore = MinPlayer(child);
		//now we should update the best score by picking the max
		if (childScore >bestScore) {
			bestScore = childScore;
			bestNode = child;
		}
	}
	return bestNode;
}

unsigned pickMove(const GameState& board)
{
	//step 1: create the root node using the current game state
	GTNode root;
	root.mState = board;

	//step 2: generate all possible game states for the AI's move
	//passing by reference because we wouldnt want extra copy
	GenStates(&root, false); //passing false because it's AI's turn (!Xplayer)
	//step 3: find the best child node for the AI using MinimaxDecide
	const GTNode* bestNode = MinimaxDecide(&root);
	//step 4: identifying the square that changed using my helper function
	return findChangedSquare(board, bestNode->mState);
}

//helper function i created 
bool hasEmptySquares(const GameState& state){
	for (int row = 0; row < 3; row++) {//iterating through the rows
		for (int col = 0; col < 3; col++) {//iterating through the columns for each row
			if (state.mBoard[row][col] == GameState::Empty) {
				return true; // found an epmty square
			}
		}
	}
	return false; //no empty square was found
}

//my helper function
int findChangedSquare(const GameState& orgState, const GameState& newState) {
	//iterating through the rows and columns of the board
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			//checking if the orgState has changed
			if (orgState.mBoard[row][col] != newState.mBoard[row][col]) {
				//converting the 2x2 matrix to 1-9 numbers
				return row * 3 + col + 1;
			}
		}
	}
	// there should be a change but if none found then return -1
	return -1;
}