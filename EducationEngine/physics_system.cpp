#include "physics_system.h"

void Phys_CalculatePosition(PhysicsObject* PhysObject)
{
//	PhysObject->Acceleration = *PhysObject->MoveDirection *
		//PhysObject->AccelerationRate;
	PhysObject->Force = PhysObject->Mass  * *PhysObject->MoveDirection *
		PhysObject->AccelerationRate;
		//PhysObject->Mass * PhysObject->Acceleration;

	PhysObject->Force += PhysObject->ForceSum;
	
	//TODO: Temporary Friction, Use Ordinary Differential Equations later
	v3 AirFriction = -PhysObject->Velocity;
	PhysObject->Force += AirFriction;
	
	*PhysObject->Position =
		(0.5f * PhysObject->Force * Square(SecondsPerFrame)) +
		(PhysObject->Velocity * SecondsPerFrame) +
		*PhysObject->Position;
	
	PhysObject->Velocity =
		PhysObject->Force *
		SecondsPerFrame + 
		PhysObject->Velocity;
}

void Phys_AddForce(PhysicsObject* PhysObject, v3 *Force)
{
	PhysObject->ForceSum += PhysObject->Mass * *Force;
}

void Phys_RemoveForce(PhysicsObject* PhysObject, v3 *Force)
{
	PhysObject->ForceSum -= PhysObject->Mass * *Force;
}

void Phys_SetMoveDirection(PhysicsObject* PhysObject, v3 *Direction)
{
	*PhysObject->MoveDirection = *Direction;
}

void Phys_SetAccelerationRate(PhysicsObject* PhysObject, float MetersPerSec)
{
	PhysObject->AccelerationRate = MetersPerSec * UnitsPerMeter * SecondsPerFrame;
}