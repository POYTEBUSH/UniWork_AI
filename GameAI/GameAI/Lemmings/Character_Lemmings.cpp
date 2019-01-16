//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "Character_Lemmings.h"
#include "PhysicsObject_Lemmings.h"
#include "Constants_Lemmings.h"
#include "../Texture2D.h"
#include "Landscape_Lemmings.h"
#include "GameManager_Lemmings.h"
#include "VirtualJoypad_Lemmings.h"
#include "ObjectManager_Lemmings.h"

using namespace std;

const int kSpritesOnSheetHorizontal = 8;
const int kSpritesOnSheetVertical   = 26;
PhysicsObjectSetupDetails_Lemmings LemmingPhysicsDetails(Vector2D(0.0, 1.0), 0.0, 1.0f, 0.0f, 1.0f);

//--------------------------------------------------------------------------------------------------

Character_Lemmings::Character_Lemmings(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, int lemmingIndex) : PhysicsObject_Lemmings(renderer, startPosition, LemmingPhysicsDetails)
{
	mRenderer = renderer;

	mTexture = new Texture2D(mRenderer);
	if( !mTexture->LoadFromFile(imagePath.c_str()) )
	{
		cout << "Failed to load texture: " << imagePath << endl;
		return;
	}

	//Self detonation numbers.
	mWhiteNumbers = new Texture2D(mRenderer);
	mWhiteNumbers->LoadFromFile("Images/Lemmings/UI/Countdown.bmp");

	mSingleSpriteWidth = (float)mTexture->GetWidth() / (float)kSpritesOnSheetHorizontal;	//8 sprites columns
	mSingleSpriteHeight = (float)mTexture->GetHeight() / (float)kSpritesOnSheetVertical;	//16 sprite rows
	mCollisionRadius = 8.0f;

	mLemmingIndex = lemmingIndex;
	
	ResetLemming(startPosition);
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::ResetLemming(Vector2D startPosition)
{
	mPosition				= startPosition;

	mCollidingBlockerIndex	= -1;

	mFacingDirection		= FACINGDIRECTION_LEFT;
	mIsAlive				= true;
	mSpawned				= false;
	mExploded				= false;
	mPlacedPlatform			= false;
	mSelfDetonation			= false;
	mDetonationCount		= kDetonationDuration;

	mVelocity.Zero();
	mLemmingType = LEMMINGTYPE_NORMAL;
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Spawn()
{ 
	mLemmingType = LEMMINGTYPE_NORMAL;

	mVelocity.Zero();
	mSpawned = true; 

	mState = LEMMINGSTATE_WALK; //Force the change.
	SetState(LEMMINGSTATE_FALL);
}

//--------------------------------------------------------------------------------------------------

Vector2D Character_Lemmings::GetCentralPosition()
{
	return Vector2D((int)(mPosition.x + mSingleSpriteWidth*0.5f), (int)(mPosition.y + mSingleSpriteHeight*0.5f));
}

//--------------------------------------------------------------------------------------------------

Character_Lemmings::~Character_Lemmings()
{
	mRenderer = NULL;

	delete mTexture;
	mTexture = NULL;

	delete mWhiteNumbers;
	mWhiteNumbers = NULL;
}

//--------------------------------------------------------------------------------------------------

Rect2D Character_Lemmings::GetCollisionBox()
{ 
	return Rect2D(mPosition.x, mPosition.y, mSingleSpriteWidth, mSingleSpriteHeight);
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Render()
{
	int w = (int)mSingleSpriteWidth*(mCurrentFrame % kSpritesOnSheetHorizontal);
	int h = (int)mSingleSpriteHeight*(mCurrentFrame / kSpritesOnSheetHorizontal);

#	//Get the portion of the spritesheet you want to draw.
	//								{XPos, YPos, WidthOfSingleSprite, HeightOfSingleSprite}
	SDL_Rect portionOfSpritesheet = { w, h, (int)mSingleSpriteWidth, (int)mSingleSpriteHeight };

	//Determine where you want it drawn.
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), (int)mSingleSpriteWidth, (int)mSingleSpriteHeight };

	//Then draw it.
	if (mFacingDirection == FACINGDIRECTION_LEFT)
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	else
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_HORIZONTAL);

	//Draw detonation numbers above Lemmings who are self detonation.
	if (mSelfDetonation && mDetonationCount >= 0)
	{
		int numPos = (int)(((float)mDetonationCount / (float)kDetonationDuration)*kDetonationDurationInSeconds);
		SDL_Rect portionOfSpritesheet;
		SDL_Rect destRect;
		portionOfSpritesheet = { numPos*(int)(mWhiteNumbers->GetWidth() / 5), 0, (int)mWhiteNumbers->GetWidth() / 5, (int)mWhiteNumbers->GetHeight() };
		destRect = { (int)GetCentralPosition().x-5, (int)mPosition.y-5, (int)mWhiteNumbers->GetWidth() / 5, (int)mWhiteNumbers->GetHeight() };
		mWhiteNumbers->Render(portionOfSpritesheet, destRect);
	}

}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Update(size_t deltaTime, SDL_Event e)
{
	if (mSpawned)
	{
		if (mIsAlive)
		{
			if (mPosition.x < 0 || mPosition.x >= kLemmingsPlayAreaWidth || mPosition.y < 0 || mPosition.y >= kLemmingsPlayAreaHeight)
			{
				mIsAlive = false;
				ObjectManager_Lemmings::Instance()->AddADeadLemming();

				return;
			}

			//Does this Lemming have the 5 second count above its head?
			if (mSelfDetonation)
			{
				mDetonationCount -= deltaTime;
				if (mDetonationCount <= 0)
					SetState(LEMMINGSTATE_DEATHEXPLOSION);
			}

			UpdateAnimations(deltaTime);

			switch (mState)
			{
				case LEMMINGSTATE_WALK:
					if (!IsOfType(LEMMINGTYPE_BLOCKER))
					{
						if (mFacingDirection == FACINGDIRECTION_LEFT)
							Walk(deltaTime, -kLemmingWalkSpeed);
						else if (mFacingDirection == FACINGDIRECTION_RIGHT)
							Walk(deltaTime, kLemmingWalkSpeed);
					}
				break;

				case LEMMINGSTATE_DIG:
					Dig(deltaTime);
				break;

				case LEMMINGSTATE_CLIMB:
					Climb(deltaTime, kLemmingClimbSpeed);
					return;
				break;

				case LEMMINGSTATE_CLIMBEXIT:
					ClimbExit(deltaTime, kLemmingClimbExitSpeed);
					return;
				break;

				case LEMMINGSTATE_BUILD:
					Build(deltaTime);
					return;
				break;

				case LEMMINGSTATE_PUNCH:
					if (mFacingDirection == FACINGDIRECTION_LEFT)
						Punch(deltaTime, -kLemmingPunchXMovement);
					else if (mFacingDirection == FACINGDIRECTION_RIGHT)
						Punch(deltaTime, kLemmingPunchXMovement);
				break;

				case LEMMINGSTATE_PICKAXE:
					if (mFacingDirection == FACINGDIRECTION_LEFT)
						PickAxe(deltaTime, -kLemmingPickAxeXMovement);
					else if (mFacingDirection == FACINGDIRECTION_RIGHT)
						PickAxe(deltaTime, kLemmingPickAxeXMovement);
				break;

				case LEMMINGSTATE_FALL:
					//Some anti-gravity to slow down the fall.
					if (IsOfType(LEMMINGTYPE_PARACHUTE))
						AddForce(Vector2D(0.0f, kParachuteAntiGravityForce));
				break;

				case LEMMINGSTATE_DEATHFALL:
					GeneralDeath(deltaTime);
					if (mCurrentFrame >= mStartFrame + 4)							//Exit update() if sprite is now just smoke.
						return;
				break;

				case LEMMINGSTATE_DEATHEXPLOSION:
					DetonationDeath(deltaTime);
					if (mCurrentFrame >= mStartFrame + 4)							//Exit update() if sprite is now just smoke.
						return;
				break;

				case LEMMINGSTATE_DEATHDROWN:
					GeneralDeath(deltaTime);
					if (mCurrentFrame >= mStartFrame + 4)							//Exit update() if sprite is now just smoke.
						return;
				break;

				case LEMMINGSTATE_EXIT:
					Escape(deltaTime);
					return;
				break;

				default:
				break;
			}

			//Collision position variables.
			int headPosition = (int)mPosition.y;
			int footPosition = (int)(mPosition.y + mSingleSpriteHeight) + 1;
			int centralXPos = (int)(mPosition.x + mSingleSpriteWidth / 2);

			//-----------------------------------------------------------------------------
			//Collided with ground so we can jump again.
			//-----------------------------------------------------------------------------
			if (centralXPos >= 0 && centralXPos < kLemmingsPlayAreaWidth && footPosition >= 0 && footPosition < kLemmingsPlayAreaHeight)
			{
				//cout << "Checking (" << centralXPos << "," << footPosition << ")" << endl;
				if (Landscape_Lemmings::Instance()->GetGroundTypeAt(centralXPos, footPosition) == LANDSCAPETYPE_GROUND)
				{
					if (!InADeathState())
					{
						if (mVelocity.y > kLemmingDeathFallSpeed && !IsOfType(LEMMINGTYPE_PARACHUTE))
							SetState(LEMMINGSTATE_DEATHFALL);
						else
						{
							if (!IsInActionState())
								SetState(LEMMINGSTATE_WALK);
						}
					}

					HitGround();
				}
			}

			//Have we picked up enough velocity to be classed as in a fall?
			if (!InADeathState())
			{
				if (mVelocity.y > kLemmingFallSpeed)
					SetState(LEMMINGSTATE_FALL);

			}
			//Always allow PhysicsObject::Update() to take effect.
			PhysicsObject_Lemmings::Update(deltaTime, e);

			//Rebound off ground when dead.
			if (InADeathState())
			{
				if (GetCentralPosition().x >= 0 && GetCentralPosition().x < kLemmingsPlayAreaWidth && GetCentralPosition().y >= 0 && GetCentralPosition().y < kLemmingsPlayAreaHeight)
				{
					if (Landscape_Lemmings::Instance()->GetGroundTypeAt(GetCentralPosition()) == LANDSCAPETYPE_GROUND)
						mVelocity *= -1.0f;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::UpdateAnimations(size_t deltaTime)
{
	//Change frame?
	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f)
	{
		//Reset frame delay count.
		mFrameDelay = kAnimationDelay_Lemmings;

		//Move frame on.
		mCurrentFrame++;

		//Loop frame around if it goes beyond the number of frames.
		if (mCurrentFrame > mEndFrame)
		{
			if (IsAlive())
				mCurrentFrame = mStartFrame;
		}
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::ForceChangeDirection()
{
	if (mFacingDirection == FACINGDIRECTION_LEFT)
		mFacingDirection = FACINGDIRECTION_RIGHT;
	else if (mFacingDirection == FACINGDIRECTION_RIGHT)
		mFacingDirection = FACINGDIRECTION_LEFT;
}
//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Walk(size_t deltaTime, float speed)
{
	if (speed < 0.0f)
		mFacingDirection = FACINGDIRECTION_LEFT;
	else
		mFacingDirection = FACINGDIRECTION_RIGHT;

	float	movement     = (speed * deltaTime);
	double	newXPos      = mPosition.x + movement;
	int		centralXPos  = (int)(newXPos + mSingleSpriteWidth / 2);
	int		footPosition = (int)(mPosition.y + mSingleSpriteHeight);

	if (centralXPos >= 0 && centralXPos < kLemmingsPlayAreaWidth && footPosition >= 0 && footPosition < kLemmingsPlayAreaHeight)
	{
		//Check the next position.
		if (Landscape_Lemmings::Instance()->GetGroundTypeAt(centralXPos, footPosition) == LANDSCAPETYPE_EMPTY)
		{
			SetPosition(Vector2D(newXPos, mPosition.y));
			return;
		}
		else
		{
			//Blocked, how about up to 10 positions above it?
			for (int i = 0; i < 10; i++)
			{
				if (Landscape_Lemmings::Instance()->GetGroundTypeAt(centralXPos, footPosition - i) == LANDSCAPETYPE_EMPTY)
				{
					SetPosition(Vector2D(newXPos, mPosition.y - i));
					return;
				}
			}
		}

		//Otherwise, first check if this guy is a CLIMBER.
		if (mLemmingType == LEMMINGTYPE_CLIMBER)
		{
			SetState(LEMMINGSTATE_CLIMB);
		}
		else
		{
			//If not, just turn the little guy around.
			if (mFacingDirection == FACINGDIRECTION_LEFT)
				mFacingDirection = FACINGDIRECTION_RIGHT;
			else
				mFacingDirection = FACINGDIRECTION_LEFT;
		}
	}
	else
	{
		SetPosition(Vector2D(newXPos, (int)mPosition.y));
		SetState(LEMMINGSTATE_FALL);
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Dig(size_t deltaTime)
{
	if ((mCurrentFrame == mStartFrame + 4) || (mCurrentFrame == mEndFrame))
	{
		int centralXPos = (int)(mPosition.x + mSingleSpriteWidth / 2);
		int footPosition = (int)(mPosition.y + mSingleSpriteHeight) + 2;

		//Use mExploded to allow only one modification of the land per time we are in this frame.
		if (!mExploded)
			Landscape_Lemmings::Instance()->DigLand(centralXPos, footPosition);
	}
	else
		mExploded = false;

	//FUDGE FIX: Lemming whilst digging gets blown sideways from explosion. Investigate later.
	mVelocity.x = 0.0f;
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::GeneralDeath(size_t deltaTime)
{
	if (!mExploded && mCurrentFrame == mStartFrame + 4)
	{
		mExploded = true;
		Landscape_Lemmings::Instance()->RemoveLandForExplosion((int)(mPosition.x + mSingleSpriteWidth / 2), (int)(mPosition.y + mSingleSpriteHeight));
	}

	if (mCurrentFrame == mEndFrame)
	{
		ObjectManager_Lemmings::Instance()->AddADeadLemming();
		mIsAlive = false;
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::DetonationDeath(size_t deltaTime)
{
	if (!mExploded && mCurrentFrame == mStartFrame + 19)
	{
		mExploded = true;
		Landscape_Lemmings::Instance()->RemoveLandForExplosion((int)(mPosition.x + mSingleSpriteWidth / 2), (int)(mPosition.y + mSingleSpriteHeight));
	}

	if (mCurrentFrame == mEndFrame)
	{
		ObjectManager_Lemmings::Instance()->AddADeadLemming();
		mIsAlive = false;
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Escape(size_t deltaTime)
{
	if (mCurrentFrame == mEndFrame)
	{
		GameManager_Lemmings::Instance()->AddEscapedLemming();
		mIsAlive = false;
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Climb(size_t deltaTime, float speed)
{
	double movement   = (speed * deltaTime);
	double newYPos	  = mPosition.y + movement;
	double xPos		  = mPosition.x;

	//We need to reposition the xPos for checking wall collisions to be on the right of facing RIGHT.
	if (mFacingDirection == FACINGDIRECTION_RIGHT)
		xPos += mSingleSpriteWidth;

	int headPosition = (int)(mPosition.y);

	if (GetCentralPosition().x >= 0 && GetCentralPosition().x < kLemmingsPlayAreaWidth && headPosition >= 0 && headPosition < kLemmingsPlayAreaHeight)
	{
		//Check the next position.
		//Look for an EMPTY position above, and GROUND to the LEFT / RIGHT (depends which way we are facing).
		if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(GetCentralPosition().x, newYPos)) == LANDSCAPETYPE_EMPTY)
		{
			for (int i = 0; i < 10; i++)
			{
				if (mFacingDirection == FACINGDIRECTION_LEFT)
				{
					if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos - i, headPosition)) == LANDSCAPETYPE_GROUND)
					{
						SetPosition(Vector2D(xPos - i, newYPos));
						return;
					}
				}
				else
				{
					//Must be facing RIGHT.
					if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos + i, headPosition)) == LANDSCAPETYPE_GROUND)
					{
						//Remember to put the draw position back over to the left of the sprite.
						SetPosition(Vector2D((xPos-mSingleSpriteWidth) + i, newYPos));
						return;
					}
				}
			}

			//EMPTY in front, and EMPTY above, so climb up.
			SetState(LEMMINGSTATE_CLIMBEXIT);
		}
		else
		{
			//Try and exit the climb.
			SetState(LEMMINGSTATE_FALL);

			//We don't want him to fall to the ground and climb back up the same wall.
			ForceChangeDirection();
		}

	}
	else
	{
		//Try and exit the climb.
		SetState(LEMMINGSTATE_CLIMBEXIT);
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::ClimbExit(size_t deltaTime, float speed)
{
	double movement		= (speed * deltaTime);
	double newYPos		= mPosition.y + movement;

	double xPos			= mPosition.x;
	//We need to reposition the xPos for checking wall collisions to be on the right of facing RIGHT.
	if (mFacingDirection == FACINGDIRECTION_RIGHT)
		xPos += mSingleSpriteWidth;

	if (GetCentralPosition().x >= 0 && GetCentralPosition().x < kLemmingsPlayAreaWidth && mPosition.y >= 0 && mPosition.y < kLemmingsPlayAreaHeight)
	{
		if (Landscape_Lemmings::Instance()->GetGroundTypeAt(GetCentralPosition()) == LANDSCAPETYPE_EMPTY)
		{
			//Enforce an upwards movement.
			SetPosition(Vector2D(mPosition.x, newYPos));
		}
		else
		{
			//Failed, so fall.
			SetState(LEMMINGSTATE_FALL);
			//We don't want him to fall to the ground and climb back up the same wall.
			ForceChangeDirection();
		}

		//Has the animation finished.
		if (mCurrentFrame == mEndFrame)
		{
			//Completed the climb, so go into fall state, which when landing will result in the walk state.
			SetState(LEMMINGSTATE_FALL);
			if (mFacingDirection == FACINGDIRECTION_LEFT)
				AddForce(Vector2D(-kLemmingJumpSpeed, 0.0f));
			else
				AddForce(Vector2D(kLemmingJumpSpeed, 0.0f));
		}
	}
	else
	{
		//Failed, so fall.
		SetState(LEMMINGSTATE_FALL);
		//We don't want him to fall to the ground and climb back up the same wall.
		ForceChangeDirection();
	}
}


//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Build(size_t deltaTime)
{
	mTypeDuration -= deltaTime;

	//Do not allow gravity to effect this lemming.
	//AddForce(Vector2D(0.0f, -kGravity_Lemmings));

	if (mCurrentFrame == mEndFrame)
	{
		if (!mPlacedPlatform)
		{
			int platformHeight = 2;
			int platformWidth  = 12;
			if(mFacingDirection == FACINGDIRECTION_LEFT)
				Landscape_Lemmings::Instance()->AddLandForPlatform((int)mPosition.x, (int)(mPosition.y + mSingleSpriteHeight - platformHeight));
			else
				Landscape_Lemmings::Instance()->AddLandForPlatform((int)(mPosition.x+platformWidth), (int)(mPosition.y + mSingleSpriteHeight - platformHeight));

			mPlacedPlatform = true;

			//Reposition the lemming.
			if (mFacingDirection == FACINGDIRECTION_LEFT)
				mPosition.x -= mSingleSpriteWidth / 4;
			else
				mPosition.x += mSingleSpriteWidth / 4;
			mPosition.y -= platformHeight;

			//When time has run out, allow the last platform to be placed, then revert
			//back to being a NORMAL lemming.
			if (mTypeDuration <= 0.0f)
			{
				SetType(LEMMINGTYPE_NORMAL);
				SetState(LEMMINGSTATE_WALK);

				//Cancel any physics shinanegans.
				HitGround();

				//Lemming always walks back down his bridge.
				ForceChangeDirection();
			}
		}
	}
	else
	{
		//We may hit a wall, if so turn the little fella around.
		double footPosition	= mPosition.y + mSingleSpriteHeight/2.0f;
		double xPos		    = mPosition.x + mSingleSpriteWidth / 4.0f;

		//We need to reposition the xPos for checking wall collisions to be on the right of facing RIGHT.
		if (mFacingDirection == FACINGDIRECTION_RIGHT)
			xPos += mSingleSpriteWidth/2.0f;


		if (xPos >= 0 && xPos < kLemmingsPlayAreaWidth && footPosition >= 0 && footPosition < kLemmingsPlayAreaHeight)
		{
			//Check the next position.
			if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos, footPosition)) == LANDSCAPETYPE_GROUND)
				ForceChangeDirection();
		}
		
		mPlacedPlatform = false;
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::Punch(size_t deltaTime, float xMovement)
{
	double newXPos	   = mPosition.x + xMovement;
	double xPos		   = mPosition.x;
	double centralYPos = mPosition.y + (mSingleSpriteHeight / 2.0f);

	//We need to reposition the xPos for checking wall collisions to be on the right of facing RIGHT.
	if (mFacingDirection == FACINGDIRECTION_RIGHT)
		xPos += mSingleSpriteWidth;

	if ((mCurrentFrame == mStartFrame + 4) || (mCurrentFrame == mStartFrame + 19))
	{
		//Use mExploded to allow only one modification of the land per time we are in this frame.
		if (!mExploded)
		{
			Landscape_Lemmings::Instance()->PunchLand((int)xPos, (int)(mPosition.y+mSingleSpriteHeight-10.0f)); //-10.0f is half of the PunchExplosions texture.
			SetPosition(Vector2D(newXPos, mPosition.y));
		}
	}
	else
		mExploded = false;

	if (xPos >= 0 && xPos < kLemmingsPlayAreaWidth && centralYPos >= 0 && centralYPos < kLemmingsPlayAreaHeight)
	{
		//Need to keep punching if there is any ground in the next 10 pixels.
		for (int i = 0; i < 10; i++)
		{
			if (mFacingDirection == FACINGDIRECTION_LEFT)
			{
				if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos - i, centralYPos)) == LANDSCAPETYPE_GROUND)
					return;
			}
			else //Must be facing right.
			{
				if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos + i, centralYPos)) == LANDSCAPETYPE_GROUND)
					return;
			}
		}

		if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos, centralYPos)) == LANDSCAPETYPE_EMPTY)
		{
			SetState(LEMMINGSTATE_WALK);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::PickAxe(size_t deltaTime, float xMovement)
{
	double newXPos	= mPosition.x + xMovement;
	double xPos		= mPosition.x;
	double yPos		= mPosition.y + mSingleSpriteHeight + 1;

	//We need to reposition the xPos for checking wall collisions to be on the right of facing RIGHT.
	if (mFacingDirection == FACINGDIRECTION_RIGHT)
		xPos += mSingleSpriteWidth;

	if (mCurrentFrame == mStartFrame + 4)
	{
		//Use mExploded to allow only one modification of the land per time we are in this frame.
		if (!mExploded)
		{
			if (mFacingDirection == FACINGDIRECTION_LEFT)
				Landscape_Lemmings::Instance()->PickAxeLand((int)(mPosition.x+(mSingleSpriteWidth/3.0f)), (int)(yPos-5)); //-5.0f just raises the xplosion a little.
			else
				Landscape_Lemmings::Instance()->PickAxeLand((int)(mPosition.x + ((mSingleSpriteWidth / 3.0f)*2.0f)), (int)(yPos - 5)); //-5.0f just raises the xplosion a little.

			SetPosition(Vector2D(newXPos, mPosition.y));
		}
	}
	else
		mExploded = false;

	if (xPos >= 0 && xPos < kLemmingsPlayAreaWidth && yPos >= 0 && yPos < kLemmingsPlayAreaHeight)
	{
		//Need to keep punching if there is any ground in the next 10 pixels.
		for (int i = 0; i < 10; i++)
		{
			if (mFacingDirection == FACINGDIRECTION_LEFT)
			{
				if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos - i, yPos)) == LANDSCAPETYPE_GROUND)
					return;
			}
			else //Must be facing right.
			{
				if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos + i, yPos)) == LANDSCAPETYPE_GROUND)
					return;
			}
		}

