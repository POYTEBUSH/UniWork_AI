#pragma once

#include "../WaypointManager.h"
#include "../Waypoint.h"

class B013432f_AStar
{
	struct EdgeCost
	{
		Waypoint* waypointFROM;
		Waypoint* waypointTO;
		double cost;

		EdgeCost(Waypoint* from, Waypoint* to, double newCost)
		{
			waypointFROM	= from;
			waypointTO		= to;
			cost			= newCost;
		}
	};

	struct AStarNode
	{
		Waypoint* thisWaypoint;
		AStarNode* parentWaypoint;
		double cost;

		AStarNode(Waypoint* currentWaypoint, AStarNode* parentalWaypoint, double newCost)
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

private:
	WaypointManager*	mWaypointManager;

	vector<EdgeCost*>	mEdgeCostList;
	vector<AStarNode*>	mOpenNodes;
	vector<AStarNode*>	mClosedNodes;

	void				SetEdgeCost();

	Waypoint*			GetNearestWaypoint(Vector2D tankPos);
	double				GetCostBetweenWaypoints(Waypoint* from, Waypoint* to);
	vector<Vector2D>	ConstructedPath(AStarNode* targetNode, Vector2D postion);
	bool				IsInList(vector<AStarNode*> listToCheck, Waypoint* waypointToCheck);
	double				GetHeuristicCost(Vector2D startPos, Vector2D endPos);
};

