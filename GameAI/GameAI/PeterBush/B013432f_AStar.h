#pragma once

#include "../WaypointManager.h"
#include "../Waypoint.h"

class B013432f_AStar
{
	struct B013432FEdgeCost
	{
		Waypoint* waypointFROM;
		Waypoint* waypointTO;
		double cost;

		B013432FEdgeCost(Waypoint* from, Waypoint* to, double newCost)
		{
			waypointFROM	= from;
			waypointTO		= to;
			cost			= newCost;
		}
	};

	struct B013432FAStarNode
	{
		Waypoint* thisWaypoint;
		B013432FAStarNode* parentWaypoint;
		double cost;

		B013432FAStarNode(Waypoint* currentWaypoint, B013432FAStarNode* parentalWaypoint, double newCost)
		{
			thisWaypoint	= currentWaypoint;
			parentWaypoint	= parentalWaypoint;
			cost			= newCost;
		}
	};

public:
	B013432f_AStar();
	~B013432f_AStar();

	vector<Vector2D>	GetPathBetweenPoint(Vector2D startPos, Vector2D endPos);
	Waypoint*			GetNearestWaypoint(Vector2D tankPos);

private:
	WaypointManager*	mWaypointManager;

	vector<B013432FEdgeCost*>	mEdgeCostList;
	vector<B013432FAStarNode*>	mOpenNodes;
	vector<B013432FAStarNode*>	mClosedNodes;

	void				SetEdgeCost();
	
	double				GetCostBetweenWaypoints(Waypoint* from, Waypoint* to);
	vector<Vector2D>	ConstructedPath(B013432FAStarNode* targetNode, Vector2D postion);
	bool				IsInList(vector<B013432FAStarNode*> listToCheck, Waypoint* waypointToCheck);
	double				GetHeuristicCost(Vector2D startPos, Vector2D endPos);
};

