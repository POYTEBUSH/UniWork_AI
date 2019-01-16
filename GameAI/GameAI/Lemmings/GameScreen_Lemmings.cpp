//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------
//36x33 Worms image
#include "GameScreen_Lemmings.h"
#include "../GameScreen.h"
#include <stdlib.h>
#include "../Texture2D.h"
#include "Constants_Lemmings.h"
#include "Landscape_Lemmings.h"
#include "GameManager_Lemmings.h"
#include "UIManager_Lemmings.h"
#include "ObjectManager_Lemmings.h"
#include "Character_Lemmings.h"
#include "VirtualJoypad_Lemmings.h"
#include <assert.h>
#include <SDL_mixer.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "StateManager_Lemmings.h"

//--------------------------------------------------------------------------------------------------

GameScreen_Lemmings::GameScreen_Lemmings(SDL_Renderer* renderer) : GameScreen(renderer)
{
	mAIOn = false;

	//--------------------------------------------------------------------------------------------------
	//Singletons' Setup.
	//--------------------------------------------------------------------------------------------------
	auto LevelVal = 2;
	UIManager_Lemmings::Instance()->Init(renderer);
	GameManager_Lemmings::Instance()->Init(LevelVal);
	Landscape_Lemmings::Instance()->Init(renderer);
	ObjectManager_Lemmings::Instance()->Init(renderer);

	//Start game in frozen state.
	mPause = true;

	//Setup state Manager
	StateManager_Lemmings::Instance()->Init(LevelVal);

	cout << "Press:" << endl;
	cout << "A - Switch AI on" << endl;
	cout << "SPACE - Pause / Unpause" << endl;
	cout << "Mouse - Play game" << endl;
}

//--------------------------------------------------------------------------------------------------

