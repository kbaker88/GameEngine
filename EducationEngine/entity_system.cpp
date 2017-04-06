#include "entity_system.h"

struct EntityBlock
{
	EntityBlock() : Entities(0), BlockSize(0) {}

	Entity* Entities;
	uint32 BlockSize;
};

//TODO: Find a better spot for this
EntityBlock EntityBlocks[3];

void Entity_CreateBlock(uint32 BlockNumber, uint32 Size)
{

	if (EntityBlocks[BlockNumber].Entities == 0)
	{
		EntityBlocks[BlockNumber].Entities = new Entity[Size];
		EntityBlocks[BlockNumber].BlockSize = Size;
	}
	else
	{
		//TODO: Error, block already exists
	}
}

void Entity_DeleteBlock(uint32 BlockNumber)
{
	if (EntityBlocks[BlockNumber].Entities != 0)
	{
		for (uint32 i = 0; i < EntityBlocks[BlockNumber].BlockSize; i++)
		{
			if (EntityBlocks[BlockNumber].Entities->CameraObj)
			{
				delete[] EntityBlocks[BlockNumber].Entities->CameraObj;
				EntityBlocks[BlockNumber].Entities->CameraObj = 0;
			}
		}
		delete[] EntityBlocks[BlockNumber].Entities;
		EntityBlocks[BlockNumber].Entities = 0;
		EntityBlocks[BlockNumber].BlockSize = 0;
	}
	else
	{
		//TODO: Error, Already Empty
	}
}

int32 Entity_Create(uint32 BlockNumber, uint32 IDNumber, 
	uint32 ObjectBlockNumber, uint32 ObjectID, v3 *Position)
{
	if (IDNumber < EntityBlocks[BlockNumber].BlockSize)
	{
		if (EntityBlocks[BlockNumber].Entities[IDNumber].Active == 0)
		{
			EntityBlocks[BlockNumber].Entities[IDNumber].ObjectID = ObjectID;
			EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr =
				Object_GetObjectPtr(ObjectBlockNumber, ObjectID);
			EntityBlocks[BlockNumber].Entities[IDNumber].ModelMatrix =
				Math_TranslateMatrix(Math_IdentityMatrix(), *Position);
			EntityBlocks[BlockNumber].Entities[IDNumber].Position =
				*Position;

			EntityBlocks[BlockNumber].Entities[IDNumber].PhysicsObj.Position =
				&EntityBlocks[BlockNumber].Entities[IDNumber].Position;
			EntityBlocks[BlockNumber].Entities[IDNumber].PhysicsObj.MoveDirection =
				&EntityBlocks[BlockNumber].Entities[IDNumber].DirectionVector;

			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Position =
				&EntityBlocks[BlockNumber].Entities[IDNumber].Position;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.NumVertices =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->NumberOfVertices;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.VerticesPtr =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->VerticeFloatArrayPtr;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Width =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Width;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Height =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Height;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Depth =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Depth;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.HalfWidth =
				EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Width * 0.5f;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.HalfHeight =
				EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Height * 0.5f;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.HalfDepth =
				EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Depth * 0.5f;

			EntityBlocks[BlockNumber].Entities[IDNumber].Active = 1;

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

int32 Entity_Create(uint32 BlockNumber, uint32 IDNumber, v3 *Position)
{
	if (IDNumber < EntityBlocks[BlockNumber].BlockSize)
	{
		if (EntityBlocks[BlockNumber].Entities[IDNumber].Active == 0)
		{
			EntityBlocks[BlockNumber].Entities[IDNumber].ModelMatrix =
				Math_TranslateMatrix(Math_IdentityMatrix(), *Position);
			EntityBlocks[BlockNumber].Entities[IDNumber].Position =
				*Position;

			EntityBlocks[BlockNumber].Entities[IDNumber].PhysicsObj.Position =
				&EntityBlocks[BlockNumber].Entities[IDNumber].Position;
			EntityBlocks[BlockNumber].Entities[IDNumber].PhysicsObj.MoveDirection =
				&EntityBlocks[BlockNumber].Entities[IDNumber].DirectionVector;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Position =
				&EntityBlocks[BlockNumber].Entities[IDNumber].Position;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.NumVertices =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->NumberOfVertices;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.VerticesPtr =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->VerticeFloatArrayPtr;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Width =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Width;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Height =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Height;
			EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj.Depth =
				EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Depth;

			EntityBlocks[BlockNumber].Entities[IDNumber].Active = 1;

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
	EntityBlocks[BlockNumber].Entities[IDNumber].CameraObj = 
		NewCamera;
	EntityBlocks[BlockNumber].Entities[IDNumber].CameraObj->
		SetPosition(&EntityBlocks[BlockNumber].Entities[IDNumber].Position);
	EntityBlocks[BlockNumber].Entities[IDNumber].CameraObj->
		SetProjectionMatrix(1);
}

Camera* Entity_GetCamera(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].Entities[IDNumber].CameraObj;
}

void Entity_Draw(uint32 BlockNumber, uint32 IDNumber, uint32 ShaderVariableID)
{
	if (EntityBlocks[BlockNumber].Entities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(ShaderVariableID, 44,
			(float*)&EntityBlocks[BlockNumber].Entities[IDNumber].ModelMatrix, 1, 0);
		EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Draw();
	}
}

void Entity_DrawPolyGonMode(uint32 BlockNumber, uint32 IDNumber, uint32 ShaderVariableID)
{
	if (EntityBlocks[BlockNumber].Entities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(ShaderVariableID, 44,
			(float*)&EntityBlocks[BlockNumber].Entities[IDNumber].ModelMatrix, 1, 0);
		EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Draw(1);
	}
}

Entity* Entity_Ptr(uint32 BlockNumber, uint32 IDNumber)
{
	return &EntityBlocks[BlockNumber].Entities[IDNumber];
}

uint32 Entity_GetObjectID(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].Entities[IDNumber].ObjectID;
}

CollisionObject* Entity_GetCollisionObjPtr(uint32 BlockNumber, uint32 IDNumber)
{
	return &EntityBlocks[BlockNumber].Entities[IDNumber].CollisionObj;
}

PhysicsObject* Entity_GetPhysObjPtr(uint32 BlockNumber, uint32 IDNumber)
{
	return &EntityBlocks[BlockNumber].Entities[IDNumber].PhysicsObj;
}

Object* Entity_GetObjectPtr(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr;
}

float Entity_GetWidth(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Width;
}

float Entity_GetHeight(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Height;
}

float Entity_GetDepth(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].Entities[IDNumber].ObjectPtr->Depth;
}

v3 Entity_GetPosition(uint32 BlockNumber, uint32 IDNumber)
{
	return EntityBlocks[BlockNumber].Entities[IDNumber].Position;
}

void Entity_SetPosition(uint32 BlockNumber, uint32 IDNumber, v3& Position)
{
	EntityBlocks[BlockNumber].Entities[IDNumber].Position = Position;
}

void Entity_UpdatePosition(uint32 BlockNumber, uint32 IDNumber)
{
	EntityBlocks[BlockNumber].Entities[IDNumber].ModelMatrix.Rc[3][0] =
		EntityBlocks[BlockNumber].Entities[IDNumber].Position.x;
	EntityBlocks[BlockNumber].Entities[IDNumber].ModelMatrix.Rc[3][1] =
		EntityBlocks[BlockNumber].Entities[IDNumber].Position.y;
	EntityBlocks[BlockNumber].Entities[IDNumber].ModelMatrix.Rc[3][2] = 
		EntityBlocks[BlockNumber].Entities[IDNumber].Position.z;
}