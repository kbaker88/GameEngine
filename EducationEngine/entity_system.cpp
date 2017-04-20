#include "entity_system.h"

void Entity_CreateBlock(EntityBlock* Block, uint32 Size)
{
	if (!Block)
	{
		//TODO: Error
	}
	else
	{
		if (!Block->Entities)
		{
			Block->Entities = new Entity[Size];
			Block->BlockSize = Size;
		}
		{
			//TODO: Error, block already exists
		}
	}
}

void Entity_DeleteBlock(EntityBlock* Block)
{
	if (Block->Entities != 0)
	{
		for (uint32 i = 0; i < Block->BlockSize; i++)
		{
			if (Block->Entities->CameraObj)
			{
				delete Block->Entities->CameraObj;
				Block->Entities->CameraObj = 0;
			}
		}
		delete[] Block->Entities;
		Block->Entities = 0;
		Block->BlockSize = 0;
	}
	else
	{
		//TODO: Error, Already Empty
	}
}

int32 Entity_Create(EntityBlock* Block, uint32 IDNumber,
	ObjectBlock* ObjBlock, uint32 ObjectID, v3 *Position)
{
	if (IDNumber < Block->BlockSize)
	{
		if (Block->Entities[IDNumber].Active == 0)
		{
			Block->Entities[IDNumber].ObjectID = ObjectID;
			Block->Entities[IDNumber].ObjectPtr =
				Object_GetObjectPtr(ObjBlock, ObjectID);
			Block->Entities[IDNumber].ModelMatrix =
				Math_TranslateMatrix(Math_IdentityMatrix(), *Position);
			Block->Entities[IDNumber].Position =
				*Position;

			Block->Entities[IDNumber].PhysicsObj.Position =
				&Block->Entities[IDNumber].Position;
			Block->Entities[IDNumber].PhysicsObj.MoveDirection =
				&Block->Entities[IDNumber].DirectionVector;

			Block->Entities[IDNumber].CollisionObj.Position =
				&Block->Entities[IDNumber].Position;
			Block->Entities[IDNumber].CollisionObj.ModelMatrix =
				&Block->Entities[IDNumber].ModelMatrix;
			Block->Entities[IDNumber].CollisionObj.NumVertices =
				Block->Entities[IDNumber].ObjectPtr->NumberOfVertices;
			Block->Entities[IDNumber].CollisionObj.VerticesPtr =
				Block->Entities[IDNumber].ObjectPtr->VerticeFloatArrayPtr;
			Block->Entities[IDNumber].CollisionObj.Width =
				Block->Entities[IDNumber].ObjectPtr->Width;
			Block->Entities[IDNumber].CollisionObj.Height =
				Block->Entities[IDNumber].ObjectPtr->Height;
			Block->Entities[IDNumber].CollisionObj.Depth =
				Block->Entities[IDNumber].ObjectPtr->Depth;
			Block->Entities[IDNumber].CollisionObj.HalfWidth =
				Block->Entities[IDNumber].CollisionObj.Width * 0.5f;
			Block->Entities[IDNumber].CollisionObj.HalfHeight =
				Block->Entities[IDNumber].CollisionObj.Height * 0.5f;
			Block->Entities[IDNumber].CollisionObj.HalfDepth =
				Block->Entities[IDNumber].CollisionObj.Depth * 0.5f;

			Block->Entities[IDNumber].Active = 1;

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

int32 Entity_Create(EntityBlock* Block, uint32 IDNumber, v3 *Position)
{
	if (IDNumber < Block->BlockSize)
	{
		if (Block->Entities[IDNumber].Active == 0)
		{
			Block->Entities[IDNumber].ModelMatrix =
				Math_TranslateMatrix(Math_IdentityMatrix(), *Position);
			Block->Entities[IDNumber].Position =
				*Position;

			Block->Entities[IDNumber].PhysicsObj.Position =
				&Block->Entities[IDNumber].Position;
			Block->Entities[IDNumber].PhysicsObj.MoveDirection =
				&Block->Entities[IDNumber].DirectionVector;
			Block->Entities[IDNumber].CollisionObj.Position =
				&Block->Entities[IDNumber].Position;
			Block->Entities[IDNumber].CollisionObj.NumVertices =
				Block->Entities[IDNumber].ObjectPtr->NumberOfVertices;
			Block->Entities[IDNumber].CollisionObj.VerticesPtr =
				Block->Entities[IDNumber].ObjectPtr->VerticeFloatArrayPtr;
			Block->Entities[IDNumber].CollisionObj.Width =
				Block->Entities[IDNumber].ObjectPtr->Width;
			Block->Entities[IDNumber].CollisionObj.Height =
				Block->Entities[IDNumber].ObjectPtr->Height;
			Block->Entities[IDNumber].CollisionObj.Depth =
				Block->Entities[IDNumber].ObjectPtr->Depth;

			Block->Entities[IDNumber].Active = 1;

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

void Entity_AddCamera(EntityBlock* Block, uint32 IDNumber, 
	Camera* NewCamera)
{
	Block->Entities[IDNumber].CameraObj = 
		NewCamera;
	Block->Entities[IDNumber].CameraObj->
		SetPosition(&Block->Entities[IDNumber].Position);
	Block->Entities[IDNumber].CameraObj->
		SetProjectionMatrix(1);
}

Camera* Entity_GetCamera(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].CameraObj;
}

void Entity_Draw(EntityBlock* Block, uint32 IDNumber, 
	uint32 ShaderVariableID)
{
	if (Block->Entities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(ShaderVariableID, 44,
			(float*)&Block->Entities[IDNumber].ModelMatrix, 1, 0);
		Block->Entities[IDNumber].ObjectPtr->Draw();
	}
}

void Entity_DrawPolyGonMode(EntityBlock* Block, uint32 IDNumber, 
	uint32 ShaderVariableID)
{
	if (Block->Entities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(ShaderVariableID, 44,
			(float*)&Block->Entities[IDNumber].ModelMatrix, 1, 0);
		Block->Entities[IDNumber].ObjectPtr->Draw(1);
	}
}

Entity* Entity_Ptr(EntityBlock* Block, uint32 IDNumber)
{
	return &Block->Entities[IDNumber];
}

uint32 Entity_GetObjectID(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].ObjectID;
}

CollisionObject* Entity_GetCollisionObjPtr(EntityBlock* Block,
	uint32 IDNumber)
{
	return &Block->Entities[IDNumber].CollisionObj;
}

PhysicsObject* Entity_GetPhysObjPtr(EntityBlock* Block, uint32 IDNumber)
{
	return &Block->Entities[IDNumber].PhysicsObj;
}

Object* Entity_GetObjectPtr(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].ObjectPtr;
}

float Entity_GetWidth(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].ObjectPtr->Width;
}

float Entity_GetHeight(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].ObjectPtr->Height;
}

float Entity_GetDepth(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].ObjectPtr->Depth;
}

v3 Entity_GetPosition(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].Position;
}

void Entity_SetPosition(EntityBlock* Block, uint32 IDNumber,
	v3& Position)
{
	Block->Entities[IDNumber].Position = Position;
}

void Entity_UpdatePosition(EntityBlock* Block, uint32 IDNumber)
{
	Block->Entities[IDNumber].ModelMatrix.Rc[3][0] =
		Block->Entities[IDNumber].Position.x;
	Block->Entities[IDNumber].ModelMatrix.Rc[3][1] =
		Block->Entities[IDNumber].Position.y;
	Block->Entities[IDNumber].ModelMatrix.Rc[3][2] = 
		Block->Entities[IDNumber].Position.z;
}