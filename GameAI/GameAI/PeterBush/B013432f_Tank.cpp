#include "B013432f_Tank.h"
#include "..\TankManager.h"
#include "..\Commons.h"
#include "..\C2DMatrix.h"

#include "B013432f_Commons.h"

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
	mFeelerRadius		= 10.0;
	//_tankState			= Idle;
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

void B013432f_Tank::CalcFeelers()
{
	Vector2D centralPos = GetCentralPosition();
	Vector2D velocity = GetVelocity();

	Vector2D frontFeeler(centralPos + velocity);
	frontFeeler.Normalize();

	Vector2D heading = Vec2DNormalize(mVelocity);

	//Create point rotated to the left of heading.
	Vector2D leftPoint;
	leftPoint.x = (heading.x * cos(kFieldOfView)) - (heading.y * sin(kFieldOfView));
	leftPoint.y = (heading.x * sin(kFieldOfView)) + (heading.y * cos(kFieldOfView));

	//Create point rotated to the right of heading.
	Vector2D rightPoint;
	rightPoint.x = (heading.x * cos(-kFieldOfView)) - (heading.y * sin(-kFieldOfView));
	rightPoint.y = (heading.x * sin(-kFieldOfView)) + (heading.y * cos(-kFieldOfView));

	//Move the left point out from the centre of the tank to the distance set by kFieldOfViewLength.
	Vector2D leftFeeler;
	leftFeeler.x = centralPos.x + (leftPoint.x* 25);
	leftFeeler.y = centralPos.y + (leftPoint.y* 25);

	//Move the right point out from the centre of the tank to the distance set by kFieldOfViewLength.
	Vector2D rightFeeler;
	rightFeeler.x = centralPos.x + (rightPoint.x* 25);
	rightFeeler.y = centralPos.y + (rightPoint.y* 25);

	feelers[0] = GetCentralPosition() + frontFeeler + mVelocity;
	feelers[1] = leftFeeler + mHeading * 5;
	feelers[2] = rightFeeler + mHeading * 5;
	feelers[3] = leftFeeler - mHeading * 45;
	feelers[4] = rightFeeler - mHeading * 45;
}



Vector2D B013432f_Tank::FindClosestTank(TankManager* tankManager, vector<BaseTank*> Tanks)
{
	double closestDistance = MaxDouble;
	Vector2D targetLocation = Vector2D(0.0, 0.0);

	for (int i = 0; i < Tanks.size(); i++)
	{
		double distanceToTank = GetCentralPosition().Distance(Tanks[i]->GetCentralPosition());

		if (distanceToTank < closestDistance)
		{
			closestDistance = distanceToTank;
			targetLocation = Tanks[i]->GetCentralPosition();
		}
	}
	return targetLocation;
}

bool B013432f_Tank::RotateManToFacePosition(Vector2D target, float deltaTime)
{
	Vector2D toTarget = Vec2DNormalize(GetCentralPosition() - target);

	//Determine the angle between the heading vector and the target.
	double angle = acos(GetHeading().Dot(toTarget));

	//Ensure angle does not become NaN and cause the tank to disappear.
	if (angle != angle)
		angle = 0.0f;

	//Return true if the player is facing the target.
	if (angle < 0.00001)
		return true;

	RotateManByRadian(angle, GetHeading().Sign(toTarget), deltaTime);

	return true;
}

void B013432f_Tank::Update(float deltaTime, SDL_Event e)
{	
	vector <BaseTank*> Tanks = mTanksICanSee;
	mPath = _tankBehaviour->GetPath();

	_tankBehaviour->SetObstacles(mCollisionManager->GetWalls());
	_tankBehaviour->SetClosestTank(FindClosestTank(_tankManager, Tanks));
	_tankBehaviour->ClosestTank(_tankManager, Tanks);
	_tankBehaviour->tanksPosition = GetCentralPosition();
	_tankBehaviour->ChooseBehaviour(e);
	_tankBehaviour->tankMaxSpeed = GetMaxSpeed();
	_tankBehaviour->tankHeading = GetHeading();
	_tankBehaviour->tankCurrentSpeed = GetCurrentSpeed();
	_tankBehaviour->tankFrontPoint = GetPointAtFrontOfTank();
	_tankBehaviour->tankBackPoint = GetPointAtRearOfTank();

	CalcFeelers();
	_tankBehaviour->feelers = feelers;

	cout.clear();
	
	if (_tankBehaviour->moving == true)
	{
		_tankBehaviour->tankVelocity = GetVelocity();
		_tankBehaviour->GetMousePos();

		Vector2D aheadDistance = mVelocity;
		if (aheadDistance.Length() == 0.0f)
			aheadDistance = mHeading;
					
		Vector2D force = _currentVelocity;
		Vector2D acceleration = force / GetMass();
		mVelocity += acceleration * deltaTime;
		mVelocity.Truncate(GetMaxSpeed());

		Vector2D ahead = GetCentralPosition() + aheadDistance;
		RotateManToFacePosition(ahead, deltaTime);
		RotateHeadingToFacePosition(ahead, deltaTime);

		_currentVelocity = _tankBehaviour->outputVelocity;
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
	Vector2D force = _currentVelocity;

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

void B013432f_Tank::Render()
{
	BaseTank::Render();
	DrawDebugLine(GetCentralPosition(), GetCentralPosition() + mHeading*kFieldOfViewLength, 255, 0, 255);
}

//--------------------------------------------------------------------------------------------------