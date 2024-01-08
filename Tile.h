#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Tile
{

public:
	
	sf::Sprite tileSprite;
	sf::Sprite mineSprite;
	sf::Sprite flagSprite;
	sf::Sprite numberSprite;

	// vector of pointers to adjacent tiles
	vector<Tile*>adjacentTiles;

	int numAdjMines;
	bool hidden;
	bool hasMine;
	bool flagged;

	
	Tile();

	void Draw(sf::RenderWindow& window);
	void DrawWithMines(sf::RenderWindow& window);
	void SetPosition(int x, int y);
	void AssignNumber();
};

