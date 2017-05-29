#ifndef PHYSICS_H
#define PHYSICS_H

#include "utility.h"

struct PhysicsObject
{
	PhysicsObject() : AccelerationRate(0.0f), Mass(1.0f),
		Force(0.0f, 0.0f, 0.0f), Velocity(0.0f, 0.0f, 0.0f),
		Acceleration(0.0f, 0.0f, 0.0f), ForceSum(0.0f, 0.0f, 0.0f), 
		ModelMatrix(0) {}
	
	~PhysicsObject() {}

	m4 *ModelMatrix;
#if DATA_ORIENTED
	v3 Position, MoveDirection;
#else
	v3 *Position, *MoveDirection;
#endif // DATA_ORIENTED
	v3 Velocity, Force, ForceSum, Acceleration, PrevPosition;
	float AccelerationRate;
	float Mass;
};

static v3 Gravity = { 0.0f, -9.807f  /* * 0.5f * SecondsPerFrame*/, 0.0f };

void 
Phys_CalculatePosition(PhysicsObject* PhysObject);
void 
Phys_AddForce(PhysicsObject* PhysObject, v3 *Force);
void
Phys_SetAccelerationRate(PhysicsObject* PhysObject,
	float MetersPerSec);

void
Phys_BounceBack(PhysicsObject* PhysObject, v3* CollisionNormal);
void
Phys_WallSlide(PhysicsObject* PhysObject, v3* CollisionNormal);

void 
Phys_StopObject(PhysicsObject* PhysObject);

#endif