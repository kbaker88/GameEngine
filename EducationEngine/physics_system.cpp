#include "physics_system.h"

void
Phys_CalculatePosition(PhysicsObject* PhysObject)
{
	PhysObject->PrevPosition = PhysObject->Position;
	PhysObject->Acceleration = PhysObject->MoveDirection *
		PhysObject->AccelerationRate;

	PhysObject->Force = PhysObject->Mass  * PhysObject->Acceleration;

	PhysObject->Force += PhysObject->ForceSum;

	//TODO: Temporary friction, use Ordinary Differential Equations later
	v3 AirFriction = -PhysObject->Velocity;
	PhysObject->Force += AirFriction;

	PhysObject->Position =
		(0.5f * PhysObject->Force * Math_Square(SecondsPerFrame)) +
		(PhysObject->Velocity * SecondsPerFrame) +
		PhysObject->Position;

	PhysObject->Velocity = PhysObject->Force *
		SecondsPerFrame + PhysObject->Velocity;

	*PhysObject->ModelMatrix = Math_TranslateMatrix(
		Math_IdentityMatrix(), PhysObject->Position);
}

void
Phys_AddForce(PhysicsObject* PhysObject, v3 *Force)
{
	PhysObject->ForceSum += PhysObject->Mass * *Force;
}

void 
Phys_SetAccelerationRate(PhysicsObject* PhysObject, 
	float MetersPerSec)
{
	PhysObject->AccelerationRate = MetersPerSec * 
		UnitsPerMeter * SecondsPerFrame;
}

void 
Phys_BounceBack(PhysicsObject* PhysObject, v3* CollisionNormal)
{
	PhysObject->Position = PhysObject->PrevPosition;

	PhysObject->Force = PhysObject->Force - Math_InnerProduct(
		&PhysObject->Force, CollisionNormal) * *CollisionNormal;

	PhysObject->Velocity = PhysObject->Velocity - 2 * Math_InnerProduct(
		&PhysObject->Velocity, CollisionNormal) * *CollisionNormal;

	PhysObject->Position =
		(0.5f * PhysObject->Force * Math_Square(SecondsPerFrame)) +
		(PhysObject->Velocity * SecondsPerFrame) +
		PhysObject->Position;

	*PhysObject->ModelMatrix = Math_TranslateMatrix(
		Math_IdentityMatrix(), PhysObject->Position);
}

void
Phys_WallSlide(PhysicsObject* PhysObject, v3* CollisionNormal)
{
	PhysObject->Position = PhysObject->PrevPosition;

	PhysObject->Force = PhysObject->Force - Math_InnerProduct(
		&PhysObject->Force, CollisionNormal) * *CollisionNormal;

	PhysObject->Velocity = PhysObject->Velocity - Math_InnerProduct(
		&PhysObject->Velocity, CollisionNormal) * *CollisionNormal;

	PhysObject->Position =
		(0.5f * PhysObject->Force * Math_Square(SecondsPerFrame)) +
		(PhysObject->Velocity * SecondsPerFrame) +
		PhysObject->Position;

	*PhysObject->ModelMatrix = Math_TranslateMatrix(
		Math_IdentityMatrix(), PhysObject->Position);
}

void 
Phys_StopObject(PhysicsObject* PhysObject)
{
	PhysObject->Position = PhysObject->PrevPosition;
	*PhysObject->ModelMatrix = Math_TranslateMatrix(
		Math_IdentityMatrix(), PhysObject->Position);
}