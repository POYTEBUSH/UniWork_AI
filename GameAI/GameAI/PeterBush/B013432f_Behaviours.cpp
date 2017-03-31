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

	switch (e.type)
	{

	case SDL_MOUSEBUTTONDOWN:
		switch(e.button.button)
		{
		case SDL_BUTTON_LEFT:
			mTargetPickup = false;
			target = GetMousePos();
			path = _AStarManager->GetPathBetweenPoint(tanksPosition, target);
			cout << "Target Set: " << target.x << " " << target.y << endl;
		break;	
		case SDL_BUTTON_RIGHT:
			mTargetPickup = true;
			target = TargetPickup();
			path = _AStarManager->GetPathBetweenPoint(tanksPosition, target);
			cout << "Target Set: " << target.x << " " << target.y << endl;
		break;
		}
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_q:
			tankBehaviour = Seek;
			cout << "Key Q Pressed" << endl;
			moving = true;
			break;
		case SDLK_w:
			tankBehaviour = Flee;
			cout << "Key W Pressed" << endl;
			moving = true;
			break;
		case SDLK_e:
			tankBehaviour = Arrive;
			cout << "Key E Pressed" << endl;
			break;
		case SDLK_r:
			if (pursuit == false && targetBool == true)
			{
				pursuit = true;
				cout << "Pursuit Set" << endl;
			}
			else if (pursuit == true && targetBool == true)
			{
				pursuit = false;
				cout << "Evade Set" << endl;
			}			
			break;
		case SDLK_t:
			if (targetBool == false)
			{
				targetBool = true;
				cout << "Now Targeting" << endl;
			}
			else
			{
				targetBool = false;
				cout << "No Longer Targeting" << endl;
			}
			break;
		case SDLK_y:
			if (pathing == false)
			{
				mTargetPickup = true;
				target = TargetPickup();
				path = _AStarManager->GetPathBetweenPoint(tanksPosition, target);
				cout << "Target Set: " << target.x << " " << target.y << endl;
				tankBehaviour = AStar;
				moving = true;
			}
			break;
		}
	}

	switch (tankBehaviour)
	{
	case Seek:
		if (moving == true)
		{
			outputVelocity = SeekBehaviour(target) + ObstacleAvoidanceBehaviour(feelers) * 1.5;
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
		outputVelocity = AStarBehaviour();
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

		if (tanksPosition.Distance(mClosestTank) <= 50)
		{
			SeekBehaviour(mClosestTank);
			return ArriveBehaviour(tanksPosition, 0.0);
		}

		if (dist <= 20 && path.size() > 0)
		{
			_baseTank->ChangeState(TANKSTATE_DROPMINE);
			path.erase(path.begin());
			cout << "Moving To: " << path[0].x << " " << path[0].y <<  " | Distance To Next Node: " << tanksPosition.Distance(path[0]) << "m | Fuel Left: " << _baseTank->GetFuel() << endl;
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
	if (pickupLocation == Vector2D(0, 0))
		path = _AStarManager->GetPathBetweenPoint(tanksPosition, _AStarManager->GetNearestWaypoint(Vector2D(rand() % (925 - 35 + 1) + 35, rand() % (605 - 35 + 1) + 35))->GetPosition());
	else if (pickupLocation.x >= 35 && pickupLocation.x <= 925 && pickupLocation.y >= 35 && pickupLocation.y <= 605)
		return SeekBehaviour(pickupLocation);
	else
		return ArriveBehaviour(tanksPosition, 0.0);
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
				cout << "Colliding With: " << mObstacles[i].x << " " << mObstacles[i].y << endl;
				 ;
			}	
		}	
	}
	return Vector2D(0, 0);
}