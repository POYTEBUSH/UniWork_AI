#pragma once
#include <string>
#include <iostream>
#include "../GameAI/Commons.h"
#include <vector>
#include <assert.h>

#define LINELENGTH 100
class b013432fHelper
{

public:
	b013432fHelper() {};
	~b013432fHelper() {};

	void PrintTitle(std::string Text);
	void PrintContentText(std::string Text);
	void PrintDividerLine();
	void PrintFooter();

	double VectorDistance(Vector2D a, Vector2D b);

	Vector2D Lerp(float t, Vector2D a, Vector2D b);

	std::vector<int>GetAlternatingValues(std::vector<int> vec1, std::vector<int> vec2);
private:
};

