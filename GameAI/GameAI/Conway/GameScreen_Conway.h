//------------------------------------------------------------------------
//  Author: Paul Roberts 2016
//------------------------------------------------------------------------

#ifndef _GAMESCREEN_CONWAY_H
#define _GAMESCREEN_CONWAY_H

#include "../GameScreen.h"
#include "../Commons.h"
#include <SDL.h>
#include <vector>
#include <algorithm>

using namespace::std;

class Texture2D;

class GameScreen_Conway : GameScreen
{
//--------------------------------------------------------------------------------------------------
public:
	GameScreen_Conway(SDL_Renderer* renderer);
	~GameScreen_Conway();

	void Render();
	void Update(size_t deltaTime, SDL_Event e);

//--------------------------------------------------------------------------------------------------
protected:

	//TODO: Students to code these functions.
	void UpdateMap()								;//{;}
	void CreateRandomMap(int percentageInactive)	;//{;}
	void LoadMap(std::string path);
	int GetAdjacentCells(int x, int y);//Gets the adjacent cells to the one specified

//--------------------------------------------------------------------------------------------------
private:
	int**		mMap;
	int**		mNextMap; //Next map on each update

	Texture2D*	mWhiteTile;
	Texture2D*	mBlackTile;

	bool		mPause;
};


#endif //_GAMESCREEN_CONWAY_H