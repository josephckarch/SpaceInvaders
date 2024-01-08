#pragma once
#include <random>

class Random
{
	// Static members are shared by ALL instances of the class
	// Static functions can be called without creating an object first
	static std::mt19937 random;
public:
	static int Int(int min, int max);
	static float Float(float min, float max);
};