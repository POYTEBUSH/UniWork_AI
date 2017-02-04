#include "B013432f_Behaviours.h"

B013432f_Behaviours::B013432f_Behaviours()
{
	tankBehaviour = Seek;
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
	POINT p;
	if (GetCursorPos(&p))
	{
		cout << "X: " << p.x << " Y: " << p.y << endl;
	}

	mousePosition = Vector2D(p.x, p.y);
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
		//case Wander:
		//	WanderBehaviour();
		//	cout << "WanderBehaviour" << endl;
		//	break;
		case Seek:
			_steering->SeekBehaviour(mousePosition);
			cout << "SeekBehaviour" << endl;
			break;
		//case Flee:
		//	FleeBehaviour();
		//	cout << "FleeBehaviour" << endl;
		//	break;
		//case Arrive:
		//	ArriveBehaviour();
		//	cout << "ArriveBehaviour" << endl;
		//	break;
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