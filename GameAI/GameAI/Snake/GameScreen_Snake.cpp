//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "GameScreen_Snake.h"
#include <stdlib.h>
#include "../Texture2D.h"
#include "SnakeConstants.h"
#include "SnakeCommons.h"
#include "../VirtualJoypad.h"

//--------------------------------------------------------------------------------------------------

GameScreen_Snake::GameScreen_Snake(SDL_Renderer* renderer) : GameScreen(renderer)
{
	mMap = new SNAKE_TILE_TYPE*[kSnakePlayAreaWidth /kSnakeTileDimensions];
	for(int i =0; i < kSnakePlayAreaWidth /kSnakeTileDimensions; i++)
		mMap[i] = new SNAKE_TILE_TYPE[kSnakePlayAreaWidth/kSnakeTileDimensions];

	//Get all required textures.
	mBlockedTile = new Texture2D(renderer);
	mBlockedTile->LoadFromFile("Images/Snake/Board/BlockedTile.bmp");
	mEmptyTile = new Texture2D(renderer);
	mEmptyTile->LoadFromFile("Images/Snake/Board/EmptyTile.bmp");
	mPickupTile = new Texture2D(renderer);
	mPickupTile->LoadFromFile("Images/Snake/Board/Pickup.bmp");
	
	mSnakeBody = new Texture2D(renderer);
	mSnakeBody->LoadFromFile("Images/Snake/Board/Body.bmp");
	mSnakeHead = new Texture2D(renderer);
	mSnakeHead->LoadFromFile("Images/Snake/Board/Head.bmp");
	mSnakeTail = new Texture2D(renderer);
	mSnakeTail->LoadFromFile("Images/Snake/Board/Tail.bmp");

	mScoreImage = new Texture2D(renderer);
	mScoreImage->LoadFromFile("Images/Snake/UI/Score.bmp");
	mNumbersImage = new Texture2D(renderer);
	mNumbersImage->LoadFromFile("Images/Snake/UI/Numbers.bmp");
	mSingleNumberWidth = mNumbersImage->GetWidth() / 10;


	ResetMap();
	ResetSnake();

	mCurrentDelay		= kUpdateDelay;

	cout << "Press:" << endl;
	cout << "A - Turn AI on / off" << endl;
	cout << "SPACE - Pause / Unpause game" << endl;
	cout << "Arrow Keys - Direct Snake" << endl << endl;


	//Start game in frozen state.
	mPause = true;
	//With AI on.
	mAIOn = false;
}

//--------------------------------------------------------------------------------------------------

