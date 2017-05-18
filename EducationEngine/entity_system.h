#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include "collision_system.h"
#include "clock.h"  // TODO: Move this later. 
#include "camera_system.h" // TODO: Move this later. 
#include "shaders.h" //TODO: Remove this later.

struct Entity
{
	Entity() : EntityID(0), Active(0), CameraObj(0),
		DirectionVector(0.0f, 0.0f, 0.0f), 
		Position(0.0f, 0.0f, 0.0f), Dimensions(0.0f, 0.0f, 0.0f),
		RenderObjPtrArray(0),
		State(0), PhysicsObj(0), CollisionObj(0),
		ObjectTypes(0) {}
	
	~Entity() {}

#if DATA_ORIENTED
	RenderObj** RenderObjPtrArray;
#else
	RenderObject** RenderObjPtrArray;
#endif
	PhysicsObject* PhysicsObj;
	CollisionObject* CollisionObj;
	Camera* CameraObj;
	uint64 ObjectTypes;
	m4 ModelMatrix;
	v3 DirectionVector, Position, Dimensions;
	uint32 EntityID, State;
	bool Active;
};

struct EntityBlock
{
	EntityBlock() : Entities(0), BlockSize(0) {}
	~EntityBlock() {}

	Entity* Entities;
	uint32 BlockSize;
};

/////////////// CREATE //////////////////////////
void 
Entity_CreateBlock(EntityBlock* Block, uint32 Size);
void
Entity_DeleteBlock(EntityBlock* Block);
// NOTE: Every 4 bits of ObjectType determines which and
//		 how many of each object type will be added.
//		 Starting with lowest significance:
//		 0-3  : Objects
//		 4-7  : Physics Objects
//		 8-11 : Collision Objects
//		 12-15: Camera Objects
#if DATA_ORIENTED

#else
int32 
Entity_Create(EntityBlock* Block, uint32 IDNumber,
	RenderObject* ObjectPtrs, v3 *Position, uint64 TypesOfObjects);

/////////////// DRAW ///////////////////////////
void 
Entity_Draw(EntityBlock* Block, uint32 IDNumber,
	uint32 ShaderVariableID);
void
Entity_DrawPolyGonMode(EntityBlock* Block, uint32 IDNumber,
	uint32 ShaderVariableID);
#endif
/////////////// RETRIEVE ////////////////////////
Entity* 
Entity_Ptr(EntityBlock* Block, uint32 IDNumber);
Camera* 
Entity_GetCamera(EntityBlock* Block, uint32 IDNumber);
CollisionObject* 
Entity_GetCollisionObjPtr(EntityBlock* Block,
	uint32 IDNumber, uint32 ObjectID);
PhysicsObject*
Entity_GetPhysObjPtr(EntityBlock* Block,
	uint32 IDNumber, uint32 ObjectID);

#if DATA_ORIENTED

#else
RenderObject* 
Entity_GetObjectPtr(EntityBlock* Block, uint32 IDNumber,
	uint32 ObjectID);
#endif
float
Entity_Width(EntityBlock* Block, uint32 IDNumber);
float 
Entity_Height(EntityBlock* Block, uint32 IDNumber);
float
Entity_Depth(EntityBlock* Block, uint32 IDNumber);
v3 
Entity_GetPosition(EntityBlock* Block, uint32 IDNumber);

/////////////// SET ////////////////////////////
void 
Entity_SetPosition(EntityBlock* Block, uint32 IDNumber,
	v3& Position);
//TODO: UpdatePosition must be used with matrix transformations, fix later
void 
Entity_UpdatePosition(EntityBlock* Block, uint32 IDNumber);

#endif