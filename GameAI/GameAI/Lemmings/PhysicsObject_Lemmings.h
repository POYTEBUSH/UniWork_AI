//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#ifndef _PHYSICSOBJECT_LEMMINGS_H
#define _PHYSICSOBJECT_LEMMINGS_H

#include "Commons_Lemmings.h"
#include <SDL.h>
#include <vector>

using namespace::std;

//---------------------------------------------------------------

class PhysicsObject_Lemmings
{
	//---------------------------------------------------------------
public:
	PhysicsObject_Lemmings(SDL_Renderer* renderer, Vector2D startPosition, PhysicsObjectSetupDetails_Lemmings details);
	~PhysicsObject_Lemmings();

	virtual void		Update(size_t deltaTime, SDL_Event e);
	virtual void		Render();

	void				AddForce(Vector2D force);
	void				ClearVelocity()											{ mVelocity.Zero(); }
	void				HitGround()												{mVelocity.y = 0.0f;}

	//Movement.
	double				GetMass()												{return mMass;}
	double				GetCurrentSpeed()										{return mCurrentSpeed;}
	double				GetMaxSpeed()											{return mMaxSpeed;}
	double				GetMaxForce()											{return mMaxForce;}
	double				GetMaxTurnRate()										{return mMaxTurnRate;}

	Vector2D			GetPosition()											{return mPosition;}
	void				SetPosition(Vector2D newPosition);
	Vector2D		    GetCentralPosition();

	Vector2D			GetHeading()											{return mHeading;}
	Vector2D			GetVelocity()											{return mVelocity;}
	Vector2D			GetSide()												{return mSide;}
	
	virtual float		GetCollisionRadius()									{return mCollisionRadius;}
	virtual Rect2D		GetCollisionBox();

	//---------------------------------------------------------------
protected:
	virtual void	MoveInHeadingDirection(float deltaTime);

	bool			RotateHeadingToFacePosition(Vector2D target, float deltaTime);
	virtual void	RotateHeadingByRadian(double radian, int sign, float deltaTime);		//Sign is direction of turn. Either -1 or 1.

	void			SetHeading(Vector2D newHeading);

	//---------------------------------------------------------------
protected:
	void			DrawDebugCircle(Vector2D centrePoint, double radius, int red, int green, int blue);
	void			DrawDebugLine(Vector2D startPoint, Vector2D endPoint, int red, int green, int blue);
	
	//---------------------------------------------------------------
protected:
	SDL_Renderer*		mRenderer;

	//Movement.
	double				mCurrentSpeed;
	Vector2D			mPosition;
	Vector2D			mVelocity;
	Vector2D			mAcceleration;
	Vector2D			mHeading;
	Vector2D			mSide;	
	//bool				mGravityEnabled;
	float				mRotationAngle;
	
	double				mMass;
	double				mMaxSpeed;
	double				mMaxForce;
	double				mMaxTurnRate;	

	float				mCollisionRadius;
};

//---------------------------------------------------------------

#endif //_PHYSICSOBJECT_LEMMINGS_H