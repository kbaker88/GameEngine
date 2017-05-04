#include "physics_system.h"

void Phys_CalculatePosition(PhysicsObject* PhysObject)
{
	PhysObject->PrevPosition = *PhysObject->Position;
	PhysObject->Acceleration = *PhysObject->MoveDirection *
		PhysObject->AccelerationRate;

	PhysObject->Force = PhysObject->Mass  * PhysObject->Acceleration; 

	PhysObject->Force += PhysObject->ForceSum;
	
	//TODO: Temporary friction, use Ordinary Differential Equations later
	v3 AirFriction = -PhysObject->Velocity;
	PhysObject->Force += AirFriction;
	
	*PhysObject->Position =
		(0.5f * PhysObject->Force * Math_Square(SecondsPerFrame)) +
		(PhysObject->Velocity * SecondsPerFrame) +
		*PhysObject->Position;
	
	PhysObject->Velocity = PhysObject->Force *
		SecondsPerFrame + PhysObject->Velocity;

	*PhysObject->ModelMatrix = Math_TranslateMatrix(
		Math_IdentityMatrix(), *PhysObject->Position);
}

void Phys_AddForce(PhysicsObject* PhysObject, v3 *Force)
{
	PhysObject->ForceSum += PhysObject->Mass * *Force;
}

void Phys_SetAccelerationRate(PhysicsObject* PhysObject, 
	float MetersPerSec)
{
	PhysObject->AccelerationRate = MetersPerSec * 
		UnitsPerMeter * SecondsPerFrame;
}

void Phys_StopObject(PhysicsObject* PhysObject)
{
	*PhysObject->Position = PhysObject->PrevPosition;
	*PhysObject->ModelMatrix = Math_TranslateMatrix(
		Math_IdentityMatrix(), *PhysObject->Position);
	//PhysObject->Velocity = v3(0.0f, 0.0f, 0.0f);
	//PhysObject->AccelerationRate = 0.0f;
	//PhysObject->ForceSum = v3(0.0f, 0.0f, 0.0f);
}