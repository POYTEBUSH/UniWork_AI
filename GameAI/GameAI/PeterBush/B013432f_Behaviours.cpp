#include "B013432f_Behaviours.h"

B013432f_Behaviours::B013432f_Behaviours()
{
	tankBehaviour = NotSet;
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
	cout << "Current Velocity: " << tankVelocity.x << " " << tankVelocity.y << " " << endl;
	cout << "Current Max Speed: " << tankMaxSpeed << endl;

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
	case NotSet:
		break;
		//case Wander:
		//	WanderBehaviour();
		//	cout << "WanderBehaviour" << endl;
		//	break;
	case Seek:
		outputVelocity = SeekFleeBehaviour(mousePosition);
		moving = true;
		cout << "SeekBehaviour" << endl;
		break;
	case Flee:
		outputVelocity = (SeekFleeBehaviour(mousePosition) *-1.0f);
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

Vector2D B013432f_Behaviours::SeekFleeBehaviour(Vector2D mousePosition)
{
	Vector2D desiredVelocity = Vec2DNormalize(mousePosition - tanksPosition) * tankMaxSpeed;
	Vector2D final = (desiredVelocity - tankVelocity);
	final.Truncate(tankMaxSpeed);
	return final;
}

//Vector2D B013432f_Behaviours::FleeBehaviour(Vector2D mousePosition)
//{
//	Vector2D desiredVelocity = Vec2DNormalize(mousePosition - tanksPosition) * tankMaxSpeed;
//	Vector2D final = (desiredVelocity - tankVelocity);
//	return final;
//}

//Vector2D B013432f_Behaviours::ArriveBehaviour()
//{
//		
//}
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