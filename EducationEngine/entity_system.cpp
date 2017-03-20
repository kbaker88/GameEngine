#include "entity_system.h"

struct EntityBlock
{
	EntityBlock() : BlockEntities(NULL), BlockSize(0) {}

	Entity* BlockEntities;
	uint32 BlockSize;
};

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
		delete[] EntityBlocks[BlockNumber].BlockEntities;
		EntityBlocks[BlockNumber].BlockEntities = NULL;
		EntityBlocks[BlockNumber].BlockSize = 0;
	}
	else
	{
		//TODO: Error, Already Empty
	}
}

int32 Entity_Create(uint32 BlockNumber, uint32 IDNumber, uint32 ObjectID, v3 &Position)
{
	if (IDNumber < EntityBlocks[BlockNumber].BlockSize)
	{
		if (EntityBlocks[BlockNumber].BlockEntities[IDNumber].Active == 0)
		{
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectID = ObjectID;
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr = Object_Find(ObjectID);
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ModelMatrix = TranslateMatrix(IdentityMatrix(),
				v3(Position.x, Position.y, Position.z));
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.Position = Position;
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ID = IDNumber;

			EntityBlocks[BlockNumber].BlockEntities[IDNumber].PhysicsObj.Position = 
				&EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.Position;

			EntityBlocks[BlockNumber].BlockEntities[IDNumber].CollisionObj.Position = 
				&EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.Position;
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].CollisionObj.NumVertices =
				EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->NumberOfVertices;
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].CollisionObj.VerticesPtr =
				EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->VerticeFloatArrayPtr;
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].CollisionObj.Width =
				EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Width;
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].CollisionObj.Height =
				EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Height;
			EntityBlocks[BlockNumber].BlockEntities[IDNumber].CollisionObj.Depth =
				EntityBlocks[BlockNumber].BlockEntities[IDNumber].ObjectInst.ObjectPtr->Depth;

			EntityBlocks[BlockNumber].BlockEntities[IDNumber].Active = 1;

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