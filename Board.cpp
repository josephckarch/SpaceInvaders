#include "Board.h"


Board::Board() {
	width = 0;
	height = 0;
	numTiles = 0;
	numMines = 0;
	numFlags = 0;
	showMines = false;
	gameLost = false;
	gameWon = false;
}

void Board::Reset() {
	numFlags = 0;
	gameLost = false;
	gameWon = false;

	// Reset smiley
	buttonSprites[0].setTexture(TextureManager::GetTexture("face_happy"));
}

void Board::IncFlags() {
	numFlags++;
}

void Board::DecFlags() {
	numFlags--;
}

bool Board::CheckTilesRevealed() {
	int numTilesRevealed = 0;

	for (unsigned int i = 0; i < tiles.size(); i++) {
		// if the tile does not have a mine
		if (!tiles[i].hasMine) {
			// if the tiles is revealed
			if (!tiles[i].hidden)
				numTilesRevealed++;
		}
	}

	if (numTilesRevealed == (numTiles - numMines))
		return true;
	
	return false;
}
