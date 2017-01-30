#pragma once

#include "..\BaseTank.h"
#include <SDL.h>
#include "..\Commons.h"
#include <vector>

using namespace std;

class B013432f_Behaviours
{
public:
	B013432f_Behaviours();
	~B013432f_Behaviours();

	Vector2D FindClosest(Vector2D currentPos, TankManager* tankManager);
};

