//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "ObjectManager_Lemmings.h"
#include "Character_Lemmings.h"
#include "Landscape_Lemmings.h"
#include <stdlib.h>
#include "../Texture2D.h"
#include "Constants_Lemmings.h"
#include "UIManager_Lemmings.h"
#include <assert.h>

//Initialise the instance to null.
ObjectManager_Lemmings* ObjectManager_Lemmings::mInstance = NULL;

//--------------------------------------------------------------------------------------------------

ObjectManager_Lemmings::ObjectManager_Lemmings()
{

}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::Init(SDL_Renderer* renderer)
{
	mRenderer				 = renderer;

	mEntranceImage = new Texture2D(mRenderer);
	mEntranceImage->LoadFromFile("Images/Lemmings/Objects/Entrance.bmp");
	mEntrancePosition = Vector2D(Landscape_Lemmings::Instance()->mStartPosition.x - mEntranceImage->GetWidth() / 2, Landscape_Lemmings::Instance()->mStartPosition.y);

	mExitImage = new Texture2D(mRenderer);
	mExitImage->LoadFromFile("Images/Lemmings/Objects/Exit.bmp");
	mExitPosition = Vector2D(Landscape_Lemmings::Instance()->mEndPosition.x - mExitImage->GetWidth() / 2, Landscape_Lemmings::Instance()->mEndPosition.y - mExitImage->GetHeight());

	mCurrentNumberOfLemmings = 0;

	mSpawnDelay				 = kMinSpawnTime;
	mSpawnDuration			 = kInitialSpawnTime;
	mNumberOfDeadLemmings	 = 0;

	for (int i = 0; i < kNumberOfLemmings; i++)
		mLemmings[i] = new Character_Lemmings(mRenderer, "Images/Lemmings/Objects/Character.bmp", Vector2D(mEntrancePosition.x + mEntranceImage->GetWidth() / 3, mEntrancePosition.y), mCurrentNumberOfLemmings);
}

//--------------------------------------------------------------------------------------------------

ObjectManager_Lemmings::~ObjectManager_Lemmings()
{
	mRenderer = NULL;

	delete mEntranceImage;
	mEntranceImage = NULL;

	delete mExitImage;
	mExitImage = NULL;
}

//--------------------------------------------------------------------------------------------------

