#include "B013432f_AStar.h"

B013432f_AStar::B013432f_AStar()
{
	mWaypointManager = WaypointManager::Instance();
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
	auto maxDistance = MaxFloat;

	Waypoint* closestWaypoint = nullptr;

	for(auto i = 0; i < allWaypoints.size(); i++)
	{
		Vector2D vecBetweenPoints = allWaypoints[i]->GetPosition() - tankPos;
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

vector<Vector2D> B013432f_AStar::ConstructedPath(AStarNode* targetNode, Vector2D endPos)
{
	vector<Vector2D> path;
	vector<Vector2D> pathInReverse;

	pathInReverse.push_back(endPos);

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

	if (nearestToTank == nullptr || nearestToEnd == nullptr || nearestToTank == nearestToEnd)
	{
		path.push_back(endPos);
		return path;
	}

	mOpenNodes.push_back(new AStarNode(nearestToTank, nullptr, 0.0f));
	AStarNode* currentNode = nullptr;

	while (!mOpenNodes.empty())
	{
		for (auto i = 0; i < mOpenNodes.size(); i++)
		{
			if (currentNode == nullptr || mOpenNodes[i]->cost < currentNode->cost)
				currentNode = mOpenNodes[i];
		}

		if (currentNode->thisWaypoint == nearestToEnd)
		{
			cout << "=====~~ Path Finished ~~=====" << endl;
			path = ConstructedPath(currentNode, endPos);
			return path;
		}

		vector<int> connectedIDs = currentNode->thisWaypoint->GetConnectedWaypointIDs();
		Waypoint*	targetWaypoint;

		for each (auto waypointsID in connectedIDs)
		{
			targetWaypoint = mWaypointManager->Instance()->GetWaypointWithID(waypointsID);

			if (!IsInList(mOpenNodes, mWaypointManager->Instance()->GetWaypointWithID(waypointsID)) || !IsInList(mClosedNodes, mWaypointManager->Instance()->GetWaypointWithID(waypointsID)))
			{
				double g = currentNode->cost + GetCostBetweenWaypoints(currentNode->thisWaypoint, targetWaypoint);
				double h = GetHeuristicCost(targetWaypoint->GetPosition(), endPos);
				double f = g + h;
				mOpenNodes.push_back(new AStarNode(targetWaypoint, currentNode, f));
			}
		}

		mClosedNodes.push_back(currentNode);

		vector<AStarNode*>::iterator iter = mOpenNodes.begin();
		while (iter != mOpenNodes.end())
		{
			if (*iter == currentNode)
				iter = mOpenNodes.erase(iter);
			else
				++iter;
		}

		currentNode = NULL;
	}
	return path;
}