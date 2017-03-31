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
	Rect2D wallL(1, 1, 30, 638);
	Rect2D wallR(928, 1, 30, 638);
	Rect2D wallT(30, 1, 898, 30);
	Rect2D wallB(30, 935, 898, 30);

	mWalls.push_back(wallL);
	mWalls.push_back(wallR);
	mWalls.push_back(wallT);
	mWalls.push_back(wallB);
}

bool B013432F_CollisionManager::CheckForCollisions(B013432F_CollisionManager)
{
	return false;
}
