#ifndef PHYSICS_H
#define PHYSICS_H

//#include "utility.h"
#include "memory_management.h"

struct PhysicsObject
{
	PhysicsObject() : AccelerationRate(1.0f), Mass(1.0f),
		Acceleration(v3(0.0f, 0.0f, 0.0f)), Velocity(v3(0.0f, 0.0f, 0.0f)),
		ForceSum(v3(0.0f, 0.0f, 0.0f)), MoveDirection(v3(0.0f, 0.0f, 0.0f))
	{}
	
	~PhysicsObject() {}

	v3 *Position;
	v3 Velocity;
	v3 Acceleration;
	v3 ForceSum;
	v3 MoveDirection;
	float AccelerationRate;
	float Mass;
};

static v3 Gravity = { 0.0f, -9.807f  * 0.5f * SecondsPerFrame, 0.0f };

void Phys_CalculatePosition(PhysicsObject* Object);
void Phys_AddForce(PhysicsObject* Object, v3 *Force);
void Phys_RemoveForce(PhysicsObject* Object, v3 *Force);

void Phys_SetMoveDirection(PhysicsObject* Object, v3 &Direction);
void Pysc_SetAccelerationRate(PhysicsObject* Object, float MetersPerSec);

#endif