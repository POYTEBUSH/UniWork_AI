#include "B013432f_Behaviours.h"

B013432f_Behaviours::B013432f_Behaviours(BaseTank* thisTank)
{
	moving = false;
	pursuit = true;
	targetBool = false;
	pathing = false;
	_baseTank = thisTank;
	mTargetPickup = false;
	srand(time(NULL));

	mTargetPickup = true;
	target = TargetPickup();
	path = _AStarManager->GetPathBetweenPoint(tanksPosition, target);
	cout << "Target Set: " << target.x << " " << target.y << endl;
	tankBehaviour = AStar;
	moving = true;
}

B013432f_Behaviours::~B013432f_Behaviours()
{

}

void B013432f_Behaviours::TargetClosest(TankManager* tankManager, vector<BaseTank*> Tanks)
{
	Vector2D closestDistance = Vector2D(0.0f, 0.0f);

	for (int i = 0; i < Tanks.size(); i++)
	{
		targetPursuit = Tanks[i]->GetCentralPosition();

		_closestTank = Tanks[i];
		if (pursuit == true && targetBool ==true)
			tankBehaviour = Pursuit;
		else if (pursuit == false && targetBool == true)
			tankBehaviour = Evade;
	}
}

void B013432f_Behaviours::ClosestTank(TankManager* tankManager, vector<BaseTank*> Tanks)
{
	Vector2D closestDistance = Vector2D(0.0f, 0.0f);
	BaseTank* closestTank = nullptr;

	for (int i = 0; i < Tanks.size(); i++)
	{
		targetPursuit = Tanks[i]->GetCentralPosition();

		_closestTank = Tanks[i];
	}
}

Vector2D B013432f_Behaviours::GetMousePos()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	return Vector2D(x, y);
}

void B013432f_Behaviours::ChooseBehaviour(SDL_Event e)
{
	distance = DistanceFromTargetCheck(target);
	ObstacleAvoidanceBehaviour(feelers);
	
	switch (tankBehaviour)
	{
	case Seek:
		if (moving == true)
		{
			outputVelocity = SeekBehaviour(target) + ObstacleAvoidanceBehaviour(feelers) * 2;
		}
		break;
	case Flee:
		if (moving == true)
		{
			outputVelocity = FleeBehaviour(target) + ObstacleAvoidanceBehaviour(feelers) * 2;
		}
		break;
	case Arrive:
		if (moving == true)
		{
			outputVelocity = ArriveBehaviour(target, distance) + ObstacleAvoidanceBehaviour(feelers) * 1.5;
		}
		break;
	case Pursuit:
		if (moving == true)
		{
			outputVelocity = PursuitBehaviour(_closestTank) + ObstacleAvoidanceBehaviour(feelers) * 2;
		}
		break;
	case Evade:
		if (moving == true)
		{
			outputVelocity = EvadeBehaviour(_closestTank) + ObstacleAvoidanceBehaviour(feelers) * 2;
		}
		break;
	case AStar:
		outputVelocity = AStarBehaviour() + ObstacleAvoidanceBehaviour(feelers) * 2;
		break;
	}
}

Vector2D B013432f_Behaviours::AStarBehaviour()
{
	Vector2D pickupLocation = TargetPickup();
	double distToEnd = tanksPosition.Distance(target);	

	while (path.size() > 1)
	{
		pathing = true;

		Vector2D toTarget = path[0] - tanksPosition;		
		double dist = tanksPosition.Distance(path[0]);


		if (tanksPosition.Distance(mClosestTank) <= 200 && _baseTank->GetBullets() > 0)
		{
			_baseTank->ChangeState(TANKSTATE_MANFIRE);
			if (tanksPosition.Distance(mClosestTank) <= 10)
				_baseTank->ChangeState(TANKSTATE_DROPMINE);
			if (_closestTank != nullptr)
				return PursuitBehaviour(_closestTank);
			else
				return ArriveBehaviour(tanksPosition, 0.0f);
		}	

		if (pickupLocation.x >= 35 && pickupLocation.x <= 925 && pickupLocation.y >= 35 && pickupLocation.y <= 605 && pickupLocation != Vector2D(0.0f, 0.0f))
			return SeekBehaviour(pickupLocation);

		if (distToEnd < 25 && _baseTank->GetCurrentSpeed() > 5)
		{
			_baseTank->ChangeState(TANKSTATE_DROPMINE);
			return ArriveBehaviour(target, distToEnd);
		}

		if (dist <= 20 && path.size() > 0)
		{			
			path.erase(path.begin());
		}

		_baseTank->ChangeState(TANKSTATE_IDLE);
		return SeekBehaviour(path[0]);
	}
	if (!mTargetPickup || path.size() <= 1)
	{
		if (distToEnd <= 10.0)
		{
			distToEnd = MaxDouble;
			path = _AStarManager->GetPathBetweenPoint(tanksPosition, pickupLocation);
		}
	}

	pathing = false;
	path = _AStarManager->GetPathBetweenPoint(tanksPosition, _AStarManager->GetNearestWaypoint(Vector2D(rand() % (925 - 35 + 1) + 35, rand() % (605 - 35 + 1) + 35))->GetPosition());
}

