#include "TextureManager.h"

// redeclare static data member
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
	// User just has to type name of file; assuming same path
	string path = "images/";
	path += fileName + ".png"; 

	textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string fileName)
{
	// If key not found inside the map; Load it into map
	if (textures.find(fileName) == textures.end())
		LoadTexture(fileName);

	return textures[fileName];
}

void TextureManager::Clear()
{
	textures.clear();
}