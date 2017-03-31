#include "B013432F_CollisionManager.h"

B013432F_CollisionManager::B013432F_CollisionManager()
{
	mCollisionManager	= Collisions::Instance();
	mObstacleManager	= ObstacleManager::Instance();
	mObstacles = mObstacleManager->GetObstacles(); 
	FindWalls();
}

B013432F_CollisionManager::~B013432F_CollisionManager()
{
	//mObstacles.clear();
}

void B013432F_CollisionManager::FindWalls()
{

	//Rect2D wallL = Rect2D(4, 5, 43, 6);

	Rect2D wallL = Rect2D(1, 1, 30, 638);
	Rect2D wallR = Rect2D(928, 1, 30, 638);
	Rect2D wallT = Rect2D(30, 1, 898, 30);
	Rect2D wallB = Rect2D(30, 935, 898, 30);

	Rect2D building1 = Rect2D(125, 255, 260, 130);
	Rect2D building2 = Rect2D(605, 255, 260, 130);

	mWalls.push_back(wallL);
	mWalls.push_back(wallR);
	mWalls.push_back(wallT);
	mWalls.push_back(wallB);
	mWalls.push_back(building1);
	mWalls.push_back(building2);
}

bool B013432F_CollisionManager::CheckForCollisions(B013432F_CollisionManager)
{
	return false;
}
