#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;

class TextureManager
{
	// static == one shared member for all instances
	static unordered_map<string, sf::Texture> textures;

public:
	static void LoadTexture(string fileName);  // LoadTexture("debug")
	static sf::Texture& GetTexture(string fileName);
	static void Clear(); // Call at end of program because we don't have a destructor
};

