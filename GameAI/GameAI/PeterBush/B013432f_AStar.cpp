#include "B013432f_AStar.h"

B013432f_AStar::B013432f_AStar()
{
	SetEdgeCost();
}

B013432f_AStar::~B013432f_AStar()
{
	mEdgeCostList.clear();
	mOpenNodes.clear();
	mClosedNodes.clear();
}

void B013432f_AStar::SetEdgeCost()
{
	vector<Waypoint*> allWaypoints = mWaypointManager->Instance()->GetAllWaypoints();

	for (Waypoint* waypoints : allWaypoints)
	{
		int currentID = waypoints->GetID();
		vector<int> connectedIDs = waypoints->GetConnectedWaypointIDs();

		for each (auto waypointsID in connectedIDs)
		{
			Waypoint*	currentWaypoint = mWaypointManager->Instance()->GetWaypointWithID(currentID);
			Waypoint*	targetWaypoint = mWaypointManager->Instance()->GetWaypointWithID(waypointsID);
			double		cost = targetWaypoint->GetPosition().Distance(currentWaypoint->GetPosition());

			mEdgeCostList.push_back(new EdgeCost(currentWaypoint, targetWaypoint, cost));
		}
	}
}

bool B013432f_AStar::IsInList(vector<AStarNode*> listToCheck, Waypoint* waypointToCheck) 
{
	for each (auto nodes in listToCheck)
	{
		if (nodes->thisWaypoint == waypointToCheck)
			return true;
	}
	return false;
}

Waypoint* B013432f_AStar::GetNearestWaypoint(Vector2D tankPos)
{
	vector<Waypoint*> allWaypoints = mWaypointManager->Instance()->GetAllWaypoints();
	float maxDistance = MaxFloat;

	Waypoint* closestWaypoint = NULL;

	for(int i = 0; i < allWaypoints.size(); i++)
	{
		Vector2D vecBetweenPoints = tankPos - allWaypoints[i]->GetPosition();
		float distance = vecBetweenPoints.Length();

		if (distance < maxDistance)
		{
			maxDistance = distance;
			closestWaypoint = allWaypoints[i];
		}
	}
	return closestWaypoint;
}

double B013432f_AStar::GetHeuristicCost(Vector2D startPos, Vector2D endPos)
{
	return startPos.Distance(endPos);
}

double B013432f_AStar::GetCostBetweenWaypoints(Waypoint* from, Waypoint* to)
{
	for each (auto edgeCost in mEdgeCostList)
	{
		if (edgeCost->waypointFROM == from && edgeCost->waypointTO == to)
			return edgeCost->cost;
	}
	return MaxDouble;
}

vector<Vector2D> B013432f_AStar::ConstructedPath(AStarNode* targetNode, Vector2D postion)
{
	vector<Vector2D> path;
	vector<Vector2D> pathInReverse;

	pathInReverse.push_back = targetNode->thisWaypoint->GetPosition();

	AStarNode* currentNode = targetNode;

	while (currentNode != NULL)
	{
		pathInReverse.push_back(currentNode->thisWaypoint->GetPosition());
		currentNode = currentNode->parentWaypoint;
	}

	vector <Vector2D>::iterator iter = pathInReverse.end();
	while (iter != pathInReverse.begin())
		path.push_back(*(--iter));

	return path;
}

vector<Vector2D> B013432f_AStar::GetPathBetweenPoint(Vector2D tankPos, Vector2D endPos)
{
	mOpenNodes.clear();
	mClosedNodes.clear();

	vector<Vector2D> path;

	Waypoint* nearestToTank = GetNearestWaypoint(tankPos);
	Waypoint* nearestToEnd = GetNearestWaypoint(endPos);

	if ((nearestToTank || nearestToEnd == NULL) || (nearestToTank == nearestToEnd))
	{
		path.push_back(endPos);
		return path;
	}
	mOpenNodes.push_back(new AStarNode(nearestToTank, NULL, 0.0f));

	AStarNode* currentNode;
	while (mOpenNodes.size() != 0)
	{
		for each (auto nodes in mOpenNodes)
		{
			if (currentNode == NULL || nodes->cost <= currentNode->cost)
				currentNode = nodes;
		}

		if (currentNode->thisWaypoint == nearestToEnd)
		{
			ConstructedPath(currentNode, endPos);
			return path;
		}

		vector<int> connectedIDs = currentNode->thisWaypoint->GetConnectedWaypointIDs();

		for each (auto waypointsID in connectedIDs)
		{
			Waypoint*	targetWaypoint = mWaypointManager->Instance()->GetWaypointWithID(waypointsID);
			double		cost = targetWaypoint->GetPosition().Distance(currentWaypoint->GetPosition());

			mOpenNodes.push_back(new);
		}
	}
}