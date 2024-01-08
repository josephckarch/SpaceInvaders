#include <SFML/Graphics.hpp>
#include "Tile.h"

Tile::Tile() {
	sf::Sprite tile(TextureManager::GetTexture("tile_hidden"));
	sf::Sprite mine(TextureManager::GetTexture("mine"));
	sf::Sprite flag(TextureManager::GetTexture("flag"));
	tileSprite = tile;
	mineSprite = mine;
	flagSprite = flag;

	numAdjMines = 0;
	hidden = true;
	hasMine = false;
	flagged = false;
}

void Tile::Draw(sf::RenderWindow& window) {

	// draw tile sprite beneath flag
	window.draw(tileSprite);

	// draw flag if tile is flagged
	if (flagged && hidden)
		window.draw(flagSprite);

	// draw mine if tile is revealed
	if (!hidden && hasMine)
		window.draw(mineSprite);

	// draw number if revealed and does not have mine
	if (!hidden && !hasMine && (numAdjMines > 0))
		window.draw(numberSprite);

}

void Tile::DrawWithMines(sf::RenderWindow& window)
{
	Draw(window);

	// draw mine on top of everything else
	if (hasMine)
		window.draw(mineSprite);
}

void Tile::SetPosition(int x, int y)
{
	tileSprite.setPosition(x, y);
	mineSprite.setPosition(x, y);
	flagSprite.setPosition(x, y);
	numberSprite.setPosition(x, y);
}

void Tile::AssignNumber()
{
	// assign correct number of adjacent mines
	for (unsigned int i = 0; i < adjacentTiles.size(); i++) {
		if (adjacentTiles[i]->hasMine)
			numAdjMines++;
	}

	// assign numberSprite representative of numAdjMines
	switch (numAdjMines) {
	case 0:
		break;
	case 1:
		numberSprite.setTexture(TextureManager::GetTexture("number_1"));
		break;
	case 2:
		numberSprite.setTexture(TextureManager::GetTexture("number_2"));
		break;
	case 3:
		numberSprite.setTexture(TextureManager::GetTexture("number_3"));
		break;
	case 4:
		numberSprite.setTexture(TextureManager::GetTexture("number_4"));
		break;
	case 5:
		numberSprite.setTexture(TextureManager::GetTexture("number_5"));
		break;
	case 6:
		numberSprite.setTexture(TextureManager::GetTexture("number_6"));
		break;
	case 7:
		numberSprite.setTexture(TextureManager::GetTexture("number_7"));
		break;
	case 8:
		numberSprite.setTexture(TextureManager::GetTexture("number_8"));
		break;
	default:
		cout << "numAdjMines is over the max value of 8!... Inside Tile.cpp\n";
	}
}