Vector2D B013432f_Behaviours::DetectPickup()
{
	vector<GameObject*> objects = PickUpManager::Instance()->GetAllPickUps();

	for each (auto pickup in objects)
	{
		Vector2D pickupPos = pickup->GetCentralPosition();
		double dist = tanksPosition.Distance(pickupPos);

		if (dist < 30)
			return pickupPos;
	}
	return Vector2D(0,0);
}

Vector2D B013432f_Behaviours::TargetPickup()
{
	vector<GameObject*> objects = PickUpManager::Instance()->GetAllPickUps();
	Vector2D pickupPos;
	double dist = MaxDouble;

	if (!objects.empty())
	{
		for each (auto pickup in objects)
		{
			double distToPickup = tanksPosition.Distance(pickupPos);

			if (distToPickup < dist && pickup->GetCentralPosition() != Vector2D(0, 0))
			{
				dist = distToPickup;
				pickupPos = pickup->GetCentralPosition();
			}
		}
		return pickupPos;
	}
	return Vector2D(0, 0);
}

double B013432f_Behaviours::DistanceFromTargetCheck(Vector2D target)
{
	Vector2D thisToTarget = target - tanksPosition;
	double distance = thisToTarget.Length();
	return distance;
}

Vector2D B013432f_Behaviours::SeekBehaviour(Vector2D target)
{
	Vector2D desiredVelocity = Vec2DNormalize(target - tanksPosition) * tankMaxSpeed;
	desiredVelocity.Truncate(tankMaxSpeed);
	Vector2D final = (desiredVelocity - tankVelocity);
	return final;
}

Vector2D B013432f_Behaviours::FleeBehaviour(Vector2D target)
{
	Vector2D desiredVelocity = Vec2DNormalize(tanksPosition - target) * tankMaxSpeed;
	desiredVelocity.Truncate(tankMaxSpeed);
	Vector2D final = (desiredVelocity - tankVelocity);
	return final;
}

Vector2D B013432f_Behaviours::ArriveBehaviour(Vector2D targetInput, double distanceValue)
{
	const double slowDown = 2;
	double neededSpeed = distanceValue / slowDown;
	neededSpeed = min(neededSpeed, tankMaxSpeed);
	Vector2D toTarget = targetInput - tanksPosition;

	if (distanceValue > 100.0)
	{
		return SeekBehaviour(targetInput);
	}
	else
	{
		Vector2D desiredVelocity = toTarget * neededSpeed / distance;
		desiredVelocity.Truncate(tankMaxSpeed);
		return (desiredVelocity - tankVelocity);
	}
}

Vector2D B013432f_Behaviours::PursuitBehaviour(BaseTank* evader)
{
	Vector2D toEvader = (evader->GetCentralPosition() - tanksPosition) - Vector2D(10.0f, 10.0f);
	double relativeHeading = tankHeading.Dot(evader->GetHeading());

	if ((toEvader.Dot(tankHeading) > 0) && (relativeHeading < -0.95))
		return ((SeekBehaviour(evader->GetCentralPosition()))* 0.8);

	double lookAheadTime = toEvader.Length() / (tankMaxSpeed + evader->GetCurrentSpeed());

	return ((SeekBehaviour(evader->GetCentralPosition() + evader->GetVelocity() * lookAheadTime))* 0.8);
}

Vector2D B013432f_Behaviours::EvadeBehaviour(BaseTank* pursuer)
{
	Vector2D toPursuer = pursuer->GetCentralPosition() - tanksPosition;
	double lookAheadTime = toPursuer.Length() / (tankMaxSpeed + pursuer->GetCurrentSpeed());

	return FleeBehaviour(pursuer->GetCentralPosition() + pursuer->GetVelocity() * lookAheadTime);
}

Vector2D B013432f_Behaviours::ObstacleAvoidanceBehaviour(vector<Vector2D> feelers)
{
	Vector2D collisionDir;

	for (int i = 0; i < mObstacles.size(); i++)
	{
		for each (Vector2D feeler in feelers)
		{
			if ((feeler.x > mObstacles[i].x && feeler.x < (mObstacles[i].x + mObstacles[i].width)) && (feeler.y > mObstacles[i].y && feeler.y < (mObstacles[i].y + mObstacles[i].height)))
			{
				collisionDir = Vector2D((mObstacles[i].x - feeler.x), (mObstacles[i].y - feeler.y));
				return FleeBehaviour(Vector2D(mObstacles[i].x + mObstacles[i].width/2, mObstacles[i].y + mObstacles[i].height/2));
			}	
		}	
	}
	return Vector2D(0, 0);
}