ObjectManager_Lemmings* ObjectManager_Lemmings::Instance()
{
	if (!mInstance)
	{
		mInstance = new ObjectManager_Lemmings;
	}

	return mInstance;
}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::Update(size_t deltaTime, SDL_Event e)
{
	//--------------------------------------------------------------------------------------------------
	//Character Updates.
	//--------------------------------------------------------------------------------------------------
	for (unsigned int i = 0; i < mCurrentNumberOfLemmings; i++)
	{
		if (mLemmings[i] != NULL && mLemmings[i]->IsAlive())
		{
			mLemmings[i]->Update(deltaTime, e);
		}
	}

	//--------------------------------------------------------------------------------------------------
	//Character spawns.
	//--------------------------------------------------------------------------------------------------
	mSpawnDelay -= deltaTime;
	if (mSpawnDelay <= 0.0f)
	{
		SpawnALemming();
		mSpawnDelay = mSpawnDuration;
	}

	//--------------------------------------------------------------------------------------------------
	//Character Collisions.
	//--------------------------------------------------------------------------------------------------
	for (unsigned int i = 0; i < mCurrentNumberOfLemmings; i++)
	{
		//Only BLOCKER types need to check.
		if(mLemmings[i]->HasSpawned() && mLemmings[i]->IsAlive())
		{
			if (mLemmings[i]->IsOfType(LEMMINGTYPE_BLOCKER))
			{
				for (unsigned int j = 0; j < mCurrentNumberOfLemmings; j++)
				{
					//Don't check against self.
					if (i != j)
					{
						if (mLemmings[j]->HasSpawned() && mLemmings[j]->IsAlive())
						{
							if (CollisionCircle(mLemmings[i]->GetCentralPosition(), 8.0f, mLemmings[j]->GetCentralPosition(), 8.0f))
							{
								//Was this lemming already colliding with BLOCKER lemming?
								if (mLemmings[j]->GetCollidingBlockerIndex() != i)
									mLemmings[j]->ForceChangeDirection();

								mLemmings[j]->SetIsCollidingWithBlocker(i);
							}
							else
							{
								//Was this lemming already colliding with BLOCKER lemming?
								if (mLemmings[j]->GetCollidingBlockerIndex() == i)
								{
									//No longer colliding, so reset colldingin blocker index.
									mLemmings[j]->SetIsCollidingWithBlocker(-1);
								}
							}
						}
					}
				}
			}
		}

		//Did this lemming reach the exit?
		if(!mLemmings[i]->IsInState(LEMMINGSTATE_EXIT))
		{
			int thirdTheWidthOfExit = (mExitImage->GetWidth() / 3) + 5;
			if (CollisionPointInBox(mLemmings[i]->GetCentralPosition(), Rect2D(mExitPosition.x + thirdTheWidthOfExit, mExitPosition.y, mExitImage->GetWidth() / 4, mExitImage->GetHeight())))
			{
				mLemmings[i]->SetState(LEMMINGSTATE_EXIT);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::Render()
{
	mEntranceImage->Render(mEntrancePosition);
	mExitImage->Render(mExitPosition);

	//--------------------------------------------------------------------------------------------------
	//Character Renders.
	//--------------------------------------------------------------------------------------------------
	for (unsigned int i = 0; i < mCurrentNumberOfLemmings; i++)
	{
		if (mLemmings[i] != NULL && mLemmings[i]->IsAlive())
			mLemmings[i]->Render();
	}
}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::SpawnALemming()
{
	//Find the next lemming in the array that is yet to be spawned.
	if (mCurrentNumberOfLemmings < kNumberOfLemmings)
	{
		for (int i = 0; i < kNumberOfLemmings; i++)
		{
			if (!mLemmings[i]->HasSpawned())
			{
				mLemmings[i]->Spawn();
				break;
			}
		}
		mCurrentNumberOfLemmings++;
	}
}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::ExplosionAt(Vector2D position)
{
	//Add a force to any Lemming within range.
	for(unsigned int i = 0; i < mCurrentNumberOfLemmings; i++)
	{
		if (mLemmings[i]->IsAlive() && !mLemmings[i]->InADeathState())
		{
			if (CollisionCircle(position, kExplosionRadius, mLemmings[i]->GetCentralPosition(), mLemmings[i]->GetCollisionRadius()))
			{
				Vector2D direction = position - mLemmings[i]->GetCentralPosition();
				direction.Normalize();
				mLemmings[i]->AddForce(Vector2D(-(direction.x*kExplosionStrength), -(direction.y*kExplosionStrength)));

				//Set this Lemming to be DEATH state.
				//mLemmings[i]->SetState(LEMMINGSTATE_DEATHEXPLOSION);
				mLemmings[i]->SetState(LEMMINGSTATE_DEATHFALL);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::DetonateAllLemmings()
{
	for (unsigned int i = 0; i < mCurrentNumberOfLemmings; i++)
	{
		if (mLemmings[i]->IsAlive() && !mLemmings[i]->InADeathState())
		{
			mLemmings[i]->SetType(LEMMINGTYPE_EXPLODER);
		}
	}
}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::DecreaseSpawnDuration()
{
	mSpawnDuration -= kSpawnTimeIncrement;
	mSpawnDuration = max(mSpawnDuration, kMinSpawnTime);

	if (mSpawnDelay > mSpawnDuration)
		mSpawnDelay = mSpawnDuration;
}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::IncreaseSpawnDuration()
{
	mSpawnDuration += kSpawnTimeIncrement;
	mSpawnDuration = min(mSpawnDuration, kMaxSpawnTime);

	if (mSpawnDelay < mSpawnDuration)
		mSpawnDelay = mSpawnDuration;
}

//--------------------------------------------------------------------------------------------------

void ObjectManager_Lemmings::RestartLevel()
{
	mCurrentNumberOfLemmings = 0;
	mNumberOfDeadLemmings	 = 0;
	mSpawnDelay				 = kMinSpawnTime;
	mSpawnDuration			 = kInitialSpawnTime;

	for (int i = 0; i < kNumberOfLemmings; i++)
	{
		if (mLemmings[i] != NULL)
			mLemmings[i]->ResetLemming(Vector2D(mEntrancePosition.x + mEntranceImage->GetWidth() / 3, mEntrancePosition.y));
	}
}

//--------------------------------------------------------------------------------------------------

bool ObjectManager_Lemmings::AllLemmingsHaveSpawned() 
{
	return kNumberOfLemmings == mCurrentNumberOfLemmings; 
}

//--------------------------------------------------------------------------------------------------

int  ObjectManager_Lemmings::GetNumberOfDeadLemmings() 
{ 
	return mNumberOfDeadLemmings; 
}

//--------------------------------------------------------------------------------------------------
