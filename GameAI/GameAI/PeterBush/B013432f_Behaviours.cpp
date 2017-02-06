#include "B013432f_Behaviours.h"

B013432f_Behaviours::B013432f_Behaviours()
{
	tankBehaviour = Idle;
	moving = false;
}

B013432f_Behaviours::~B013432f_Behaviours()
{

}

//Vector2D B013432f_Behaviours::FindClosest(Vector2D currentPos, TankManager* tankManager, bool Can)
//{
//	
//	Vector2D thisDistance;
//	Vector2D closestDistance = Vector2D(0.0f, 0.0f);
//	Vector2D currentPosition = currentPos;
//
//	for (int i = 0; i < Tanks.size(); i++)
//	{
//		if (Tanks[i]->CanSee(Tanks[i]))
//		{
//			thisDistance = Tanks[i]->GetCentralPosition() - currentPosition;
//
//			if ((thisDistance.Length.x < closestDistance.Length.x) && (thisDistance.Length.y < closestDistance.Length.y))
//				closestDistance = thisDistance;
//		}
//	}
//}

void B013432f_Behaviours::GetMousePos()
{
	//cout << "Current Velocity: " << tankVelocity.x << " " << tankVelocity.y << " " << endl;
	//cout << "Current Max Speed: " << tankMaxSpeed << endl;

	int x, y;
	SDL_GetMouseState(&x, &y);
	cout << x << y << endl;
	mousePosition = Vector2D(x, y);
}

void B013432f_Behaviours::ChooseBehaviour(SDL_Event e)
{
	switch (e.type)
	{
		//Deal with keyboard input.
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_q:
			tankBehaviour = Seek;
			break;
		case SDLK_w:
			tankBehaviour = Flee;
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
		distance = DistanceFromTargetCheck(mousePosition);
		outputVelocity = SeekBehaviour(mousePosition);
		moving = true;
		cout << "SeekBehaviour" << endl;
		break;
	case Flee:
		distance = DistanceFromTargetCheck(mousePosition);
		outputVelocity = FleeBehaviour(mousePosition);
		cout << "FleeBehaviour" << endl;
		break;
	case Arrive:
		//ArriveBehaviour();
		cout << "ArriveBehaviour" << endl;
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

//GET DISTANCE CHECK WORKING
//MAKE TANK ROTATE TO FOLLOW THE MOUSE AT ALL TIMES
//MAKE IT LOOK AT THE NEAREST ENEMY IF CLOSER THAN TANK

double B013432f_Behaviours::DistanceFromTargetCheck(Vector2D target)
{
	Vector2D thisToTarget = target - tanksPosition;
	double distance = thisToTarget.Length();

	return distance;
}

Vector2D B013432f_Behaviours::SeekBehaviour(Vector2D target)
{
	Vector2D desiredVelocity = Vec2DNormalize(target - tanksPosition) * tankMaxSpeed;
	Vector2D final = (desiredVelocity - tankVelocity);
	final.Truncate(tankMaxSpeed);

	return final;
}

Vector2D B013432f_Behaviours::FleeBehaviour(Vector2D target)
{
	Vector2D desiredVelocity = Vec2DNormalize(tanksPosition - target) * tankMaxSpeed;
	Vector2D final = (desiredVelocity - tankVelocity);
	return final;
}

Vector2D B013432f_Behaviours::ArriveBehaviour(Vector2D target, double distance)
{
	if (distance > 0)
	{
		const double slowDown = 0.3;

		double neededSpeed = distance / (double(deceleration * slowDown);
		cout << "Speed: " << neededSpeed;

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