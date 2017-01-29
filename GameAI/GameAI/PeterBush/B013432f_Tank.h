#ifndef B013432FTANK_H
#define B013432FTANK_H

#include "..\BaseTank.h"
#include <SDL.h>
#include "..\Commons.h"

//---------------------------------------------------------------

class B013432f_Tank : public BaseTank
{
	//---------------------------------------------------------------
public:
	B013432f_Tank(SDL_Renderer* renderer, TankSetupDetails details);
	~B013432f_Tank();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);

	void RotateHeadingByRadian(double radian, int sign);

	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);

private:
	TURN_DIRECTION  mTankTurnDirection;
	bool			mTankTurnKeyDown;
	MOVE_DIRECTION  mTankMoveDirection;
	bool			mTankMoveKeyDown;
	TURN_DIRECTION  mManTurnDirection;
	bool			mManKeyDown;
	bool			mFireKeyDown;
};

//---------------------------------------------------------------

#endif //B013432FTANK_H