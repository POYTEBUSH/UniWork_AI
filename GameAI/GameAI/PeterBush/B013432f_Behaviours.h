#pragma once

#include "..\BaseTank.h"
#include <SDL.h>
#include "..\Commons.h"
#include "..\TankManager.h"
#include "..\ObstacleManager.h"
#include "..\Collisions.h"

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

	#pragma region Tank Variables
	
	Vector2D mousePosition;
	Vector2D tanksPosition;
	Vector2D tankVelocity;
	Vector2D tankHeading;
	Vector2D tankFrontPoint;
	Vector2D tankBackPoint;
	double tankCurrentSpeed;
	double tankMaxSpeed;
	Vector2D outputVelocity;
	bool moving;
	bool pursuit;
	bool targetBool;
	double distance;
	double deceleration = 2.0;

	#pragma endregion

	#pragma region Tank Locators
	//used in obstacle avoidance
	Vector2D minDetectionBoxLength = Vector2D(5.0, 5.0);
	Vector2D detectionBoxLength;
	vector<Vector2D> feelers{ Vector2D(0,0),Vector2D(1,1),Vector2D(0,0) };
	//used in wall avoidance
	double wallDetectionFeelerLength = 40.0;
	#pragma endregion

	#pragma region Behaviour Methods

	void TargetClosest(TankManager* tankManager, vector <BaseTank*>);

	void ChooseBehaviour(SDL_Event e);
	void GetMousePos();

	double DistanceFromTargetCheck(Vector2D target); //Iplemented
	Vector2D SeekBehaviour(Vector2D target); //Iplemented
	Vector2D FleeBehaviour(Vector2D target); //Iplemented
	Vector2D ArriveBehaviour(Vector2D target, double distance); //Iplemented
	Vector2D PursuitBehaviour(BaseTank* evader); //Implemented
	Vector2D EvadeBehaviour(BaseTank* evader);
	Vector2D WanderBehaviour();
	Vector2D ObstacleAvoidanceBehaviour(vector<Vector2D> feelers);
	Vector2D WallAvoidanceBehaviour();
	Vector2D InterposeBehaviour();
	Vector2D HideBehaviour();
	Vector2D PathFollowBehaviour();
	Vector2D OffsetPusuitBehaviour();
#pragma endregion

private:
	BehaviourType tankBehaviour;
	BaseTank* _closestTank;
	BaseTank* _baseTank;
	
	int speed;

	Vector2D target;
	Vector2D targetPursuit;
	Vector2D avoidTarget;
};