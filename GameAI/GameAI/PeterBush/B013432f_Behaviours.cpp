#include "B013432f_Behaviours.h"

B013432f_Behaviours::B013432f_Behaviours()
{
	tankBehaviour = Wander;
}

B013432f_Behaviours::~B013432f_Behaviours()
{

}

//Vector2D B013432f_Behaviours::FindClosest(Vector2D currentPos, TankManager* tankManager, bool Can)
//{
//	vector<BaseTank*> Tanks = {tankManager->GetVisibleTanks(_baseT)};
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

void B013432f_Behaviours::ChooseBehaviour(SDL_Event e)
{
	cout << "test"<< endl;

	switch (e.type)
	{
		//Deal with keyboard input.
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_1:
			tankBehaviour = Wander;
			break;
		case SDLK_2:
			tankBehaviour = Seek;
			break;
		case SDLK_3:
			tankBehaviour = Flee;
			break;
		case SDLK_4:
			tankBehaviour = Arrive;
			break;		
		}
	}

	switch (tankBehaviour)
	{
		case Wander:
			WanderBehaviour();
			break;
		case Seek:
			SeekBehaviour();
			break;
		case Flee:
			FleeBehaviour();
			break;
		case Arrive:
			ArriveBehaviour();
			break;
		case Pursuit:
			PursuitBehaviour();
			break;
		case Evade:
			EvadeBehaviour();
			break;
		case ObstacleAvoidance:
			ObstacleAvoidanceBehaviour();
			break;
		case WallAvoidance:
			WallAvoidanceBehaviour();
			break;
		case Interpose:
			InterposeBehaviour();
			break;
		case Hide:
			HideBehaviour();
			break;
		case PathFollow:
			PathFollowBehaviour();
			break;
		case OffsetPusuit:
			OffsetPusuitBehaviour();
			break;
	}
}

void B013432f_Behaviours::SeekBehaviour()
{
	cout << "SeekBehaviour" << endl;
}
void B013432f_Behaviours::FleeBehaviour()
{
	cout << "FleeBehaviour" << endl;
}
void B013432f_Behaviours::ArriveBehaviour()
{
	cout << "ArriveBehaviour" << endl;
}
void B013432f_Behaviours::PursuitBehaviour()
{
	cout << "PursuitBehaviour" << endl;
}
void B013432f_Behaviours::EvadeBehaviour()
{
	cout << "EvadeBehaviour" << endl;
}
void B013432f_Behaviours::WanderBehaviour()
{
	cout << "WanderBehaviour" << endl;
}
void B013432f_Behaviours::ObstacleAvoidanceBehaviour()
{
	cout << "ObstacleAvoidanceBehaviour" << endl;
}
void B013432f_Behaviours::WallAvoidanceBehaviour()
{
	cout << "WallAvoidanceBehaviour" << endl;
}
void B013432f_Behaviours::InterposeBehaviour()
{
	cout << "InterposeBehaviour" << endl;
}
void B013432f_Behaviours::HideBehaviour()
{
	cout << "HideBehaviour" << endl;
}
void B013432f_Behaviours::PathFollowBehaviour()
{
	cout << "PathFollowBehaviour" << endl;
}
void B013432f_Behaviours::OffsetPusuitBehaviour()
{
	cout << "OffsetPusuitBehaviour" << endl;
}