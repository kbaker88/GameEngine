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
			if (EntityBlocks[BlockNumber].BlockEntities->CameraObj)
			{
				delete[] EntityBlocks[BlockNumber].BlockEntities->CameraObj;
				EntityBlocks[BlockNumber].BlockEntities->CameraObj = NULL;
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

int32 Entity_Create(uint32 EntityBlockNumber, uint32 IDNumber, uint32 ObjectBlockNumber, uint32 ObjectID, v3 *Position)
{
	if (IDNumber < EntityBlocks[EntityBlockNumber].BlockSize)
	{
		if (EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].Active == 0)
		{
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectID = ObjectID;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectPtr =
				Object_GetObjectPtr(ObjectBlockNumber, ObjectID);
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ModelMatrix =
				TranslateMatrix(IdentityMatrix(), *Position);
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].Position =
				*Position;

			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].PhysicsObj.Position =
				&EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].Position;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].PhysicsObj.MoveDirection =
				&EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].DirectionVector;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.Position =
				&EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].Position;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.NumVertices =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectPtr->NumberOfVertices;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.VerticesPtr =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectPtr->VerticeFloatArrayPtr;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.Width =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectPtr->Width;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.Height =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectPtr->Height;
			EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].CollisionObj.Depth =
				EntityBlocks[EntityBlockNumber].BlockEntities[IDNumber].ObjectPtr->Depth;

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

void Entity_AddCamera(uint32 BlockNumber, uint32 IDNumber, Camera* NewCamera)
{
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].CameraObj = 
		NewCamera;
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].CameraObj->
		SetPosition(&EntityBlocks[BlockNumber].BlockEntities[IDNumber].Position);
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].CameraObj->
		SetProjectionMatrix(1);
}

Camera* Entity_GetCamera(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].CameraObj;
}

void Entity_Draw(uint32 BlockNumber, uint32 IDNumber, uint32 ShaderVariableID)
{
	if (EntityBlocks[BlockNumber].BlockEntities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(ShaderVariableID, 44,
			(float*)&EntityBlocks[BlockNumber].BlockEntities[IDNumber].ModelMatrix, 1, 0);
		EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectPtr->Draw();
	}
}

void Entity_DrawPolyGonMode(uint32 BlockNumber, uint32 IDNumber, uint32 ShaderVariableID)
{
	if (EntityBlocks[BlockNumber].BlockEntities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(ShaderVariableID, 44,
			(float*)&EntityBlocks[BlockNumber].BlockEntities[IDNumber].ModelMatrix, 1, 0);
		EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectPtr->Draw(1);
	}
}

Entity* Entity_Ptr(uint32 BlockNumber, uint32 IDNumber)
{
	return &EntityBlocks[BlockNumber].BlockEntities[IDNumber];
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

Object* Entity_GetObjectPtr(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectPtr;
}

float Entity_GetWidth(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectPtr->Width;
}

float Entity_GetHeight(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectPtr->Height;
}

float Entity_GetDepth(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectPtr->Depth;
}

v3 Entity_GetPosition(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].BlockEntities[IDNumber].Position;
}

void Entity_SetPosition(uint32 BlockNumber, uint32 IDNumber, v3& Position)
{
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].Position = Position;
}

void Entity_UpdatePosition(uint32 BlockNumber, uint32 IDNumber)
{
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].ModelMatrix.Rc[3][0] =
		EntityBlocks[BlockNumber].BlockEntities[IDNumber].Position.x;
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].ModelMatrix.Rc[3][1] =
		EntityBlocks[BlockNumber].BlockEntities[IDNumber].Position.y;
	EntityBlocks[BlockNumber].BlockEntities[IDNumber].ModelMatrix.Rc[3][2] = 
		EntityBlocks[BlockNumber].BlockEntities[IDNumber].Position.z;
}