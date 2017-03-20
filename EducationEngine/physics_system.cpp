#include "physics_system.h"

void Phys_CalculatePosition(PhysicsObject* Object)
{
	Object->Acceleration =
		Object->Mass *
		Object->MoveDirection *
		Object->AccelerationRate;
	
	Object->Acceleration +=
		Object->ForceSum;
	
	//TODO: Temporary Friction, Use Ordinary Differential Equations later
	v3 AirFriction = -Object->Velocity;
	Object->Acceleration += AirFriction;
	
	*Object->Position =
		(0.5f * Object->Acceleration * Square(SecondsPerFrame)) +
		(Object->Velocity * SecondsPerFrame) +
		*Object->Position;
	
	Object->Velocity =
		Object->Acceleration *
		SecondsPerFrame + 
		Object->Velocity;
}

void Phys_AddForce(PhysicsObject* Object, v3 *Force)
{
	Object->ForceSum += Object->Mass * *Force;
}

void Phys_RemoveForce(PhysicsObject* Object, v3 *Force)
{
	Object->ForceSum -= Object->Mass * *Force;
}


void Phys_SetMoveDirection(PhysicsObject* Object, v3 &Direction)
{
	Object->MoveDirection = Direction;
}

void Pysc_SetAccelerationRate(PhysicsObject* Object, float MetersPerSec)
{
	Object->AccelerationRate = MetersPerSec * UnitsPerMeter * SecondsPerFrame;
}