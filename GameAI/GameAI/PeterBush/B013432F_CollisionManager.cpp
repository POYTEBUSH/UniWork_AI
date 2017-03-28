#include "B013432F_CollisionManager.h"

B013432F_CollisionManager::B013432F_CollisionManager()
{
	mCollisionManager	= Collisions::Instance();
	mObstacleManager	= ObstacleManager::Instance();
	mObstacles = mObstacleManager->GetObstacles();
}

B013432F_CollisionManager::~B013432F_CollisionManager()
{
	//mObstacles.clear();
}

void B013432F_CollisionManager::FindWalls()
{
	mWalls.push_back(mObstacles.at(0)->GetCentralPosition());
	mWalls.push_back(mObstacles.at(1)->GetCentralPosition());
	mWalls.push_back(mObstacles.at(2)->GetCentralPosition());
	mWalls.push_back(mObstacles.at(3)->GetCentralPosition());
}

bool B013432F_CollisionManager::CheckForCollisions(B013432F_CollisionManager)
{
	return false;
}
