#ifndef PHYSICS_H
#define PHYSICS_H

//#include "utility.h"
#include "memory_management.h"

struct PhysicsObject
{
	PhysicsObject() : AccelerationRate(1.0f), Mass(1.0f),
		Force(0.0f, 0.0f, 0.0f), Velocity(0.0f, 0.0f, 0.0f),
		Acceleration(0.0f, 0.0f, 0.0f), ForceSum(0.0f, 0.0f, 0.0f), 
		MoveDirection(NULL), Position(NULL) {}
	
	~PhysicsObject() {}

	v3 *Position, *MoveDirection;
	v3 Velocity, Force, ForceSum, Acceleration;
	float AccelerationRate;
	float Mass;
};

static v3 Gravity = { 0.0f, -9.807f  * 0.5f * SecondsPerFrame, 0.0f };

void Phys_CalculatePosition(PhysicsObject* PhysObject);
void Phys_AddForce(PhysicsObject* PhysObject, v3 *Force);
void Phys_RemoveForce(PhysicsObject* PhysObject, v3 *Force);
void Phys_SetAccelerationRate(PhysicsObject* PhysObject, float MetersPerSec);

//TODO: Is this needed anymore now that movedirection is a ptr?
void Phys_SetMoveDirection(PhysicsObject* PhysObject, v3 *Direction);

#endif