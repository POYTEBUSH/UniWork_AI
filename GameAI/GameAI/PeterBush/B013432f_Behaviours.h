#pragma once

#include "..\BaseTank.h"
#include <SDL.h>
#include "..\Commons.h"
#include "..\TankManager.h"

using namespace std;

enum BehaviourType
{
	Idle,
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

	Vector2D mousePosition;
	Vector2D tanksPosition;
	Vector2D tankVelocity;
	double tankMaxSpeed;
	Vector2D outputVelocity;
	bool moving;
	double distance;
	double deceleration = 2.0;

	void TargetClosest(TankManager* tankManager, vector <BaseTank*>);

	void ChooseBehaviour(SDL_Event e);
	void GetMousePos();

	Vector2D SeekBehaviour(Vector2D target); //Iplemented
	Vector2D FleeBehaviour(Vector2D target); //Iplemented
	Vector2D ArriveBehaviour(Vector2D target, double distance); //Iplemented
	double DistanceFromTargetCheck(Vector2D target); //Iplemented
	Vector2D PursuitBehaviour(Vector2D targetPursuit); //Iplemented
	Vector2D EvadeBehaviour();
	Vector2D WanderBehaviour();
	Vector2D ObstacleAvoidanceBehaviour();
	Vector2D WallAvoidanceBehaviour();
	Vector2D InterposeBehaviour();
	Vector2D HideBehaviour();
	Vector2D PathFollowBehaviour();
	Vector2D OffsetPusuitBehaviour();

private:
	BehaviourType tankBehaviour;
	BaseTank* _closestTank;

	Vector2D target;
	Vector2D targetPursuit;
};

