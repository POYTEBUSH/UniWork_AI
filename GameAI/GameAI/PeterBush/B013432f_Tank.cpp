#include "B013432f_Tank.h"
#include "..\TankManager.h"
#include "..\Commons.h"
#include "..\C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

B013432f_Tank::B013432f_Tank(SDL_Renderer* renderer, TankSetupDetails details)
	: BaseTank(renderer, details)
{
	mTankTurnDirection  = DIRECTION_UNKNOWN;
	mTankTurnKeyDown	= false;
	mTankMoveDirection	= DIRECTION_NONE;
	mTankMoveKeyDown	= false;
	mManTurnDirection   = DIRECTION_UNKNOWN;
	mManKeyDown			= false;
	mFireKeyDown		= false;
	_tankBehaviour->tankMaxSpeed = GetMaxSpeed();
}

//--------------------------------------------------------------------------------------------------

B013432f_Tank::~B013432f_Tank()
{
}

//--------------------------------------------------------------------------------------------------

void B013432f_Tank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void B013432f_Tank::Update(float deltaTime, SDL_Event e)
{	
	//Vector2D closestTank = _tankBehaviour->FindClosest(GetPosition(), _tankManager);
	//cout << "Closest Tank " << closestTank.Length() << endl;

	_tankBehaviour->tanksPosition = GetCentralPosition();
	_tankBehaviour->ChooseBehaviour(e);
	_tankBehaviour->tankMaxSpeed = GetMaxSpeed();
	
	if (_tankBehaviour->moving == true)
	{
		_tankBehaviour->tankVelocity = GetVelocity();
		_tankBehaviour->GetMousePos();
		MoveInHeadingDirection(deltaTime);
		//cout << "Fuel: " << GetFuel() << endl;

		Vector2D aheadDistance = mVelocity;
		if (aheadDistance.Length() == 0.0f)
			aheadDistance = mHeading;

		Vector2D ahead = GetCentralPosition() + aheadDistance;
		RotateHeadingToFacePosition(ahead, deltaTime);
	}

	//Call parent update.
	BaseTank::Update(deltaTime, e);
}

void B013432f_Tank::UpdateMovement()
{
	//velocity
}

//--------------------------------------------------------------------------------------------------

void B013432f_Tank::MoveInHeadingDirection(float deltaTime) // Use this, pass the values from the seek method etc. into this!
{
	//Get the force that propels in current heading.
	_tankBehaviour->outputVelocity.Truncate(GetMaxSpeed());
	Vector2D force = _tankBehaviour->outputVelocity;

	//Acceleration = Force/Mass
	Vector2D acceleration = force/GetMass();

	//Update velocity.
	mVelocity += acceleration * deltaTime;
	
	//Don't allow the tank does not go faster than max speed.
	mVelocity.Truncate(GetMaxSpeed()); //TODO: Add Penalty for going faster than MAX Speed.

	//Finally, update the position.
	Vector2D newPosition = GetPosition();
		newPosition.x += mVelocity.x*deltaTime;
		newPosition.y += (mVelocity.y/**-1.0f*/)*deltaTime;	//Y flipped as adding to Y moves down screen.
	SetPosition(newPosition);
}

//--------------------------------------------------------------------------------------------------

void B013432f_Tank::RotateHeadingByRadian(double radian, int sign)
{
	//Clamp the amount to turn to the max turn rate.
	if (radian > mMaxTurnRate) 
		radian = mMaxTurnRate;
	else if(radian < -mMaxTurnRate)
		radian = -mMaxTurnRate;
	//IncrementTankRotationAngle(RadsToDegs(radian));
    mRotationAngle += RadsToDegs(radian)*sign;

	//Usee a rotation matrix to rotate the player's heading
	C2DMatrix RotationMatrix;
  
	//Calculate the direction of rotation.
	RotationMatrix.Rotate(radian * sign);	
	//Get the new heading.
	RotationMatrix.TransformVector2Ds(mHeading);

	//cout << "RotateHeadingByRadian -- Heading x = " << mHeading.x << " y = " << mHeading.y << endl;

	//Get the new velocity.
	RotationMatrix.TransformVector2Ds(mVelocity);

	//Side vector must always be perpendicular to the heading.
	mSide = mHeading.Perp();
}

//--------------------------------------------------------------------------------------------------