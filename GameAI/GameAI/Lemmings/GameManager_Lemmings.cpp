//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "GameManager_Lemmings.h"
#include <stdlib.h>
#include "../Texture2D.h"
#include "Constants_Lemmings.h"
#include "UIManager_Lemmings.h"
#include <assert.h>

//Initialise the instance to null.
GameManager_Lemmings* GameManager_Lemmings::mInstance = NULL;

//--------------------------------------------------------------------------------------------------

GameManager_Lemmings::GameManager_Lemmings()
{

}

//--------------------------------------------------------------------------------------------------

void GameManager_Lemmings::Init(int level)
{
	mLevelID = level;
	RestartLevel();
}

//--------------------------------------------------------------------------------------------------

void GameManager_Lemmings::RestartLevel()
{
	//Set counts for Icons.
	switch (mLevelID)
	{
		case 1:
			mIconQuantities[LEMMINGTYPE_CLIMBER]	= 0;
			mIconQuantities[LEMMINGTYPE_PARACHUTE]	= 0;
			mIconQuantities[LEMMINGTYPE_EXPLODER]	= 0;
			mIconQuantities[LEMMINGTYPE_BLOCKER]	= 0;
			mIconQuantities[LEMMINGTYPE_BUILDER]	= 0;
			mIconQuantities[LEMMINGTYPE_PUNCHER]	= 0;
			mIconQuantities[LEMMINGTYPE_PICKAXE]	= 0;
			mIconQuantities[LEMMINGTYPE_DIGGER]		= 10;
			mLevelTime								= 60000;
		break;

		case 2:
			mIconQuantities[LEMMINGTYPE_CLIMBER]	= 0;
			mIconQuantities[LEMMINGTYPE_PARACHUTE]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_EXPLODER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_BLOCKER]	= 10;
			mIconQuantities[LEMMINGTYPE_BUILDER]	= 0;
			mIconQuantities[LEMMINGTYPE_PUNCHER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_PICKAXE]	= 0;
			mIconQuantities[LEMMINGTYPE_DIGGER]		= 0;
			mLevelTime								= 60000;
		break;

		case 3:
			mIconQuantities[LEMMINGTYPE_CLIMBER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_PARACHUTE]	= 0;
			mIconQuantities[LEMMINGTYPE_EXPLODER]	= 10;
			mIconQuantities[LEMMINGTYPE_BLOCKER]	= 10;
			mIconQuantities[LEMMINGTYPE_BUILDER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_PUNCHER]	= 10;
			mIconQuantities[LEMMINGTYPE_PICKAXE]	= 10;
			mIconQuantities[LEMMINGTYPE_DIGGER]		= 10;
			mLevelTime								= 80000;
		break;

		case 4:
			mIconQuantities[LEMMINGTYPE_CLIMBER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_PARACHUTE]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_EXPLODER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_BLOCKER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_BUILDER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_PUNCHER]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_PICKAXE]	= kNumberOfLemmings;
			mIconQuantities[LEMMINGTYPE_DIGGER]		= kNumberOfLemmings;
			mLevelTime = 80000;
		break;
	}

	//LEMMINGTYPE_NORMAL treated as a none selection as far as UI is concerned.
	mSelectedIcon	 = LEMMINGTYPE_NORMAL;
	mEscapedLemmings = 0;
}

//--------------------------------------------------------------------------------------------------

GameManager_Lemmings::~GameManager_Lemmings()
{
}

//--------------------------------------------------------------------------------------------------

GameManager_Lemmings* GameManager_Lemmings::Instance()
{
	if (!mInstance)
	{
		mInstance = new GameManager_Lemmings;
	}

	return mInstance;
}

//--------------------------------------------------------------------------------------------------

void GameManager_Lemmings::Update(size_t deltaTime, SDL_Event e)
{
	//Does the highlight need to be cleared? Happens when all of an icon has been used up.
	if (mSelectedIcon != LEMMINGTYPE_NORMAL)
	{
		if (mIconQuantities[mSelectedIcon] <= 0)
		{
			mSelectedIcon = LEMMINGTYPE_NORMAL;
		}
	}

	//Time keeps on a ticking.
	mLevelTime -= deltaTime;
	mLevelTime = max(mLevelTime, 0);
}

//--------------------------------------------------------------------------------------------------

unsigned int GameManager_Lemmings::GetNumberOfEscapedLemmings()
{
	return mEscapedLemmings; 
}

//--------------------------------------------------------------------------------------------------