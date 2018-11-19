#include "b013432fHelper.h"



void b013432fHelper::PrintTitle(std::string Text)
{
	auto textLength = Text.size();
	auto paddingSide = ((LINELENGTH - textLength)) / 2; //-4 for Edges and padding

	std::cout << "/";
	for (size_t i = 0; i < paddingSide; i++)
	{
		std::cout << "=";
	}
	std::cout << " " << Text << " ";
	for (size_t i = 0; i < paddingSide; i++)
	{
		std::cout << "=";
	}
	std::cout << "\\\n";
}

void b013432fHelper::PrintContentText(std::string Text)
{
	auto textLength = Text.size();
	auto paddingSide = ((LINELENGTH - textLength)) / 2; //-4 for Edges and padding

	std::cout << "|";
	for (size_t i = 0; i < paddingSide; i++)
	{
		std::cout << " ";
	}
	std::cout << " " << Text << " ";
	for (size_t i = 0; i < paddingSide; i++)
	{
		std::cout << " ";
	}
	std::cout << "|\n";
}

void b013432fHelper::PrintDividerLine()
{
	auto paddingSide = LINELENGTH + 2; //-4 for Edges and padding

	std::cout << "|";
	for (size_t i = 0; i < paddingSide; i++)
	{
		std::cout << "-";
	}
	std::cout << "|\n";
}

void b013432fHelper::PrintFooter()
{
	auto paddingSide = LINELENGTH + 2; //-4 for Edges and padding

	std::cout << "\\";
	for (size_t i = 0; i < paddingSide; i++)
	{
		std::cout << "=";
	}
	std::cout << "/\n";
}

double b013432fHelper::VectorDistance(Vector2D a, Vector2D b)
{
	int distancex = (b.x - a.x);
	int distancey = (b.x - a.y);
	distancex *= distancex;
	distancey *= distancey;

	return sqrt(abs(distancex - distancey));
}

Vector2D b013432fHelper::Lerp(float t, Vector2D a, Vector2D b)
{
	return (1 / t)*a + t * b;
}

std::vector<int> b013432fHelper::GetAlternatingValues(std::vector<int> vec1, std::vector<int> vec2)
{
	std::vector<int> output;
	if (vec1.size() != vec2.size()) {
		assert("GetAlternatingValues - Both Vectors vary in size");
	}
	else {
		for (int i = 0; i < vec1.size(); i++)
		{
			if (i % 2 == 0) {
				output.push_back(vec1[i]);
			}
			else {
				output.push_back(vec2[i]);
			}
		}
	}
	return output;
}
