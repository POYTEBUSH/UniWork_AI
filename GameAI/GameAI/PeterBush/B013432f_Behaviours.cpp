#include "B013432f_Behaviours.h"

B013432f_Behaviours::B013432f_Behaviours()
{
	tankBehaviour = Idle;
	moving = false;
	pursuit = true;
	targetBool = false;
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
		//cout << "Spotted Tank " << Tanks[i]->GetTankName();
		_closestTank = Tanks[i];
		if (pursuit == true && targetBool ==true)
			tankBehaviour = Pursuit;
		else if (pursuit == false && targetBool == true)
			tankBehaviour = Evade;
	}
	//if (Tanks.size() == 0)
	//	tankBehaviour = Seek;
}

void B013432f_Behaviours::GetMousePos()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	mousePosition = Vector2D(x, y);
}

void B013432f_Behaviours::ChooseBehaviour(SDL_Event e)
{	
	target = mousePosition;
	distance = DistanceFromTargetCheck(target);
	ObstacleAvoidanceBehaviour(feelers);

	switch (e.type)
	{
		//Deal with keyboard input.
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
		}
	}

	switch (tankBehaviour)
	{
	case Idle:
		//cout << "Please select a behaviour" << endl;
		break;
		//case Wander:
		//	WanderBehaviour();
		//	cout << "WanderBehaviour" << endl;
		//	break;
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
			//cout << "Arrive" << endl;
			outputVelocity = ArriveBehaviour(target, distance) + ObstacleAvoidanceBehaviour(feelers) * 2;
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
		//case WallAvoidance:
		//	WallAvoidanceBehaviour();
		//	cout << "WallAvoidanceBehaviour" << endl;
		//	break;
		//case Interpose:
		//	InterposeBehaviour();
		//	cout << "InterposeBehaviour" << endl;
		//	break;
		//case Hide:
		//	HideBehaviour();
		//	cout << "HideBehaviour" << endl;
		//	break;
		//case PathFollow:
		//	PathFollowBehaviour();
		//	cout << "PathFollowBehaviour" << endl;
		//	break;
		//case OffsetPusuit:
		//	OffsetPusuitBehaviour();
		//	cout << "OffsetPusuitBehaviour" << endl;
		//	break;
	}
}

double B013432f_Behaviours::DistanceFromTargetCheck(Vector2D target)
{
	Vector2D thisToTarget = target - tanksPosition;
	double distance = thisToTarget.Length();

	//cout << distance << endl;

	return distance;
}

Vector2D B013432f_Behaviours::SeekBehaviour(Vector2D target)
{
	Vector2D desiredVelocity = Vec2DNormalize(target - tanksPosition) * tankMaxSpeed;
	desiredVelocity.Truncate(tankMaxSpeed);

	//cout << "Seek Speed: " << desiredVelocity.Length() << endl;

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
	vector<GameObject*> objects = ObstacleManager::Instance()->GetObstacles();
	for (int i = 0; i < objects.size(); i++)
	{

		Rect2D rect = Rect2D(objects[i]->GetPosition().x, objects[i]->GetPosition().y,
			(objects[i]->GetCentralPosition().x - objects[i]->GetPosition().x) * 2,
			(objects[i]->GetCentralPosition().y - objects[i]->GetPosition().y) * 2);

			//cout << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << endl;

		
		for (int j = 0; j < feelers.size(); j++)
		{
			if ((feelers[0].x >= rect.x && feelers[0].x <= rect.x + rect.width) && (feelers[0].y >= rect.y && feelers[0].y <= rect.y + rect.height))
			{
				cout << "colliding with object: " <<  objects[i] << endl;				
				return FleeBehaviour(target);
			}
			if ((feelers[1].x >= rect.x && feelers[1].x <= rect.x + rect.width) && (feelers[1].y >= rect.y && feelers[1].y <= rect.y + rect.height))
			{
				cout << "colliding with object: " << objects[i] << endl;
				return FleeBehaviour(target.Perp() * 90);
			}
			if ((feelers[2].x >= rect.x && feelers[2].x <= rect.x + rect.width) && (feelers[2].y >= rect.y && feelers[2].y <= rect.y + rect.height))
			{
				cout << "colliding with object: " << objects[i] << endl;
				return FleeBehaviour(target.Perp() * -90);
			}
		}		
	}
	return Vector2D(0, 0);
}
//Vector2D B013432f_Behaviours::WanderBehaviour()
//{
//	
//}
	//Vector2D B013432f_Behaviours::WallAvoidanceBehaviour()
	//{
	//	
	//}
	//Vector2D B013432f_Behaviours::InterposeBehaviour()
	//{
	//	
	//}
	//Vector2D B013432f_Behaviours::HideBehaviour()
	//{
	//	
	//}
	//Vector2D B013432f_Behaviours::PathFollowBehaviour()
	//{
	//	
	//}
	//Vector2D B013432f_Behaviours::OffsetPusuitBehaviour()
	//{
	//	
	//}