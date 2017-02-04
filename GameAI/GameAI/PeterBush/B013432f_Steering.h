#pragma once
#include "..\BaseTank.h"
#include <SDL.h>
#include "..\Commons.h"
#include "..\TankManager.h"

class B013432f_Steering
{
public:
	B013432f_Steering();
	~B013432f_Steering();

	Vector2D SeekBehaviour(Vector2D mousePosition);
	Vector2D FleeBehaviour();
	Vector2D ArriveBehaviour();
	Vector2D PursuitBehaviour();
	Vector2D EvadeBehaviour();
	Vector2D WanderBehaviour();
	Vector2D ObstacleAvoidanceBehaviour();
	Vector2D WallAvoidanceBehaviour();
	Vector2D InterposeBehaviour();
	Vector2D HideBehaviour();
	Vector2D PathFollowBehaviour();
	Vector2D OffsetPusuitBehaviour();

private:
	Vector2D ObtainVelocity(Vector2D currentVelocity); //used to obtain the current velocity
	Vector2D ObtainPosition(Vector2D currentPos); //used to obtain the current position
	double ObtainMaxSpeed(double maxSpeed); //used to obtain the max speed
};

