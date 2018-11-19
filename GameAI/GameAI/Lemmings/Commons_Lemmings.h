//------------------------------------------------------------------------
//  Author: Paul Roberts 2018
//------------------------------------------------------------------------

#pragma once

#include "../Commons.h"
#include <SDL.h>

//--------------------------------------------------------------------

enum LandscapeType_Lemmings
{
	LANDSCAPETYPE_EMPTY,
	LANDSCAPETYPE_GROUND
};

//--------------------------------------------------------------------

enum LEMMING_TYPE
{
	LEMMINGTYPE_CLIMBER,
	LEMMINGTYPE_PARACHUTE,
	LEMMINGTYPE_EXPLODER,
	LEMMINGTYPE_BLOCKER,
	LEMMINGTYPE_BUILDER,
	LEMMINGTYPE_PUNCHER,
	LEMMINGTYPE_PICKAXE,
	LEMMINGTYPE_DIGGER,
	LEMMINGTYPE_NORMAL,

	LEMMINGTYPE_MAX
};

//--------------------------------------------------------------------

enum LEMMING_STATE
{
	LEMMINGSTATE_WALK,
	LEMMINGSTATE_BLOCKING,
	LEMMINGSTATE_DIG,
	LEMMINGSTATE_FALL,
	LEMMINGSTATE_CLIMB,
	LEMMINGSTATE_CLIMBEXIT,
	LEMMINGSTATE_BUILD,
	LEMMINGSTATE_PUNCH,
	LEMMINGSTATE_PICKAXE,
	LEMMINGSTATE_DEATHFALL,
	LEMMINGSTATE_DEATHCOUNTDOWN,
	LEMMINGSTATE_DEATHEXPLOSION,
	LEMMINGSTATE_DEATHDROWN,
	LEMMINGSTATE_EXIT
};

//--------------------------------------------------------------------

enum FACING_DIRECTION
{
	FACINGDIRECTION_LEFT = 0,
	FACINGDIRECTION_RIGHT
};

//--------------------------------------------------------------------

struct PhysicsObjectSetupDetails_Lemmings
{
	Vector2D	Direction;
	double		RotationAngle;
	float		Mass;
	float		TurnRate;
	float		MaxSpeed;

	PhysicsObjectSetupDetails_Lemmings(Vector2D	dir, double angle, float mass, float turnRate, float maxSpeed)
	{
		Direction = dir;
		RotationAngle = angle;
		Mass = mass;
		TurnRate = turnRate;
		MaxSpeed = maxSpeed;
	}
};

//--------------------------------------------------------------------
//Collisions.
//--------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------

inline bool CollisionCircle(Vector2D circle1_Pos, float circle1_Radius, Vector2D circle2_Pos, float circle2_Radius)
{
	Vector2D vec = Vector2D((circle1_Pos.x - circle2_Pos.x), (circle1_Pos.y - circle2_Pos.y));
	double distance = sqrt((vec.x*vec.x) + (vec.y*vec.y));

	double combinedDistance = (circle1_Radius + circle2_Radius);
	return distance < combinedDistance;
}

//--------------------------------------------------------------------------------------------------

inline bool CollisionBox(Rect2D rect1, Rect2D rect2)
{
	if (rect1.x + (rect1.width / 2) > rect2.x &&
		rect1.x + (rect1.width / 2) < rect2.x + rect2.width &&
		rect1.y + (rect1.height / 2) > rect2.y &&
		rect1.y + (rect1.height / 2) < rect2.y + rect2.height)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------

inline bool CollisionPointInBox(Vector2D point, Rect2D rect2)
{
	if (point.x > rect2.x &&
		point.x < (rect2.x + rect2.width) &&
		point.y > rect2.y &&
		point.y < (rect2.y + rect2.height))
	{
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------------------------