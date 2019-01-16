//------------------------------------------------------------------------
//  Author: Paul Roberts 2015
//------------------------------------------------------------------------

#include "GameScreenManager.h"
#include "GameScreen.h"
#include "Chess\GameScreen_Chess.h"
#include "Conway\GameScreen_Conway.h"
#include "NeuralNetworks_Clustering\GameScreen_Clustering.h"
#include "Lemmings\GameScreen_Lemmings.h"
#include "Snake\GameScreen_Snake.h"

//--------------------------------------------------------------------------------------------------

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	mRenderer = renderer;
	mCurrentScreen = NULL;

	//Ensure the first screen is set up.
	ChangeScreen(startScreen);
}

//--------------------------------------------------------------------------------------------------

GameScreenManager::~GameScreenManager()
{
	mRenderer = NULL;

	delete mCurrentScreen;
	mCurrentScreen = NULL;
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::Render()
{
	mCurrentScreen->Render();
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::Update(size_t deltaTime, SDL_Event e)
{
	mCurrentScreen->Update(deltaTime, e);
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	//Clear up the old screen.
	if(mCurrentScreen != NULL)
	{
		delete mCurrentScreen;
	}

	//Initialise the new screen.
	mCurrentScreenID = newScreen;
	switch(newScreen)
	{
		case SCREEN_INTRO:
		break;

		case SCREEN_MENU:
		break;

		case SCREEN_CHESS:
			mCurrentScreen = (GameScreen*)new GameScreen_Chess(mRenderer);
		break;
		
		case SCREEN_CONWAY:
			mCurrentScreen = (GameScreen*)new GameScreen_Conway(mRenderer);
		break;

		case SCREEN_CLUSTERING:
			mCurrentScreen = (GameScreen*)new GameScreen_Clustering(mRenderer);
		break;

		case SCREEN_LEMMINGS:
			mCurrentScreen = (GameScreen*)new GameScreen_Lemmings(mRenderer);
		break;

		case SCREEN_SNAKE:
			mCurrentScreen = (GameScreen*)new GameScreen_Snake(mRenderer);
		break;
		
		default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------