#ifndef B013432FTANK_H
#define B013432FTANK_H

#include "B013432f_Behaviours.h"
#include "B013432f_PathFinding.h"

//---------------------------------------------------------------

class B013432f_Tank : public BaseTank
{
	//---------------------------------------------------------------
public:
	B013432f_Tank(SDL_Renderer* renderer, TankSetupDetails details);
	~B013432f_Tank();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);
	void UpdateMovement();
	void Render();
	void CalcFeelers();

	//TankState GetState()const { return _tankState; };

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
	double			mFeelerRadius;
	vector<Vector2D> feelers {Vector2D(0,0),Vector2D(1,1),Vector2D(0,0), Vector2D(0,0), Vector2D(0,0)};

	B013432f_Behaviours* _tankBehaviour = new B013432f_Behaviours;

	//B013432f_Behaviours*	_tankBehaviour;
	BaseTank*				_baseTank;
	TankManager*			_tankManager;
	b013432f_PathFinding*	mPathFinder = new b013432f_PathFinding();

	//TankState				_tankState;

	Vector2D				_currentVelocity;
};

//---------------------------------------------------------------

#endif //B013432FTANK_H