//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _CHARACTER_LEMMINGS_H
#define _CHARACTER_LEMMINGS_H

#include <SDL.h>
#include "PhysicsObject_Lemmings.h"
#include "Commons_Lemmings.h"
#include "../Texture2D.h"
#include <iostream>
using namespace::std;

class Landscape_Lemmings;

class Character_Lemmings : public PhysicsObject_Lemmings
{
//--------------------------------------------------------------------------------------------------
public:
	Character_Lemmings(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, int lemmingIndex);
	~Character_Lemmings();

	virtual void		Render();
	virtual void		Update(size_t deltaTime, SDL_Event e);

	void				ResetLemming(Vector2D startPosition);
	bool				HasSpawned()									{ return mSpawned; }
	void				Spawn();

	virtual Vector2D	GetCentralPosition();
	virtual Rect2D		GetCollisionBox();

	int					GetLemmingIndex()								{ return mLemmingIndex; }

	LEMMING_TYPE		GetType()										{ return mLemmingType; }
	void				SetType(LEMMING_TYPE newType);
	bool				IsOfType(LEMMING_TYPE type)						{ return mLemmingType == type; }
				
	LEMMING_STATE		GetState()										{ return mState; }
	virtual void		SetState(LEMMING_STATE newState);
	bool				IsInState(LEMMING_STATE state)					{ return mState == state; }
	bool				IsInActionState();

	bool				InTheAir()										{ return IsInState(LEMMINGSTATE_FALL); }
	bool				InADeathState()									{ return IsInState(LEMMINGSTATE_DEATHFALL) || IsInState(LEMMINGSTATE_DEATHDROWN) || IsInState(LEMMINGSTATE_DEATHEXPLOSION); }
	FACING_DIRECTION	GetFacing()										{ return mFacingDirection; }
	void				ForceChangeDirection();

	bool				IsAlive()										{ return mIsAlive; }
	bool				IsSelfDetonating()								{ return mSelfDetonation; }

	unsigned int		GetCollidingBlockerIndex()						{ return mCollidingBlockerIndex; }
	void				SetIsCollidingWithBlocker(int index)			{ mCollidingBlockerIndex = index; }

//--------------------------------------------------------------------------------------------------
protected:
	void				UpdateAnimations(size_t deltaTime);

	void				Walk(size_t deltaTime, float speed);
	void				Dig(size_t deltaTime);
	void				GeneralDeath(size_t deltaTime);
	void				DetonationDeath(size_t deltaTime);
	void				Escape(size_t deltaTime);
	void				Climb(size_t deltaTime, float speed);
	void				ClimbExit(size_t deltaTime, float speed);
	void				Build(size_t deltaTime);
	void				Punch(size_t deltaTime, float xMovement);
	void				PickAxe(size_t deltaTime, float xMovement);

//--------------------------------------------------------------------------------------------------
protected:
	SDL_Renderer*			mRenderer;
	Texture2D*				mTexture;
	Texture2D*				mWhiteNumbers;

	LEMMING_TYPE			mLemmingType;
	int						mLemmingIndex;

	FACING_DIRECTION		mFacingDirection;
	float					mSingleSpriteWidth;
	float					mSingleSpriteHeight;

	bool					mIsAlive;				//Has spawned, but died.
	bool					mSpawned;				//Has been put in play.
	bool					mExploded;
	bool					mPlacedPlatform;

	LEMMING_STATE			mState;
	int						mTypeDuration;

	int						mStartFrame;
	int						mEndFrame;
	int						mCurrentFrame;
	float					mFrameDelay;

	bool					mSelfDetonation;
	int						mDetonationCount;

	unsigned int			mCollidingBlockerIndex;
};
//--------------------------------------------------------------------------------------------------
#endif //_CHARACTER_LEMMINGS_H