#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H

#include "collision_system.h"

//TODO: Allow creation of entities without an object
//TODO: Should entities be allowed to add textures? Object Specific?
struct Entity
{
	Entity() : EntityID(0), Active(0)
	{}
	
	~Entity()
	{}

	uint32 EntityID, ObjectID;
	ObjectInstance ObjectInst;
	PhysicsObject PhysicsObj;
	CollisionObject CollisionObj;
	bool Active;
};

void Entity_CreateBlock(uint32 BlockNumber, uint32 Size);
void Entity_DeleteBlock(uint32 BlockNumber);

int32 Entity_Create(uint32 EntityBlockNumber, uint32 IDNumber, uint32 ObjectBlockNumber, uint32 ObjectID, v3 &Position);
void Entity_AddTexture(uint32 BlockNumber, uint32 IDNumber, TextureStorage* Texture);
void Entity_Draw(uint32 BlockNumber, uint32 IDNumber, uint32 ShaderVariableID);
void Entity_DrawPolyGonMode(uint32 BlockNumber, uint32 IDNumber, uint32 ShaderVariableID);

uint32 Entity_GetObjectID(uint32 BlockNumber, uint32 IDNumber);
CollisionObject* Entity_GetCollisionObjPtr(uint32 BlockNumber, uint32 IDNumber);
PhysicsObject* Entity_GetPhysObjPtr(uint32 BlockNumber, uint32 IDNumber);
ObjectInstance* Entity_GetObjInstancePtr(uint32 BlockNumber, uint32 IDNumber);
float Entity_GetWidth(uint32 BlockNumber, uint32 IDNumber);
float Entity_GetHeight(uint32 BlockNumber, uint32 IDNumber);
float Entity_GetDepth(uint32 BlockNumber, uint32 IDNumber);
v3 Entity_GetPosition(uint32 BlockNumber, uint32 IDNumber);
void Entity_SetPosition(uint32 BlockNumber, uint32 IDNumber, v3& Position);
void Entity_UpdatePosition(uint32 BlockNumber, uint32 IDNumber, v3 &Position); //TODO: Must be used with matrix transformations, fix later

#endif