GameScreen_Lemmings::~GameScreen_Lemmings()
{
	mRenderer = NULL;
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Lemmings::Render()
{
	//--------------------------------------------------------------------------------------------------
	//Landscape Renders.
	//--------------------------------------------------------------------------------------------------
	Landscape_Lemmings::Instance()->RenderDestructibleLand();

	ObjectManager_Lemmings::Instance()->Render();

	//--------------------------------------------------------------------------------------------------
	//Landscape Renders.
	//--------------------------------------------------------------------------------------------------
	//Landscape_Lemmings::Instance()->RenderSea();

	//--------------------------------------------------------------------------------------------------
	//UI Render.
	//--------------------------------------------------------------------------------------------------
	UIManager_Lemmings::Instance()->Render();
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Lemmings::Update(size_t deltaTime, SDL_Event e)
{
	//If time has run out, or all lemmings have spawned and have either escaped or died - Restart the level.
	if ((GameManager_Lemmings::Instance()->mLevelTime <= 0) ||
		(ObjectManager_Lemmings::Instance()->AllLemmingsHaveSpawned() && (kNumberOfLemmings - GameManager_Lemmings::Instance()->GetNumberOfEscapedLemmings() - ObjectManager_Lemmings::Instance()->GetNumberOfDeadLemmings() == 0)))
	{
		RestartLevel(GameManager_Lemmings::Instance()->mLevelID);
	}



	if (!mPause)
	{
		//--------------------------------------------------------------------------------------------------
		//Singletons' Updates.
		//--------------------------------------------------------------------------------------------------
		Landscape_Lemmings::Instance()->Update(deltaTime, e);
		GameManager_Lemmings::Instance()->Update(deltaTime, e);
		UIManager_Lemmings::Instance()->Update(deltaTime, e);
		ObjectManager_Lemmings::Instance()->Update(deltaTime, e);
		Landscape_Lemmings::Instance()->Update(deltaTime, e);

		if (mAIOn)
		{
			//TODO: Update your AI manager here.
			//mLemmingGene->GeneScore++;

			StateManager_Lemmings::Instance()->Update(deltaTime);
			HandlePlayerInput();
		}
	}

	//--------------------------------------------------------------------------------------------------
	//Player input only available if AI if off.
	//--------------------------------------------------------------------------------------------------
	if (!mAIOn)
	{
		VirtualJoypad_Lemmings::Instance()->SetInputState(e);
		HandlePlayerInput();
	}
	else
	{
	}

	//Additional input outside of player.
	switch (e.type)
	{
	case SDL_QUIT:
		exit(0);
		break;

	case SDL_KEYUP:
		switch (e.key.keysym.sym)
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
		}
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Lemmings::HandlePlayerInput()
{
	bool handledClick = false;

	//If clicked, then handle the position.
	if (VirtualJoypad_Lemmings::Instance()->Clicked)
	{
		if (!mPause)
		{
			if (VirtualJoypad_Lemmings::Instance()->MousePosition.y > kLemmingsPlayAreaHeight + kScoreHeight)
			{
				//Must be on the icon panel.
				int iconTypeOffset = IconPanelIndividualWidth * 2; //This caters for the + and - icons, which arent selections for lemming type.
				if (VirtualJoypad_Lemmings::Instance()->MousePosition.x < iconTypeOffset)
				{
					if (VirtualJoypad_Lemmings::Instance()->MousePosition.x < IconPanelIndividualWidth)
					{
						//Decrease spawn speed.
						ObjectManager_Lemmings::Instance()->IncreaseSpawnDuration();
					}
					else
					{
						//increase spawn speed.
						ObjectManager_Lemmings::Instance()->DecreaseSpawnDuration();
					}
				}
				else
				{
					handledClick = true;

					LEMMING_TYPE selection = LEMMINGTYPE_NORMAL;

					for (int i = 1; i < LEMMINGTYPE_MAX; i++)
					{
						if (VirtualJoypad_Lemmings::Instance()->MousePosition.x < iconTypeOffset + (IconPanelIndividualWidth*i))
						{
							selection = (LEMMING_TYPE)(i - 1);
							break;
						}
					}

					//Only change the selected icon if a valid icon was selected, otherwise stick with the previous selection.
					if (selection != LEMMINGTYPE_NORMAL)
						GameManager_Lemmings::Instance()->SetIconSelected(selection);
					else
					{
						if (VirtualJoypad_Lemmings::Instance()->MousePosition.x > (IconPanelIndividualWidth*(LEMMINGTYPE_MAX + 2)) &&
							VirtualJoypad_Lemmings::Instance()->MousePosition.x < iconTypeOffset + (IconPanelIndividualWidth*(LEMMINGTYPE_MAX + 3)))
						{
							//This is the detonate all icon.
							ObjectManager_Lemmings::Instance()->DetonateAllLemmings();
						}
					}
				}
			}
			else if (VirtualJoypad_Lemmings::Instance()->MousePosition.y < kLemmingsPlayAreaHeight)
			{
				handledClick = true;

				//First check if a lemming option has been selected from the icons.
				if (GameManager_Lemmings::Instance()->GetSelectedIcon() != LEMMINGTYPE_NORMAL)
				{
					//Must be clicking on a Lemming, but which one?
					for (unsigned int i = 0; i < ObjectManager_Lemmings::Instance()->mCurrentNumberOfLemmings; i++)
					{
						if (ObjectManager_Lemmings::Instance()->mLemmings[i]->IsAlive())
						{
							if (CollisionCircle(ObjectManager_Lemmings::Instance()->mLemmings[i]->GetCentralPosition(), ObjectManager_Lemmings::Instance()->mLemmings[i]->GetCollisionRadius(), VirtualJoypad_Lemmings::Instance()->MousePosition, 10.0f))
							{
								//Only try to use an icon if this lemming is not of this type.
								if (!ObjectManager_Lemmings::Instance()->mLemmings[i]->IsOfType(GameManager_Lemmings::Instance()->GetSelectedIcon()) ||
									GameManager_Lemmings::Instance()->GetSelectedIcon() == LEMMINGTYPE_EXPLODER)
								{
									bool alreadySelfDetonating = ObjectManager_Lemmings::Instance()->mLemmings[i]->IsSelfDetonating();

									ObjectManager_Lemmings::Instance()->mLemmings[i]->SetType(GameManager_Lemmings::Instance()->GetSelectedIcon());

									//Only use an icon if we are now the type the player chose.
									if (GameManager_Lemmings::Instance()->GetSelectedIcon() == ObjectManager_Lemmings::Instance()->mLemmings[i]->GetType() ||
										(GameManager_Lemmings::Instance()->GetSelectedIcon() == LEMMINGTYPE_EXPLODER && !alreadySelfDetonating))
										GameManager_Lemmings::Instance()->UseAnIcon();

									break;
								}
							}
						}
					}
				}
			}
		}

		//The player can click to unpause the game whn the game is paused.
		if (VirtualJoypad_Lemmings::Instance()->MousePosition.y > kLemmingsPlayAreaHeight + kScoreHeight)
		{
			if (VirtualJoypad_Lemmings::Instance()->MousePosition.x > (IconPanelIndividualWidth*(LEMMINGTYPE_MAX + 1)) &&
				VirtualJoypad_Lemmings::Instance()->MousePosition.x < (IconPanelIndividualWidth*(LEMMINGTYPE_MAX + 2))) //+2 to include + and - icons.
			{
				//Unpause the game.
				mPause = !mPause;
				handledClick = true;
			}
		}

		if (handledClick)
			VirtualJoypad_Lemmings::Instance()->ClickedHandled();
	}
}

//--------------------------------------------------------------------------------------------------

void GameScreen_Lemmings::RestartLevel(int level)
{
		//--------------------------------------------------------------------------------------------------
		//Singletons' Restart.
		//--------------------------------------------------------------------------------------------------
		//UIManager_Lemmings::Instance()->RestartLevel();
		GameManager_Lemmings::Instance()->RestartLevel();
		Landscape_Lemmings::Instance()->RestartLevel();
		ObjectManager_Lemmings::Instance()->RestartLevel();
		StateManager_Lemmings::Instance()->RestartLevel();

		

		//Music.
	
}

//--------------------------------------------------------------------------------------------------
