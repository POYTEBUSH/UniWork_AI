//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _GAMESCREEN_SNAKE_H
#define _GAMESCREEN_SNAKE_H

#include "../GameScreen.h"
#include "../Commons.h"
#include "SnakeConstants.h"
#include "SnakeCommons.h"
#include <SDL.h>
#include <vector>

using namespace::std;

class Texture2D;

class GameScreen_Snake : GameScreen
{
//--------------------------------------------------------------------------------------------------
public:
	GameScreen_Snake(SDL_Renderer* renderer);
	~GameScreen_Snake();

	void Render();
	void Update(size_t deltaTime, SDL_Event e);

//--------------------------------------------------------------------------------------------------
private:

	void UpdateMap();
	
	void UpdateSnake(size_t deltaTime, SDL_Event e);
	void DoSnakeMovement(size_t deltaTime);
	void RenderSnake();

	void ResetMap();
	void ResetPickup();
	void ResetGame();

	void AddToSnakeBody(unsigned int numberOfSegments);
	void ResetSnake();

	void IncreaseSpeed();
	

//--------------------------------------------------------------------------------------------------
private:
	SNAKE_TILE_TYPE**	mMap;

	Texture2D*			mSnakeHead;
	Texture2D*			mSnakeBody;
	Texture2D*			mSnakeTail;

	Texture2D*			mPickupTile;
	Texture2D*			mBlockedTile;
	Texture2D*			mEmptyTile;

	Texture2D*			mScoreImage;
	Texture2D*			mNumbersImage;
	unsigned int		mSingleNumberWidth;

	Vector2D			mPickupPosition;
	vector<Vector2D>	mSnake;

	MOVEMENT_DIRECTION	mCurrentMovementDirection;
	MOVEMENT_DIRECTION	mProposedMovementDirection;

	int					mDelay;
	int					mCurrentDelay;
	unsigned int		mCurrentNumberOfSegmentsToAdd;

	bool				mPause;
	int					mLevelTime;

	bool				mAIOn;
};

//--------------------------------------------------------------------------------------------------
#endif //_GAMESCREEN_SNAKE_H