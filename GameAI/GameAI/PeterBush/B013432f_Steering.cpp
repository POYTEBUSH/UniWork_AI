#include "B013432f_Steering.h"

B013432f_Steering::B013432f_Steering()
{
}

B013432f_Steering::~B013432f_Steering()
{
}

Vector2D B013432f_Steering::SeekBehaviour(Vector2D mousePosition)
{
	Vector2D desiredVelocity = Vec2DNormalize(mousePosition - tanksPosition) * tankMaxSpeed;
	Vector2D final = (desiredVelocity - tankVelocity);
	return final;
}
//Vector2D B013432f_Behaviours::FleeBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::ArriveBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::PursuitBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::EvadeBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::WanderBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::ObstacleAvoidanceBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::WallAvoidanceBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::InterposeBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::HideBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::PathFollowBehaviour()
//{
//	
//}
//Vector2D B013432f_Behaviours::OffsetPusuitBehaviour()
//{
//	
//}

Vector2D B013432f_Steering::ObtainVelocity(Vector2D currentVelocity); //used to obtain the current velocity
Vector2D B013432f_Steering::ObtainPosition(Vector2D currentPos); //used to obtain the current position
double B013432f_Steering::ObtainMaxSpeed(dobule maxSpeed); //used to obtain the max speed