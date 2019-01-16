//------------------------------------------------------------------------
//  Author: Paul Roberts 2016
//------------------------------------------------------------------------

#include "GameScreen_Conway.h"
#include <stdlib.h>
#include "../Texture2D.h"
#include "ConwayConstants.h"
#include "../TinyXML/tinyxml.h"
#include <iostream>
#include <cstdio>
#include <ctime>

//--------------------------------------------------------------------------------------------------

GameScreen_Conway::GameScreen_Conway(SDL_Renderer* renderer) : GameScreen(renderer)
{
	mMap = new int*[kConwayScreenWidth / kConwayTileDimensions];
	for (int i = 0; i < kConwayScreenWidth / kConwayTileDimensions; i++)
		mMap[i] = new int[kConwayScreenHeight / kConwayTileDimensions];

	mNextMap = new int*[kConwayScreenWidth / kConwayTileDimensions];
	for (int i = 0; i < kConwayScreenWidth / kConwayTileDimensions; i++)
		mNextMap[i] = new int[kConwayScreenHeight / kConwayTileDimensions];

	//Get all required textures.
	mWhiteTile = new Texture2D(renderer);
	mWhiteTile->LoadFromFile("Images/Conway/WhiteTile.png");
	mBlackTile = new Texture2D(renderer);
	mBlackTile->LoadFromFile("Images/Conway/BlackTile.png");

	//Start map has 50% active cells.
	CreateRandomMap(75);

	//Start game in frozen state.
	mPause = true;

	cout << "Press:" << endl;
	cout << "R - Generate random environment" << endl;
	cout << "L - Load seed file" << endl;
	cout << "SPACE - Pause / Unpause" << endl;

}

//--------------------------------------------------------------------------------------------------

GameScreen_Conway::~GameScreen_Conway()
{
	delete mWhiteTile;
	mWhiteTile = NULL;

	delete mBlackTile;
	mBlackTile = NULL;

	for (int i = 0; i < kConwayScreenWidth / kConwayTileDimensions; i++)
	{
		delete mMap[i];
		mMap[i] = NULL;
	}
	delete mMap;
	mMap = NULL;

	for (int i = 0; i < kConwayScreenWidth / kConwayTileDimensions; i++)
	{
		delete mMap[i];
		mNextMap[i] = NULL;
	}
	delete mNextMap;
	mNextMap = NULL;
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Conway::Render()
{
	for (int x = 0; x < kConwayScreenWidth / kConwayTileDimensions; x++)
	{
		for (int y = 0; y < kConwayScreenHeight / kConwayTileDimensions; y++)
		{
			switch (mMap[x][y])
			{
			case 0:
				mBlackTile->Render(Vector2D(x*kConwayTileDimensions, y*kConwayTileDimensions));
				break;

			case 1:
				mWhiteTile->Render(Vector2D(x*kConwayTileDimensions, y*kConwayTileDimensions));
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Conway::Update(size_t deltaTime, SDL_Event e)
{
	//Additional input outside of player.
	switch (e.type)
	{
	case SDL_QUIT:
		exit(0);
		break;

		//Deal with mouse click input.
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			mPause = !mPause;
			break;

		case SDLK_r:
			mPause = true;
			CreateRandomMap(75);
			break;

		case SDLK_l:
			mPause = true;
			LoadMap("Conway/ConwaySeed.xml");
			break;
		}
		break;

	default:
		break;
	}

	if (!mPause)
		UpdateMap();
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Conway::UpdateMap()
{
	//Rules
	//1. Any living cell with less than 2 live neighbour dies.
	//2. Any living cell with 2 or 3 live cells lives on to next generation.
	//3. Any live cell with more than 3 live neighbours dies.
	//4. Any dead cell with exactly 3 live neighbours becomes a living cell.

	for (int y = 0; y < kConwayScreenHeight / kConwayTileDimensions; y++)
	{
		for (int x = 0; x < kConwayScreenWidth / kConwayTileDimensions; x++)
		{
			mNextMap[x][y] = 0;
		}
	}

	clock_t start;
	double duration;
	start = std::clock();

	for (int y = 0; y < kConwayScreenHeight / kConwayTileDimensions; y++)
	{
		for (int x = 0; x < kConwayScreenWidth / kConwayTileDimensions; x++)
		{
			auto currentCell = mMap[x][y];
			auto livingCells = GetAdjacentCells(x, y);

			//We now know the number of adjacent living and dead cells
			if (currentCell == 1) {

				if (livingCells < 2 || livingCells > 3)
					mNextMap[x][y] = 0;
				if (livingCells == 2 || livingCells == 3)
					mNextMap[x][y] = 1;
			}
			else {
				if (livingCells == 3)
					mNextMap[x][y] = 1;
			}
		}
	}

	for (int y = 0; y < kConwayScreenHeight / kConwayTileDimensions; y++)
	{
		for (int x = 0; x < kConwayScreenWidth / kConwayTileDimensions; x++)
		{
			mMap[x][y] = mNextMap[x][y];
		}
	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << duration << "(" << (1 / duration) << " FPS)" << '\n';
}

int GameScreen_Conway::GetAdjacentCells(int x, int y) {

	int output = 0;

	if (mMap != nullptr) {

		for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
		{
			auto _x = x + i; auto _y = y + j;
			if (IsCellInBounds(_x, _y))
				output += mMap[_x][_y];
		}
	}
	return output;
}

bool GameScreen_Conway::IsCellInBounds(int x, int y)
{
	return (x >= 0 && x < (kConwayScreenWidth / kConwayTileDimensions) && y >= 0 && y < (kConwayScreenHeight / kConwayTileDimensions));
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Conway::CreateRandomMap(int percentageInactive)
{
	int randomNumber = 0;

	for (int x = 0; x < kConwayScreenWidth / kConwayTileDimensions; x++)
	{
		for (int y = 0; y < kConwayScreenHeight / kConwayTileDimensions; y++)
		{
			randomNumber = (rand() % 100) + 1;

			if (randomNumber > percentageInactive)
				mMap[x][y] = 1;
			else
				mMap[x][y] = 0;
		}
	}
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Conway::LoadMap(std::string path)
{
	//Get the whole xml document.
	TiXmlDocument doc;
	if (!doc.LoadFile(path))
	{
		cerr << doc.ErrorDesc() << endl;
	}

	//Now get the root element.
	TiXmlElement* root = doc.FirstChildElement();
	if (!root)
	{
		cerr << "Failed to load file: No root element." << endl;
		doc.Clear();
	}
	else
	{
		//Jump to the first 'objectgroup' element.
		for (TiXmlElement* groupElement = root->FirstChildElement("objectgroup"); groupElement != NULL; groupElement = groupElement->NextSiblingElement())
		{
			string name = groupElement->Attribute("name");
			if (name == "Seed")
			{
				int x = 0;
				int y = 0;

				//Jump to the first 'object' element - within 'objectgroup'
				for (TiXmlElement* objectElement = groupElement->FirstChildElement("object"); objectElement != NULL; objectElement = objectElement->NextSiblingElement())
				{
					string name = objectElement->Attribute("name");
					if (name == "TileTypes")
					{
						//Reset x position of map to 0 at start of each element.
						x = 0;

						//Split up the comma delimited connections.
						stringstream ss(objectElement->Attribute("value"));
						int i;
						while (ss >> i)
						{
							mMap[x][y] = i;

							if (ss.peek() == ',')
								ss.ignore();

							//Increment the x position.
							x++;
						}

						//Move down to the next y position in the map.
						y++;
					}
				}
			}
		}
	}
}