GameScreen_Snake::~GameScreen_Snake()
{
	delete mSnakeBody;
	mSnakeBody = NULL;

	delete mSnakeHead;
	mSnakeHead = NULL;

	delete mSnakeTail;
	mSnakeTail = NULL;

	delete mBlockedTile;
	mBlockedTile = NULL;

	delete mEmptyTile;
	mEmptyTile = NULL;

	delete mPickupTile;
	mPickupTile = NULL;

	delete mScoreImage;
	mScoreImage = NULL;

	delete mNumbersImage;
	mNumbersImage = NULL;

	for(int i =0; i < kSnakePlayAreaWidth/kSnakeTileDimensions; i++)
	{
		delete mMap[i];
		mMap[i] = NULL;
	}
	delete mMap;
	mMap = NULL;
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::Render()
{
	//UI
	mScoreImage->Render(Vector2D());

	string score = to_string(mSnake.size() - 2); //Take away the starting size.
	int xOffset = mScoreImage->GetWidth();
	int w;
	SDL_Rect portionOfSpritesheet, destRect;
	for (unsigned int i = 0; i < score.length(); i++)
	{
		char number = score[i];
		w = (int)mSingleNumberWidth*(atoi(&number) % 10);
		portionOfSpritesheet = { w, 0, (int)mSingleNumberWidth, (int)kSnakeScoreHeight };
		destRect = { xOffset, 2, (int)mSingleNumberWidth, (int)kSnakeScoreHeight-4 };

		mNumbersImage->Render(portionOfSpritesheet, destRect);

		xOffset += mSingleNumberWidth;

	}

	//Game board.
	for(int x = 0; x < kSnakePlayAreaWidth/kSnakeTileDimensions; x++)
	{
		for(int y = 0; y < kSnakePlayAreaHeight/kSnakeTileDimensions; y++)
		{
			switch(mMap[x][y])
			{
				case TYPE_BLOCKED:
					mBlockedTile->Render(Vector2D(x*kSnakeTileDimensions,(y*kSnakeTileDimensions)+kSnakeScoreHeight));
				break;
				
				case TYPE_EMPTY:
					mEmptyTile->Render(Vector2D(x*kSnakeTileDimensions,(y*kSnakeTileDimensions)+kSnakeScoreHeight));
				break;

				case TYPE_PICKUP:
					mPickupTile->Render(Vector2D(x*kSnakeTileDimensions, (y*kSnakeTileDimensions)+kSnakeScoreHeight));
				break;
			}
		}
	}

	RenderSnake();
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::Update(size_t deltaTime, SDL_Event e)
{
	switch(e.type)
	{
		case SDL_QUIT:
			exit(0);
		break;

		case SDL_KEYUP:
			switch(e.key.keysym.sym)
			{
				case SDLK_SPACE:
					mPause = !mPause;
				break;

				case SDLK_a:
					mAIOn = !mAIOn;
					if (mAIOn)
						cout << "AI on" << endl;
					else
						cout << "AI off" << endl;
				break;

				default:
				break;
			}
		break;
	
		default:
		break;
	}

	//--------------------------------------------------------------------------------------------------
	//Update the Virtual Joypad.
	//--------------------------------------------------------------------------------------------------
	VirtualJoypad::Instance()->SetJoypadState(e);
	
	if (!mPause)
	{
		if (mAIOn)
		{
			//TODO: Update SNAKE AI here.
		}
		else
			UpdateSnake(deltaTime, e);
		UpdateMap();
	}
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::UpdateMap()
{
	//Add the outer border to the map.
	for (int x = 0; x < kSnakePlayAreaWidth / kSnakeTileDimensions; x++)
	{
		for (int y = 0; y < kSnakePlayAreaHeight / kSnakeTileDimensions; y++)
		{
			if (x == 0 || y == 0 || x == (kSnakePlayAreaWidth / kSnakeTileDimensions) - 1 || y == (kSnakePlayAreaWidth / kSnakeTileDimensions) - 1)
				mMap[x][y] = TYPE_BLOCKED;
			else if(mMap[x][y] != TYPE_PICKUP)
				mMap[x][y] = TYPE_EMPTY;
		}
	}

	//Incorporate snake body (not including head)
	for (unsigned int i = 1; i < mSnake.size(); i++)
	{
		mMap[(int)mSnake[i].x][(int)mSnake[i].y] = TYPE_SNAKEBODY;
	}
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::ResetMap()
{
	for (int x = 0; x < kSnakePlayAreaWidth / kSnakeTileDimensions; x++)
	{
		for (int y = 0; y < kSnakePlayAreaHeight / kSnakeTileDimensions; y++)
		{
			if (x == 0 || y == 0 || x == (kSnakePlayAreaWidth/kSnakeTileDimensions)-1 || y == (kSnakePlayAreaWidth / kSnakeTileDimensions)-1)
				mMap[x][y] = TYPE_BLOCKED;
			else
				mMap[x][y] = TYPE_EMPTY;
		}
	}

	//Place the pickup in a random spot.
	ResetPickup();
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::ResetPickup()
{
	int x = 1+(rand() % ((kSnakePlayAreaWidth / kSnakeTileDimensions)-2));
	int y = 1+(rand() % ((kSnakePlayAreaHeight / kSnakeTileDimensions)-2));

	//Ensure it is not spawned under the tail.
	while (mMap[x][y] != TYPE_EMPTY)
	{
		x = 1 + (rand() % ((kSnakePlayAreaWidth / kSnakeTileDimensions) - 2));
		y = 1 + (rand() % ((kSnakePlayAreaHeight / kSnakeTileDimensions) - 2));
	};

	mMap[x][y] = TYPE_PICKUP;

	//Remove the old pick up.
	if(mMap[(int)(mPickupPosition.x / kSnakeTileDimensions)][(int)(mPickupPosition.y / kSnakeTileDimensions)] == TYPE_PICKUP)
		mMap[(int)(mPickupPosition.x/kSnakeTileDimensions)][(int)(mPickupPosition.y/kSnakeTileDimensions)] = TYPE_EMPTY;

	//Position used in NN to determine where aboouts the pckup is without searching the whole map.
	mPickupPosition = Vector2D(x*kSnakeTileDimensions, y*kSnakeTileDimensions);
}

//--------------------------------------------------------------------------------------------------
// Snake functions.
//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::ResetSnake()
{
	mLevelTime = kTimePerLevel;
	mCurrentNumberOfSegmentsToAdd = 1;
	mCurrentDelay = kUpdateDelay;
	mDelay = kUpdateDelay;

	mSnake.clear();

	//Snake always starts with 2 elements at the centre of the map.
	mSnake.push_back(Vector2D((kSnakePlayAreaWidth/kSnakeTileDimensions)*0.5f, (kSnakePlayAreaWidth/kSnakeTileDimensions)*0.5f));
	mSnake.push_back(Vector2D(mSnake[0].x, mSnake[0].y + 1));

	//Starts moving upwards.
	mCurrentMovementDirection = UP;
	mProposedMovementDirection = UP;

	//Clear the inputs from the virtual joypad.
	VirtualJoypad::Instance()->ResetJoypadState();
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::RenderSnake()
{
	//Draw in reverse so that the head will always be drawn last and therefore most visible.
	for (int i = mSnake.size()-1; i >= 0; i--)
	{
		if (i == 0)
		{
			mSnakeHead->Render(Vector2D(mSnake[i].x*kSnakeTileDimensions, (mSnake[i].y*kSnakeTileDimensions) + kSnakeScoreHeight));
		}
		else if (mSnake.size() >= 3 && i > mSnake.size() - 2)
		{
			mSnakeTail->Render(Vector2D(mSnake[i].x*kSnakeTileDimensions, (mSnake[i].y*kSnakeTileDimensions) + kSnakeScoreHeight));
		}
		else if (mSnake.size() >= 4 && i > mSnake.size() - 3)
		{
			mSnakeTail->Render(Vector2D(mSnake[i].x*kSnakeTileDimensions, (mSnake[i].y*kSnakeTileDimensions) + kSnakeScoreHeight));
		}
		else
		{
			mSnakeBody->Render(Vector2D(mSnake[i].x*kSnakeTileDimensions, (mSnake[i].y*kSnakeTileDimensions) + kSnakeScoreHeight));
		}
	}
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::UpdateSnake(size_t deltaTime, SDL_Event e)
{
	//React to the VIRTUAL JOYPAD input.
	if (VirtualJoypad::Instance()->UpArrow)
		mProposedMovementDirection = UP;
	else if (VirtualJoypad::Instance()->DownArrow)
		mProposedMovementDirection = DOWN;
	else if (VirtualJoypad::Instance()->LeftArrow)
		mProposedMovementDirection = LEFT;
	else if (VirtualJoypad::Instance()->RightArrow)
		mProposedMovementDirection = RIGHT;

	DoSnakeMovement(deltaTime);
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::DoSnakeMovement(size_t deltaTime)
{
	mCurrentDelay -= deltaTime;
	if (mCurrentDelay <= 0) //TEST
	{
		//Rest the delay.
		mCurrentDelay = mDelay;

		//Do not allow invalid movements - such as moving down when currently moving up.
		if (mCurrentMovementDirection == DOWN && mProposedMovementDirection == UP)
			mProposedMovementDirection = DOWN;
		else if (mCurrentMovementDirection == UP && mProposedMovementDirection == DOWN)
			mProposedMovementDirection = UP;
		else if (mCurrentMovementDirection == RIGHT && mProposedMovementDirection == LEFT)
			mProposedMovementDirection = RIGHT;
		else if (mCurrentMovementDirection == LEFT && mProposedMovementDirection == RIGHT)
			mProposedMovementDirection = LEFT;

		mCurrentMovementDirection = mProposedMovementDirection;
		//Do the actual movement.
		Vector2D headPos = mSnake.at(0);
		switch (mCurrentMovementDirection)
		{
			case UP:
				mSnake.insert(mSnake.begin(), Vector2D(headPos.x, headPos.y - 1));
			break;

			case DOWN:
				mSnake.insert(mSnake.begin(), Vector2D(headPos.x, headPos.y + 1));
			break;

			case LEFT:
				mSnake.insert(mSnake.begin(), Vector2D(headPos.x - 1, headPos.y));
			break;

			case RIGHT:
				mSnake.insert(mSnake.begin(), Vector2D(headPos.x + 1, headPos.y));
			break;

			default:
			break;
		}

		mSnake.erase(mSnake.begin() + mSnake.size() - 1);
	}

	if (mLevelTime <= 0)
		ResetGame();

	//--------------------------------------------------------------------------------------------------
	//Collisions
	//--------------------------------------------------------------------------------------------------
	switch (mMap[(int)mSnake[0].x][(int)mSnake[0].y])
	{
		case TYPE_BLOCKED:
		case TYPE_SNAKEBODY:
			ResetGame();
		break;

		case TYPE_PICKUP:
			AddToSnakeBody(mCurrentNumberOfSegmentsToAdd);
			mCurrentNumberOfSegmentsToAdd++;
			ResetPickup();
			IncreaseSpeed();
		break;

		default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::ResetGame()
{
	ResetMap();
	ResetSnake();
	mPause = true;
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::AddToSnakeBody(unsigned int numberOfSegments)
{
	for(unsigned int i = 0; i < numberOfSegments; i++)
		mSnake.push_back(Vector2D(mSnake[mSnake.size()-1].x, mSnake[mSnake.size()-1].y));
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Snake::IncreaseSpeed()
{
	mDelay = max(kMinUpdateDelay, mDelay-kDelayReductionAmount);
}

//--------------------------------------------------------------------------------------------------
