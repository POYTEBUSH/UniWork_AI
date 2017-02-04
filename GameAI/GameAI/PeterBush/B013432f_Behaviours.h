#pragma once

#include "..\BaseTank.h"
#include <SDL.h>
#include "..\Commons.h"
#include <vector>
#include "..\TankManager.h"

using namespace std;

enum BehaviourType
{
	Seek,				//Seeks to find where the target is going
	Flee,				//Tries to run in the complete opposite way the target is going
	Arrive,				//What to do when the tank has arrived at a point
	Pursuit,			//Chasing after a tank
	Evade,				//Running away from a tank with no set direction
	Wander,				//Free movement	
	ObstacleAvoidance,	//Stops the tank hitting into Buildings and structures
	WallAvoidance,		//Stops the tank hitting into the border / walls
	Interpose,			//Attempts to find the distance that two or more targets are heading towards
	Hide,				//Tries to put an object between itself and the target
	PathFollow,			//Follows a set path
	OffsetPusuit		//Tries to chase the target at an offset position
};

class B013432f_Behaviours
{
public:
	B013432f_Behaviours();
	~B013432f_Behaviours();

	Vector2D FindClosest(Vector2D currentPos, TankManager* tankManager, bool canBeSeen);

	void ChooseBehaviour(SDL_Event e);
	void SeekBehaviour();
	void FleeBehaviour();
	void ArriveBehaviour();
	void PursuitBehaviour();
	void EvadeBehaviour();
	void WanderBehaviour();
	void ObstacleAvoidanceBehaviour();
	void WallAvoidanceBehaviour();
	void InterposeBehaviour();
	void HideBehaviour();
	void PathFollowBehaviour();
	void OffsetPusuitBehaviour();

private:
	BehaviourType tankBehaviour;
};
