//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#include "PhysicsObject_Lemmings.h"
#include <SDL.h>
#include <iostream>
#include <fstream>
#include "Landscape_Lemmings.h"
#include "Constants_Lemmings.h"
#include <cassert>
#include "../C2DMatrix.h"
#include "Commons_Lemmings.h"

using namespace::std;

//--------------------------------------------------------------------------------------------------

PhysicsObject_Lemmings::PhysicsObject_Lemmings(SDL_Renderer* renderer, Vector2D startPosition, PhysicsObjectSetupDetails_Lemmings details)
{
	mRenderer						= renderer;
	mPosition						= startPosition;

	mCurrentSpeed					= 0.0f;
	mVelocity.Zero();
	mAcceleration.Zero();
	mHeading						= Vector2D(0.0f, -1.0f);
	mSide							= Vector2D(1.0f, 0.0f);
	mMass							= details.Mass;
	mMaxSpeed						= details.MaxSpeed;
	mRotationAngle					= 0.0f;

	//TODO: These may not be needed.
	mCollisionRadius				= 2.0f;
	mMaxForce						= 10.0f;
	mMaxTurnRate					= 5000;// details.TurnRate / 500.0f
}

//--------------------------------------------------------------------------------------------------

PhysicsObject_Lemmings::~PhysicsObject_Lemmings()
{
	mRenderer = NULL;
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::Update(size_t deltaTime, SDL_Event e)
{
	//Include gravity
	mAcceleration.y += kGravity_Lemmings;

	//			  mass * acceleration
	Vector2D force = mMass*mAcceleration;
	mVelocity += (force * deltaTime);
	
	//We need a maximum velocity.
	mVelocity.x = min(mVelocity.x, kMaxXVelocity);
	mVelocity.y = min(mVelocity.y, kMaxYVelocity);

	//Rebound off ground.
//	if (Landscape_Lemmings::Instance()->GetGroundTypeAt(mPosition.x, mPosition.y) == LANDSCAPETYPE_GROUND)
//		mVelocity *= -1.0f;

	mPosition.x += (int)mVelocity.x;
	mPosition.y += (int)mVelocity.y;

	//Clear the acceleration for next frame.
	mAcceleration.Zero();
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::Render()
{
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::MoveInHeadingDirection(float deltaTime)
{
	//cout << "PHYSICS OBJECT: Must override MoveInHeadingDirection()." << endl;
}

//--------------------------------------------------------------------------------------------------

Rect2D PhysicsObject_Lemmings::GetCollisionBox()
{
	//cout << "PHYSICS OBJECT: Must override GetCollisionBox()." << endl;
	return Rect2D();
}

//--------------------------------------------------------------------------------------------------

Vector2D PhysicsObject_Lemmings::GetCentralPosition()
{
	cout << "---PhysicsObject::GetCentralPosition() --- Must override" << endl;
	return Vector2D();
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::AddForce(Vector2D force)
{
	mAcceleration += force;
}

//--------------------------------------------------------------------------------------------------

bool PhysicsObject_Lemmings::RotateHeadingToFacePosition(Vector2D target, float deltaTime)
{
	Vector2D toTarget = Vec2DNormalize(GetCentralPosition()-target);

	//Determine the angle between the heading vector and the target.
	double angle = acos(mHeading.Dot(toTarget));

	//Ensure angle does not become NaN and cause the tank to disappear.
	if( angle != angle)
		angle = 0.0f;

	//Return true if the player is facing the target.
	if(angle < 0.00001) 
		return true;

	RotateHeadingByRadian(angle, mHeading.Sign(toTarget), deltaTime);

	return true;
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::RotateHeadingByRadian(double radian, int sign, float deltaTime)
{
	//Incorporate delta time.
	radian *= deltaTime;

	//Clamp the amount to turn to the max turn rate.
	if (radian > mMaxTurnRate) 
		radian = mMaxTurnRate;
	else if(radian < -mMaxTurnRate)
		radian = -mMaxTurnRate;

	//IncrementTankRotationAngle(RadsToDegs(radian));
    mRotationAngle += (float)RadsToDegs(radian)*sign;

	//Usee a rotation matrix to rotate the player's heading
	C2DMatrix RotationMatrix;
  
	//Calculate the direction of rotation.
	RotationMatrix.Rotate(radian * sign);	
	//Get the new heading.
	RotationMatrix.TransformVector2Ds(mHeading);

	//Side vector must always be perpendicular to the heading.
	mSide = mHeading.Perp();
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::SetHeading(Vector2D newHeading)
{
	//Check if the new heading is unit length.
	assert( (newHeading.LengthSq() - 1.0) < 0.00001);
  
	//Set the new heading.
	mHeading = newHeading;

	//Side vector must always be perpendicular to the heading.
	mSide = mHeading.Perp();
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::SetPosition(Vector2D newPosition)
{
	//Set the new position.
	mPosition.x = newPosition.x;
	mPosition.y = newPosition.y;
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::DrawDebugCircle(Vector2D centrePoint, double radius, int red, int green, int blue)
{
#ifdef DEBUG_LINES_VISIBLE 
	Vector2D polarVec(0.0f, radius);

	float stepSize = 0.02f;
	float _360DegAsRads = (float)DegsToRads(360.0f);

	while (polarVec.x < _360DegAsRads)
	{
		Vector2D polarAsCart(polarVec.y * cosf(polarVec.x), polarVec.y * sinf(polarVec.x));
		Vector2D drawPoint(centrePoint.x + polarAsCart.x, centrePoint.y + polarAsCart.y);

		SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
		SDL_RenderDrawPoint(mRenderer, (int)drawPoint.x, (int)drawPoint.y);

		polarVec.x += stepSize;
	}
#endif
}

//--------------------------------------------------------------------------------------------------

void PhysicsObject_Lemmings::DrawDebugLine(Vector2D startPoint, Vector2D endPoint, int red, int green, int blue)
{
#ifdef DEBUG_LINES_VISIBLE
	SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
	SDL_RenderDrawLine(mRenderer, (int)startPoint.x, (int)startPoint.y, (int)endPoint.x, (int)endPoint.y);
#endif
}

//--------------------------------------------------------------------------------------------------