/*		if (Landscape_Lemmings::Instance()->GetGroundTypeAt(Vector2D(xPos, yPos)) == LANDSCAPETYPE_EMPTY)
		{
			SetState(LEMMINGSTATE_WALK);
			return;
		}*/
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::SetState(LEMMING_STATE newState)
{
	if (mState != newState)
	{
		if (newState != LEMMINGSTATE_DEATHCOUNTDOWN)
		{
			mState = newState;

			//Remove previous forces, apart from fall. Looks silly to stop mid air.
			if(newState != LEMMINGSTATE_FALL)
				ClearVelocity();

			//Reset frame delay count.
			mFrameDelay = kAnimationDelay_Lemmings;
		}

		switch (newState)
		{
			case LEMMINGSTATE_WALK:
				if (!IsOfType(LEMMINGTYPE_BLOCKER))
				{
					mStartFrame = 0;
					mCurrentFrame = 0;
					mEndFrame = 7;


					if (!IsOfType(LEMMINGTYPE_DIGGER) && !IsOfType(LEMMINGTYPE_CLIMBER) && !IsOfType(LEMMINGTYPE_PARACHUTE))
						SetType(LEMMINGTYPE_NORMAL);

					//cout << "LEMMINGSTATE_WALK" << endl;
				}
			break;

			case LEMMINGSTATE_BLOCKING:
				mStartFrame = 8;
				mCurrentFrame = 8;
				mEndFrame = 19;

				//cout << "LEMMINGSTATE_BLOCKING" << endl;
			break;

			case LEMMINGSTATE_DIG:
				mStartFrame		= 32;
				mCurrentFrame	= 32;
				mEndFrame		= 39;
				mExploded		= false;

				//cout << "LEMMINGSTATE_DIG" << endl;
			break;

			case LEMMINGSTATE_FALL:
				if (!IsOfType(LEMMINGTYPE_BLOCKER))
				{
					if (IsOfType(LEMMINGTYPE_PARACHUTE))
					{
						mStartFrame		= 23;
						mCurrentFrame	= 23;
						mEndFrame		= 27;

						//Remove current velocity.
						HitGround();

						//cout << "LEMMINGSTATE_FALL (Parachute)" << endl;
					}
					else
					{
						mStartFrame		= 28;
						mCurrentFrame	= 28;
						mEndFrame		= 31;
						
						//Revert to normal Lemming in all case apart from CLIMBER type;
						if (!IsOfType(LEMMINGTYPE_CLIMBER))
							SetType(LEMMINGTYPE_NORMAL);

						//cout << "LEMMINGSTATE_FALL" << endl;
					}
				}
				else
				{
					//cout << "LEMMINGSTATE_FALL (Blocker)" << endl;
				}
			break;

			case LEMMINGSTATE_CLIMB:
				mStartFrame		= 78;
				mCurrentFrame	= 78;
				mEndFrame		= 85;

				//Due to the nature of the walk, when attaching to the wall we need to offset the sprite.
				if (mFacingDirection == FACINGDIRECTION_LEFT)
					mPosition.x += mSingleSpriteWidth / 2;
				else
					mPosition.x -= mSingleSpriteWidth / 2;

				//cout << "LEMMINGSTATE_CLIMB" << endl;
			break;

			case LEMMINGSTATE_CLIMBEXIT:
				mStartFrame		= 86;
				mCurrentFrame	= 86;
				mEndFrame		= 94; 

				//cout << "LEMMINGSTATE_CLIMBEXIT" << endl;				
			break;

			case LEMMINGSTATE_BUILD:
				mStartFrame		= 95;
				mCurrentFrame	= 95;
				mEndFrame		= 111;
				mPlacedPlatform = false;
				mTypeDuration = kBuildDuration;

				//This animation is too high, so needs dropping.
				mPosition.y += 1;

				//cout << "LEMMINGSTATE_BUILD" << endl;
			break;

			case LEMMINGSTATE_PUNCH:
				mStartFrame = 112;
				mCurrentFrame = 112; //Key frames for explosions: 116 & 131
				mEndFrame = 140;

				//cout << "LEMMINGSTATE_PUNCH" << endl;
			break;
			
			case LEMMINGSTATE_PICKAXE:
				mStartFrame		= 172;
				mCurrentFrame	= 172; //Explosion to take place 4 frames in.
				mEndFrame		= 195;

				//cout << "LEMMINGSTATE_PICKAXE" << endl;
			break;

			case LEMMINGSTATE_DEATHFALL:
				mStartFrame		= 56;
				mCurrentFrame	= 56;
				mEndFrame		= 69;
				mExploded		= false;

				//cout << "LEMMINGSTATE_DEATHFALL" << endl;
			break;

			case LEMMINGSTATE_DEATHEXPLOSION:
				mStartFrame		= 141; //Explosion to take place 19 frames in.
				mCurrentFrame	= 141;
				mEndFrame		= 171;
				mExploded		= false;

				//cout << "LEMMINGSTATE_DEATHEXPLOSION" << endl;
			break;

			case LEMMINGSTATE_DEATHDROWN:
				mStartFrame		= 40;
				mCurrentFrame	= 40;
				mEndFrame		= 55;

				//cout << "LEMMINGSTATE_DEATHDROWN" << endl;
			break;
				
			case LEMMINGSTATE_DEATHCOUNTDOWN:
				//This is not an actual state in itself, but begins a countdown over whatever the Lemming is doing, which
				//will result in LEMMINGSTATE_DEATHEXPLOSION when it reaches zero.

				if (!mSelfDetonation)
					mSelfDetonation = true;

				//cout << "LEMMINGSTATE_COUNTDOWN" << endl;
			break;

			case LEMMINGSTATE_EXIT:
				mStartFrame		= 70;
				mCurrentFrame	= 70;
				mEndFrame		= 77;

				//cout << "LEMMINGSTATE_EXIT" << endl;
			break;

			default:
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------------

void Character_Lemmings::SetType(LEMMING_TYPE newType)
{
	//Once in LEMMINGTYPE_BLOCKER, you cannot change, but LEMMINGSTATE_DEATHCOUNTDOWN should still be applied to BLOCKERS.	
	if (mIsAlive)
	{
		if (mLemmingType != newType)
		{
			//Don't actually change the type for these
			if(newType != LEMMINGTYPE_EXPLODER && mLemmingType != LEMMINGTYPE_BLOCKER)
				mLemmingType = newType;

			//Reset frame delay count.
			mFrameDelay = kAnimationDelay_Lemmings;

			switch (newType)
			{
				case LEMMINGTYPE_NORMAL:
					if (mLemmingType != LEMMINGTYPE_BLOCKER)
					{
						//cout << "LEMMINGTYPE_NORMAL" << endl;
					}
				break;

				case LEMMINGTYPE_BLOCKER:
					//Blocker takes effect immendately.
					SetState(LEMMINGSTATE_BLOCKING);

					//cout << "LEMMINGTYPE_BLOCKER" << endl;
				break;

				case LEMMINGTYPE_DIGGER:
					if (mLemmingType != LEMMINGTYPE_BLOCKER)
					{
						//Digger takes effect instantly.
						if (IsInState(LEMMINGSTATE_CLIMB) || IsInState(LEMMINGSTATE_CLIMBEXIT))
							SetState(LEMMINGSTATE_FALL);
						if (!IsInState(LEMMINGSTATE_FALL))
							SetState(LEMMINGSTATE_DIG);

						//cout << "LEMMINGTYPE_DIGGER" << endl;
					}
				break;

				case LEMMINGTYPE_PICKAXE:
					if (mLemmingType != LEMMINGTYPE_BLOCKER)
					{
						//PickAxe takes effect instantly.
						if (IsInState(LEMMINGSTATE_CLIMB) || IsInState(LEMMINGSTATE_CLIMBEXIT))
							SetState(LEMMINGSTATE_FALL);
						if (!IsInState(LEMMINGSTATE_FALL))
							SetState(LEMMINGSTATE_PICKAXE);

						//cout << "LEMMINGTYPE_PICKAXE" << endl;
					}
				break;

				case LEMMINGTYPE_PUNCHER:
					if (mLemmingType != LEMMINGTYPE_BLOCKER)
					{
						//Puncher takes effect instantly.
						if (IsInState(LEMMINGSTATE_CLIMB) || IsInState(LEMMINGSTATE_CLIMBEXIT))
							SetState(LEMMINGSTATE_FALL);
						if (!IsInState(LEMMINGSTATE_FALL))
							SetState(LEMMINGSTATE_PUNCH);

						//cout << "LEMMINGTYPE_PUNCHER" << endl;
					}
				break;

				case LEMMINGTYPE_BUILDER:
					if (mLemmingType != LEMMINGTYPE_BLOCKER)
					{
						//Building takes effect immediately.
						if (IsInState(LEMMINGSTATE_CLIMB) || IsInState(LEMMINGSTATE_CLIMBEXIT))
							SetState(LEMMINGSTATE_FALL);
						if (!IsInState(LEMMINGSTATE_FALL))
							SetState(LEMMINGSTATE_BUILD);

						//cout << "LEMMINGTYPE_BUILDER" << endl;
					}
				break;

				case LEMMINGTYPE_CLIMBER:
					if (mLemmingType != LEMMINGTYPE_BLOCKER)
					{
						SetState(LEMMINGSTATE_WALK);

						//cout << "LEMMINGTYPE_CLIMBER" << endl;
					}
				break;

				case LEMMINGTYPE_EXPLODER:
					SetState(LEMMINGSTATE_DEATHCOUNTDOWN);
					//cout << "LEMMINGATATE_DEATHCOUNTDOWN" << endl;
				break;

				case LEMMINGTYPE_PARACHUTE:
					if (mLemmingType != LEMMINGTYPE_BLOCKER)
					{
						//If already in a fall we need to catch him and change his behaviour.
						if (IsInState(LEMMINGSTATE_FALL))
						{
							mStartFrame = 23;
							mCurrentFrame = 23;
							mEndFrame = 27;
						}
						//cout << "LEMMINGTYPE_PARACHUTE" << endl;
					}
				break;

				default:
				break;
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------

bool Character_Lemmings::IsInActionState()
{
	//An action state is a state that does not want to be overridden by the WALK state, until IT decides.
	return IsInState(LEMMINGSTATE_DIG) ||
		IsInState(LEMMINGSTATE_CLIMB) ||
		IsInState(LEMMINGSTATE_PUNCH) ||
		IsInState(LEMMINGSTATE_PICKAXE);
}

//--------------------------------------------------------------------------------------------------
