#pragma once
#include "../WaypointManager.h"
#include "../Waypoint.h"
#include <SDL.h>
#include "..\Commons.h"

using namespace std;

class b013432f_PathFinding
{
public:
	b013432f_PathFinding();
	~b013432f_PathFinding();
	void Update();
	void OutputPositions();

	void SetCurrentWaypoint();
	Waypoint* GetCurrentWaypoint()	{ return currentWaypoint;}

private:
	vector<Waypoint*> mMapwaypoints;
	Waypoint* currentWaypoint;

};

