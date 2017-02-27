#include "b013432f_PathFinding.h"

b013432f_PathFinding::b013432f_PathFinding()
{
	mMapwaypoints = WaypointManager::Instance()->GetAllWaypoints();
}

b013432f_PathFinding::~b013432f_PathFinding()
{

}

void b013432f_PathFinding::OutputPositions()
{
	for (int i = 0; i < mMapwaypoints.size(); i++)
	{
		Vector2D output = mMapwaypoints[i]->GetPosition();
		cout << output.x << " " << output.y << endl;
	}
}

//Hold vector to closest waypoint
//Only hold the vector for the connected waypoints

void b013432f_PathFinding::FindClosestWaypoint();

void b013432f_PathFinding::SetCurrentWaypoint()
{


	for (int i = 0; i < mMapwaypoints.size(); i++)
	{
		Vector2D output = mMapwaypoints[i]->GetPosition();
		cout << output.x << " " << output.y << endl;
	}
}