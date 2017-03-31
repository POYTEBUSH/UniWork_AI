#pragma once
#include <vector>
#include "../Collisions.h"
#include "../BaseTank.h"
#include "../ObstacleManager.h"

class B013432F_CollisionManager
{
public:
	B013432F_CollisionManager();
	~B013432F_CollisionManager();

	void FindWalls();
	vector<Rect2D> GetWalls() { return mWalls; };
	bool CheckForCollisions(B013432F_CollisionManager);

private:
	Collisions* mCollisionManager;
	ObstacleManager* mObstacleManager;

	vector<GameObject*> mObstacles;
	vector<Rect2D> mWalls;
};

