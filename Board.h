#pragma once
#include "Tile.h"
#include <vector>

class Board
{
public:
	int width;
	int height;
	int numTiles;
	int numMines;
	int numFlags;
	bool showMines;
	bool gameLost;
	bool gameWon;

	// vector of tiles that each have their own sprites(tile_hidden.png) and locations
	vector<Tile> tiles;
	// vector of tiles that each have their own sprites(mine.png) and locations
	vector<Tile> mines;
	// vector of ints that contain the tile #'s that have a mine on them
	vector<int> mineLocations;
	// vector of Sprites that contains that 5 buttons on bottom
	vector<sf::Sprite> buttonSprites;

	Board();
	void Reset();
	void IncFlags();
	void DecFlags();
	bool CheckTilesRevealed();
};

