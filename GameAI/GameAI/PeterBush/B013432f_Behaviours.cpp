#include "B013432f_Behaviours.h"

B013432f_Behaviours::B013432f_Behaviours()
{
	tankBehaviour = Idle;
	moving = false;
}

B013432f_Behaviours::~B013432f_Behaviours()
{

}

//MAKE IT LOOK AT THE NEAREST ENEMY IF CLOSER THAN TANK
//MAKE THE BELOW CODE WORK
//GET THE CLOSEST TANK TO THE PLAYER AND FOLLOW IT WHEN CLOSE, SIMILAR TO THE ARRIVE
//IF THE TANK FACES YOU FLEE
//OBSTICLE AVOIDANCE

Vector2D B013432f_Behaviours::FindClosest(Vector2D currentPos, TankManager* tankManager)
{
	vector<BaseTank*> Tanks;

	Vector2D thisDistance;
	Vector2D closestDistance = Vector2D(0.0f, 0.0f);
	Vector2D currentPosition = currentPos;

	int numberCanSee = 0;

	for (int i = 0; i < Tanks.size(); i++)
	{
		if (Tanks[i]->CanSee(Tanks[i]))
		{
			numberCanSee++;
			thisDistance = Tanks[i]->GetCentralPosition() - currentPosition;

			if ((thisDistance.x < closestDistance.x) && (thisDistance.y < closestDistance.y))
				closestDistance = thisDistance;
			cout << "A tank can see you!" << endl;
		}
	}
	return thisDistance;
}

void B013432f_Behaviours::GetMousePos()
{
	//cout << "Current Velocity: " << tankVelocity.x << " " << tankVelocity.y << " " << endl;
	//cout << "Current Max Speed: " << tankMaxSpeed << endl;

	int x, y;
	SDL_GetMouseState(&x, &y);
	//cout << x << y << endl;
	mousePosition = Vector2D(x, y);
}

void B013432f_Behaviours::ChooseBehaviour(SDL_Event e)
{	
	Vector2D target = mousePosition;
	distance = DistanceFromTargetCheck(target);
	//<< "Distance " << distance << endl;
	//cout << "TARGET AT: " << target.x << " " << target.y << endl;

	switch (e.type)
	{
		//Deal with keyboard input.
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_q:
			tankBehaviour = Seek;
			moving = true;
			break;
		case SDLK_w:
			tankBehaviour = Flee;
			moving = true;
			break;
		case SDLK_e:
			tankBehaviour = Arrive;
			break;
		case SDLK_r:
			tankBehaviour = ObstacleAvoidance;
			break;
		}
	}

	switch (tankBehaviour)
	{
	case Idle:
		break;
		//case Wander:
		//	WanderBehaviour();
		//	cout << "WanderBehaviour" << endl;
		//	break;
	case Seek:
		if (moving == true)
		{
			outputVelocity = SeekBehaviour(target);
			//cout << "SeekBehaviour" << endl;
		}
		break;
	case Flee:
		if (moving == true)
		{
			outputVelocity = FleeBehaviour(target);
			//cout << "FleeBehaviour" << endl;
		}
		break;
	case Arrive:
		if (moving == true)
		{
			outputVelocity = ArriveBehaviour(target, distance);
			//cout << "ArriveBehaviour" << endl;
		}
		break;
		//case Pursuit:
		//	PursuitBehaviour();
		//	cout << "PursuitBehaviour" << endl;
		//	break;
		//case Evade:
		//	EvadeBehaviour();
		//	cout << "EvadeBehaviour" << endl;
		//	break;
		//case ObstacleAvoidance:
		//	ObstacleAvoidanceBehaviour();
		//	cout << "ObstacleAvoidanceBehaviour" << endl;
		//	break;
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

	cout << "Seek Speed: " << desiredVelocity.Length() << endl;

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

	const double slowDown = 5;
	double neededSpeed = distanceValue / slowDown;
	neededSpeed = min(neededSpeed, tankMaxSpeed);
	Vector2D toTarget = targetInput - tanksPosition;

	if (distanceValue > 75.0)
	{
		//cout << "Speed: " << neededSpeed << endl;
		return SeekBehaviour(targetInput);
	}
	else
	{
		//cout << "Arriving" << endl;
		//cout << "Speed: " << neededSpeed << endl;

		Vector2D desiredVelocity = toTarget * neededSpeed / distance;
		desiredVelocity.Truncate(tankMaxSpeed);
		return (desiredVelocity - tankVelocity);
	}
}
	//Vector2D B013432f_Behaviours::PursuitBehaviour()
	//{
	//	
	//}
	//Vector2D B013432f_Behaviours::EvadeBehaviour()
	//{
	//	
	//}
	//Vector2D B013432f_Behaviours::WanderBehaviour()
	//{
	//	
	//}
	//Vector2D B013432f_Behaviours::ObstacleAvoidanceBehaviour()
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