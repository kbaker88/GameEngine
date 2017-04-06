#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include "collision_system.h"
#include "clock.h"  // TODO: Move this later. 
#include "camera.h" // TODO: Move this later. 
#include "shaders.h" //TODO: Remove this later.

struct Entity
{
	Entity() : EntityID(0), Active(0), CameraObj(0),
		DirectionVector(0.0f, 0.0f, 0.0f), Position(0.0f, 0.0f, 0.0f),
		ObjectPtr(0) {}
	
	~Entity() {}

	Object* ObjectPtr;
	PhysicsObject PhysicsObj;
	CollisionObject CollisionObj;
	Camera *CameraObj;
	m4 ModelMatrix;
	v3 DirectionVector, Position;
	uint32 EntityID, ObjectID;
	bool Active;
};

// Entity Creation
void Entity_CreateBlock(uint32 BlockNumber, uint32 Size);
void Entity_DeleteBlock(uint32 BlockNumber);

int32 Entity_Create(uint32 EntityBlockNumber, uint32 IDNumber, v3 *Position);
int32 Entity_Create(uint32 EntityBlockNumber, uint32 IDNumber, 
	uint32 ObjectBlockNumber, uint32 ObjectID, v3 *Position);

// Entity Add-Ons
void Entity_AddCamera(uint32 BlockNumber, uint32 IDNumber, 
	Camera* NewCamera);

// Entity Draw
void Entity_Draw(uint32 BlockNumber, uint32 IDNumber, 
	uint32 ShaderVariableID);
void Entity_DrawPolyGonMode(uint32 BlockNumber, uint32 IDNumber, 
	uint32 ShaderVariableID);

// Entity Get Values
Entity* Entity_Ptr(uint32 BlockNumber, uint32 IDNumber);
uint32 Entity_GetObjectID(uint32 BlockNumber, uint32 IDNumber);
Camera* Entity_GetCamera(uint32 BlockNumber, uint32 IDNumber);
CollisionObject* Entity_GetCollisionObjPtr(uint32 BlockNumber, 
	uint32 IDNumber);
PhysicsObject* Entity_GetPhysObjPtr(uint32 BlockNumber, 
	uint32 IDNumber);
Object* Entity_GetObjectPtr(uint32 BlockNumber, uint32 IDNumber);
float Entity_GetWidth(uint32 BlockNumber, uint32 IDNumber);
float Entity_GetHeight(uint32 BlockNumber, uint32 IDNumber);
float Entity_GetDepth(uint32 BlockNumber, uint32 IDNumber);
v3 Entity_GetPosition(uint32 BlockNumber, uint32 IDNumber);

// Entity Set Values
void Entity_SetPosition(uint32 BlockNumber, uint32 IDNumber, 
	v3& Position);
//TODO: UpdatePosition must be used with matrix transformations, fix later
void Entity_UpdatePosition(uint32 BlockNumber, uint32 IDNumber);

#endif