#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include "collision_system.h"
#include "clock.h"  // TODO: Move this later. 
#include "camera.h" // TODO: Move this later. 
#include "shaders.h" //TODO: Remove this later.

struct Entity
{
	Entity() : EntityID(0), Active(0), CameraObj(0),
		DirectionVector(0.0f, 0.0f, 0.0f), 
		Position(0.0f, 0.0f, 0.0f), ObjectPtr(0), 
		State(0) {}
	
	~Entity() {}

	Object* ObjectPtr;
	PhysicsObject PhysicsObj;
	CollisionObject CollisionObj;
	Camera *CameraObj;
	m4 ModelMatrix;
	v3 DirectionVector, Position;
	uint32 EntityID, ObjectID, State;
	bool Active;
};

struct EntityBlock
{
	EntityBlock() : Entities(0), BlockSize(0) {}
	~EntityBlock() {}

	Entity* Entities;
	uint32 BlockSize;
};

// Entity Creation
void Entity_CreateBlock(EntityBlock* Block, uint32 Size);
void Entity_DeleteBlock(EntityBlock* Block);

int32 Entity_Create(EntityBlock* Block, uint32 IDNumber, 
	v3 *Position);
int32 Entity_Create(EntityBlock* Block, uint32 IDNumber,
	ObjectBlock* ObjBlock, uint32 ObjectID, v3 *Position);

// Entity Add-Ons
void Entity_AddCamera(EntityBlock* Block, uint32 IDNumber,
	Camera* NewCamera);

// Entity Draw
void Entity_Draw(EntityBlock* Block, uint32 IDNumber,
	uint32 ShaderVariableID);
void Entity_DrawPolyGonMode(EntityBlock* Block, uint32 IDNumber,
	uint32 ShaderVariableID);

// Entity Get Values
Entity* Entity_Ptr(EntityBlock* Block, uint32 IDNumber);
uint32 Entity_GetObjectID(EntityBlock* Block, uint32 IDNumber);
Camera* Entity_GetCamera(EntityBlock* Block, uint32 IDNumber);
CollisionObject* Entity_GetCollisionObjPtr(EntityBlock* Block,
	uint32 IDNumber);
PhysicsObject* Entity_GetPhysObjPtr(EntityBlock* Block,
	uint32 IDNumber);
Object* Entity_GetObjectPtr(EntityBlock* Block, uint32 IDNumber);
float Entity_GetWidth(EntityBlock* Block, uint32 IDNumber);
float Entity_GetHeight(EntityBlock* Block, uint32 IDNumber);
float Entity_GetDepth(EntityBlock* Block, uint32 IDNumber);
v3 Entity_GetPosition(EntityBlock* Block, uint32 IDNumber);

// Entity Set Values
void Entity_SetPosition(EntityBlock* Block, uint32 IDNumber,
	v3& Position);
//TODO: UpdatePosition must be used with matrix transformations, fix later
void Entity_UpdatePosition(EntityBlock* Block, uint32 IDNumber);

#endif