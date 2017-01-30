#include "B013432f_Behaviours.h"
#include "..\TankManager.h"

B013432f_Behaviours::B013432f_Behaviours()
{

}

B013432f_Behaviours::~B013432f_Behaviours()
{

}

Vector2D B013432f_Behaviours::FindClosest(Vector2D currentPos, TankManager* tankManager)
{
	vector<BaseTank> Tanks = {(tankManager->GetVisibleTanks)};
	Vector2D closestDistance = Vector2D(0.0f, 0.0f);
	Vector2D currentPosition = currentPos;

	for (int i = 0; i < Tanks.size - 1; i++)
	{
		if (Tanks[i].CanSee)
		{
			Vector2D thisDistance = Tanks[i].GetCentralPosition - currentPosition;

			if (thisDistance.Length < closestDistance.Length)
				closestDistance = thisDistance;
		}
	}
}