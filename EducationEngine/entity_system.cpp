#include "entity_system.h"

struct EntityBlock
{
	EntityBlock() : BlockEntities(NULL), BlockSize(0) {}

	Entity* BlockEntities;
	uint32 BlockSize;
};

//TODO: Find a better spot for this
EntityBlock EntityBlocks[3];

void Entity_CreateBlock(uint32 BlockNumber, uint32 Size)
{

	if (EntityBlocks[BlockNumber].BlockEntities == NULL)
	{
		EntityBlocks[BlockNumber].BlockEntities = new Entity[Size];
		EntityBlocks[BlockNumber].BlockSize = Size;
	}
	else
	{
		//TODO: Error, block already exists
	}
}

void Entity_DeleteBlock(uint32 BlockNumber)
{
	if (EntityBlocks[BlockNumber].BlockEntities != NULL)
	{
		for (uint32 i = 0; i < EntityBlocks[BlockNumber].BlockSize; i++)
		{
			if (EntityBlocks[BlockNumber].BlockEntities->PlayerPtr != NULL)
			{
				delete EntityBlocks[BlockNumber].BlockEntities->PlayerPtr;
				EntityBlocks[BlockNumber].BlockEntities->PlayerPtr = NULL;;
			}
		}
		delete[] EntityBlocks[BlockNumber].BlockEntities;
		EntityBlocks[BlockNumber].BlockEntities = NULL;
		EntityBlocks[BlockNumber].BlockSize = 0;
	}
	else
	{
		//TODO: Error, Already Empty
	}
}

int32 Entity_Create(uint32 EntityBlockNumber, uint32 IDNumber, uint32 ObjectBlockNumber, uint32 ObjectID, v3 &Position)
{
	if (IDNumber < EntityBlocks[EntityBlockNumber].BlockSize)
	{
		if (EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].Active == 0)
		{
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectID = ObjectID;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr =
				Object_GetObjectPtr(ObjectBlockNumber, ObjectID);
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.ModelMatrix =
				TranslateMatrix(IdentityMatrix(), Position);
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.Position =
				Position;

			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].PhysicsObj.Position =
				&EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.Position;

			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.Position =
				&EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.Position;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.NumVertices =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->NumberOfVertices;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.VerticesPtr =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->VerticeFloatArrayPtr;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.Width =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Width;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.Height =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Height;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.Depth =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Depth;

			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].Active = 1;

			return 1;
		}
		else
		{
			//TODO: Error, already active entity
			return 0;
		}
	}
	else
	{
		//TODO: Error, Size out of bounds
		return -1;
	}
}

void Entity_CreatePlayer(uint32 EntityBlockNumber, uint32 IDNumber, Player* NewPlayer)
{
	if (EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].PlayerPtr == NULL)
	{
		EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].PlayerPtr = NewPlayer;
	}
	else
	{
		//TODO: Already Player assigned here
	}
}

void Entity_AddTexture(uint32 BlockNumber, uint32 IDNumber, TextureStorage* Texture)
{
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->InputTexture(Texture);
}

void Entity_Draw(uint32 BlockNumber, uint32 IDNumber, uint32 ShaderVariableID)
{
	if (EntityBlocks[BlockNumber].BlockEntities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(3, ShaderVariableID,
			(float*)&EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ModelMatrix);
		EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Draw();
	}
}

void Entity_DrawPolyGonMode(uint32 BlockNumber, uint32 IDNumber, uint32 ShaderVariableID)
{
	if (EntityBlocks[BlockNumber].BlockEntities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(3, ShaderVariableID,
			(float*)&EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ModelMatrix);
		EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Draw(1);
	}
}

uint32 Entity_GetObjectID(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectID;
}

CollisionObject* Entity_GetCollisionObjPtr(uint32 BlockNumber, uint32 IDNumber)
{
	return &EntityBlocks[BlockNumber].BlockEntities[IDNumber].CollisionObj;
}

PhysicsObject* Entity_GetPhysObjPtr(uint32 BlockNumber, uint32 IDNumber)
{
	return &EntityBlocks[BlockNumber].BlockEntities[IDNumber].PhysicsObj;
}

ObjectInstance* Entity_GetObjInstancePtr(uint32 BlockNumber, uint32 IDNumber)
{
	return &EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst;
}

Player* Entity_GetPlayerPtr(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].PlayerPtr;
}

float Entity_GetWidth(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Width;
}

float Entity_GetHeight(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Height;
}

float Entity_GetDepth(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Depth;
}

v3 Entity_GetPosition(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.Position;
}

void Entity_SetPosition(uint32 BlockNumber, uint32 IDNumber, v3& Position)
{
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.Position = Position;
}

void Entity_UpdatePosition(uint32 BlockNumber, uint32 IDNumber, v3 &Position)
{
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ModelMatrix.Rc[3][0] = Position.x;
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ModelMatrix.Rc[3][1] = Position.y;
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ModelMatrix.Rc[3][2] = Position.z;
}