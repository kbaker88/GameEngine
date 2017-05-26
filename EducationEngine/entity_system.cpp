#include "entity_system.h"

void 
Entity_CreateBlock(EntityBlock* Block, uint32 Size)
{
	if (!Block)
	{
		//TODO: Error
	}
	else
	{
		if (!Block->Entities)
		{
#if MEMORY_ON
			Block->Entities = (Entity*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(Entity) * Size);
#else
			Block->Entities = new Entity[Size];
			Block->BlockSize = Size;
#endif
		}
		{
			//TODO: Error, block already exists
		}
	}
}

void 
Entity_DeleteBlock(EntityBlock* Block)
{
	if (Block->Entities != 0)
	{
#if MEMORY_ON
		for (uint32 i = 0; i < Block->BlockSize; i++)
		{
			// TODO: Make some cleanup function.
		}
#else
		for (uint32 i = 0; i < Block->BlockSize; i++)
		{
			unsigned short ObjectType = 0;
			ObjectType = (Block->Entities[i].ObjectTypes & 0xF);
			if (ObjectType)
			{
				delete[] Block->Entities[i].RenderObjPtrArray;
				Block->Entities[i].RenderObjPtrArray = 0;
			}
			ObjectType = ((Block->Entities[i].ObjectTypes & 0xF0) >> 4);
			if (ObjectType)
			{
				delete[] Block->Entities[i].PhysicsObj;
				Block->Entities[i].PhysicsObj = 0;
			}
			ObjectType = ((Block->Entities[i].ObjectTypes & 0xF00) >> 8);
			if (ObjectType)
			{
				delete[] Block->Entities[i].CollisionObj;
				Block->Entities[i].CollisionObj = 0;
			}
			ObjectType = ((Block->Entities[i].ObjectTypes & 0xF000) >> 12);
			if (ObjectType)
			{
				delete[] Block->Entities[i].CameraObj;
				Block->Entities[i].CameraObj = 0;
			}
			Block->Entities[i].ObjectTypes = 0;
		}
		delete[] Block->Entities;
		Block->Entities = 0;
		Block->BlockSize = 0;
#endif
	}
	else
	{
		//TODO: Error, Already Empty
	}
}

#if DATA_ORIENTED
int32 
Entity_Create(EntityBlock* Block, uint32 IDNumber,
	RenderObj* RenderObjects, v3 *Position, 
	Model* ModelObj, float Width, float Height, float Depth,
	uint64 TypesOfObjects)
{
	if ((IDNumber < Block->BlockSize) &&
		(!Block->Entities[IDNumber].Active))
	{
		unsigned short ObjectType = 0;

		Block->Entities[IDNumber].EntityID = IDNumber;
		Block->Entities[IDNumber].ObjectTypes = TypesOfObjects;
		Block->Entities[IDNumber].Position =
			*Position;
		Block->Entities[IDNumber].ModelMatrix =
			Math_TranslateMatrix(Math_IdentityMatrix(),
				Block->Entities[IDNumber].Position);

		ObjectType = (TypesOfObjects & 0xF);
		if (ObjectType)
		{
#if MEMORY_ON
			Block->Entities[IDNumber].RenderObjPtrArray =
				(Entity**)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(Entity*) * ObjectType);
#else
			Block->Entities[IDNumber].RenderObjPtrArray =
				new RenderObj*[ObjectType];
#endif
			// NOTE: Potential error point if user enters an ObjectType
			//		 with a count greater than there are actual pointers;
			// TODO: Fix this error point
			for (uint32 Index = 0; Index < ObjectType; Index++)
			{
				Block->Entities[IDNumber].RenderObjPtrArray[Index] =
					&RenderObjects[Index];
			}
			if (ObjectType == 1)
			{
				Block->Entities[IDNumber].Dimensions.x = Width;
				Block->Entities[IDNumber].Dimensions.y = Height;
				Block->Entities[IDNumber].Dimensions.z = Depth;
			}
		}
		ObjectType = ((TypesOfObjects & 0xF0) >> 4);
		if (ObjectType)
		{
#if MEMORY_ON
			Block->Entities[IDNumber].PhysicsObj =
				(PhysicsObject*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(PhysicsObject) * ObjectType);
#else
			Block->Entities[IDNumber].PhysicsObj =
				new PhysicsObject[ObjectType];
#endif
			for (uint32 Index = 0; Index < ObjectType; Index++)
			{
				Block->Entities[IDNumber].PhysicsObj[Index].Position =
					&Block->Entities[IDNumber].Position;
				Block->Entities[IDNumber].PhysicsObj[Index].MoveDirection =
					&Block->Entities[IDNumber].DirectionVector;
				Block->Entities[IDNumber].PhysicsObj[Index].ModelMatrix =
					&Block->Entities[IDNumber].ModelMatrix;
			}
		}
		ObjectType = ((TypesOfObjects & 0xF00) >> 8);
		if (ObjectType)
		{
#if MEMORY_ON
			Block->Entities[IDNumber].CollisionObj =
				(CollisionObject*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(CollisionObject) * ObjectType);
#else
			Block->Entities[IDNumber].CollisionObj =
				new CollisionObject[ObjectType];
#endif
			for (uint32 Index = 0; Index < ObjectType; Index++)
			{
				Block->Entities[IDNumber].CollisionObj[Index].Position =
					&Block->Entities[IDNumber].Position;
				Block->Entities[IDNumber].CollisionObj[Index].ModelMatrix =
					&Block->Entities[IDNumber].ModelMatrix;

				if (Block->Entities[IDNumber].RenderObjPtrArray)
				{
					Block->Entities[IDNumber].CollisionObj[Index].NumVertices =
						RenderObjects->NumVertices;
					Block->Entities[IDNumber].CollisionObj[Index].VerticesPtr =
						ModelObj->Data[0];
					Block->Entities[IDNumber].CollisionObj[Index].Width = Width;
					Block->Entities[IDNumber].CollisionObj[Index].Height = Height;
					Block->Entities[IDNumber].CollisionObj[Index].Depth = Depth;
					Block->Entities[IDNumber].CollisionObj[Index].HalfWidth =
						Block->Entities[IDNumber].CollisionObj[Index].Width * 0.5f;
					Block->Entities[IDNumber].CollisionObj[Index].HalfHeight =
						Block->Entities[IDNumber].CollisionObj[Index].Height * 0.5f;
					Block->Entities[IDNumber].CollisionObj[Index].HalfDepth =
						Block->Entities[IDNumber].CollisionObj[Index].Depth * 0.5f;
				}
			}
		}
		ObjectType = ((TypesOfObjects & 0xF000) >> 12);
		if (ObjectType)
		{
#if MEMORY_ON
			Block->Entities[IDNumber].CameraObj =
				(Camera*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(Camera) * ObjectType);
#else
			Block->Entities[IDNumber].CameraObj = new Camera[ObjectType];
#endif

			for (uint32 Index = 0; Index < ObjectType; Index++)
			{
				Block->Entities[IDNumber].CameraObj[Index].
					SetPosition(&Block->Entities[IDNumber].Position);
				Block->Entities[IDNumber].CameraObj[Index].
					SetProjectionMatrix(1);
			}
		}

		Block->Entities[IDNumber].Active = 1;

		return 1;
	}
	else
	{
		if (IDNumber < Block->BlockSize)
		{
			//TODO: Error, Size out of bounds
			return -1;
		}
		else
		{
			//TODO: Error, Already active
			return 0;
		}
	}
}

#else
int32 
Entity_Create(EntityBlock* Block, uint32 IDNumber,
	RenderObject* ObjectPtrs, v3 *Position, uint64 TypesOfObjects)
{
	if ((IDNumber < Block->BlockSize) &&
		(!Block->Entities[IDNumber].Active))
	{
		unsigned short ObjectType = 0;

		Block->Entities[IDNumber].EntityID = IDNumber;
		Block->Entities[IDNumber].ObjectTypes = TypesOfObjects;
		Block->Entities[IDNumber].Position =
			*Position;
		Block->Entities[IDNumber].ModelMatrix =
			Math_TranslateMatrix(Math_IdentityMatrix(),
				Block->Entities[IDNumber].Position);

		ObjectType = (TypesOfObjects & 0xF);
		if (ObjectType)
		{
#if MEMORY_ON
			Block->Entities[IDNumber].RenderObjPtrArray = 
				(Entity**)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(Entity*) * ObjectType);
#else
			Block->Entities[IDNumber].RenderObjPtrArray = 
				new RenderObject*[ObjectType];
#endif
			// NOTE: Potential error point if user enters an ObjectType
			//		 with a count greater than there are actual pointers;
			// TODO: Fix this error point
			for (uint32 Index = 0; Index < ObjectType; Index++)
			{
				Block->Entities[IDNumber].RenderObjPtrArray[Index] =
					&ObjectPtrs[Index];
			}
			if (ObjectType == 1)
			{
				Block->Entities[IDNumber].Dimensions.x = 
					Block->Entities[IDNumber].RenderObjPtrArray[0]->Width;
				Block->Entities[IDNumber].Dimensions.y =
					Block->Entities[IDNumber].RenderObjPtrArray[0]->Height;
				Block->Entities[IDNumber].Dimensions.z =
					Block->Entities[IDNumber].RenderObjPtrArray[0]->Depth;
			}
		}
		ObjectType = ((TypesOfObjects & 0xF0) >> 4);
		if (ObjectType)
		{
#if MEMORY_ON
			Block->Entities[IDNumber].PhysicsObj = 
				(PhysicsObject*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(PhysicsObject) * ObjectType);
#else
			Block->Entities[IDNumber].PhysicsObj =
				new PhysicsObject[ObjectType];
#endif
			for (uint32 Index = 0; Index < ObjectType; Index++)
			{
				Block->Entities[IDNumber].PhysicsObj[Index].Position =
					&Block->Entities[IDNumber].Position;
				Block->Entities[IDNumber].PhysicsObj[Index].MoveDirection =
					&Block->Entities[IDNumber].DirectionVector;
				Block->Entities[IDNumber].PhysicsObj[Index].ModelMatrix =
					&Block->Entities[IDNumber].ModelMatrix;
			}
		}
		ObjectType = ((TypesOfObjects & 0xF00) >> 8);
		if (ObjectType)
		{
#if MEMORY_ON
			Block->Entities[IDNumber].CollisionObj =
				(CollisionObject*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(CollisionObject) * ObjectType);
#else
			Block->Entities[IDNumber].CollisionObj =
				new CollisionObject[ObjectType];
#endif
			for (uint32 Index = 0; Index < ObjectType; Index++)
			{
				Block->Entities[IDNumber].CollisionObj[Index].Position =
					&Block->Entities[IDNumber].Position;
				Block->Entities[IDNumber].CollisionObj[Index].ModelMatrix =
					&Block->Entities[IDNumber].ModelMatrix;

				if (Block->Entities[IDNumber].RenderObjPtrArray)
				{
					Block->Entities[IDNumber].CollisionObj[Index].NumVertices =
						Block->Entities[IDNumber].RenderObjPtrArray[0]->NumberOfVertices;
					Block->Entities[IDNumber].CollisionObj[Index].VerticesPtr =
						Block->Entities[IDNumber].RenderObjPtrArray[0]->VerticeFloatArrayPtr;
					Block->Entities[IDNumber].CollisionObj[Index].Width =
						Block->Entities[IDNumber].RenderObjPtrArray[0]->Width;
					Block->Entities[IDNumber].CollisionObj[Index].Height =
						Block->Entities[IDNumber].RenderObjPtrArray[0]->Height;
					Block->Entities[IDNumber].CollisionObj[Index].Depth =
						Block->Entities[IDNumber].RenderObjPtrArray[0]->Depth;
					Block->Entities[IDNumber].CollisionObj[Index].HalfWidth =
						Block->Entities[IDNumber].CollisionObj[Index].Width * 0.5f;
					Block->Entities[IDNumber].CollisionObj[Index].HalfHeight =
						Block->Entities[IDNumber].CollisionObj[Index].Height * 0.5f;
					Block->Entities[IDNumber].CollisionObj[Index].HalfDepth =
						Block->Entities[IDNumber].CollisionObj[Index].Depth * 0.5f;
				}
			}
		}
		ObjectType = ((TypesOfObjects & 0xF000) >> 12);
		if (ObjectType)
		{
#if MEMORY_ON
			Block->Entities[IDNumber].CameraObj =
				(Camera*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(Camera) * ObjectType);
#else
			Block->Entities[IDNumber].CameraObj = new Camera[ObjectType];
#endif

			for (uint32 Index = 0; Index < ObjectType; Index++)
			{
				Block->Entities[IDNumber].CameraObj[Index].
					SetPosition(&Block->Entities[IDNumber].Position);
				Block->Entities[IDNumber].CameraObj[Index].
					SetProjectionMatrix(1);
			}
		}

		Block->Entities[IDNumber].Active = 1;

		return 1;
	}
	else
	{
		if (IDNumber < Block->BlockSize)
		{
			//TODO: Error, Size out of bounds
			return -1;
		}
		else
		{
			//TODO: Error, Already active
			return 0;
		}
	}
}
#endif

Camera*
Entity_GetCamera(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].CameraObj;
}

#if DATA_ORIENTED

#else
void 
Entity_Draw(EntityBlock* Block, uint32 IDNumber, 
	uint32 ShaderVariableID)
{
	if (Block->Entities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(ShaderVariableID, 44,
			(float*)&Block->Entities[IDNumber].ModelMatrix, 1, 0);
		Block->Entities[IDNumber].RenderObjPtrArray[0]->Draw();
	}
}

void 
Entity_DrawPolyGonMode(EntityBlock* Block, uint32 IDNumber, 
	uint32 ShaderVariableID)
{
	if (Block->Entities[IDNumber].Active == 1)
	{
		Render_UpdateShaderVariable(ShaderVariableID, 44,
			(float*)&Block->Entities[IDNumber].ModelMatrix, 1, 0);
		Block->Entities[IDNumber].RenderObjPtrArray[0]->Draw(1);
	}
}
#endif

Entity* 
Entity_Ptr(EntityBlock* Block, uint32 IDNumber)
{
	return &Block->Entities[IDNumber];
}

CollisionObject*
Entity_GetCollisionObjPtr(EntityBlock* Block,
	uint32 IDNumber, uint32 ObjectID)
{
	if (ObjectID < ((Block->Entities[IDNumber].ObjectTypes & 0xF00) >> 8))
	{
		return &Block->Entities[IDNumber].CollisionObj[ObjectID];
	}
	else
	{
		// TODO: Error
		return 0;
	}
}

PhysicsObject* 
Entity_GetPhysObjPtr(EntityBlock* Block, uint32 IDNumber,
	uint32 ObjectID)
{
	if (ObjectID < ((Block->Entities[IDNumber].ObjectTypes & 0xF0) >> 4))
	{
		return &Block->Entities[IDNumber].PhysicsObj[ObjectID];
	}
	else
	{
		// TODO: Error
		return 0;
	}
}

#if DATA_ORIENTED

#else
RenderObject* 
Entity_GetObjectPtr(EntityBlock* Block, uint32 IDNumber,
	uint32 ObjectID)
{
	if (ObjectID < (Block->Entities[IDNumber].ObjectTypes & 0xF))
	{
		return Block->Entities[IDNumber].RenderObjPtrArray[ObjectID];
	}
	else
	{
		// TODO: Error
		return 0;
	}
}
#endif

float
Entity_Width(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].Dimensions.x;
}

float 
Entity_Height(EntityBlock* Block, uint32 IDNumber)
{
	return  Block->Entities[IDNumber].Dimensions.y;
}

float 
Entity_Depth(EntityBlock* Block, uint32 IDNumber)
{
	return  Block->Entities[IDNumber].Dimensions.z;
}

v3 
Entity_GetPosition(EntityBlock* Block, uint32 IDNumber)
{
	return Block->Entities[IDNumber].Position;
}

void 
Entity_SetPosition(EntityBlock* Block, uint32 IDNumber,
	v3& Position)
{
	Block->Entities[IDNumber].Position = Position;
}

void 
Entity_UpdatePosition(EntityBlock* Block, uint32 IDNumber)
{
	Block->Entities[IDNumber].ModelMatrix.Rc[3][0] =
		Block->Entities[IDNumber].Position.x;
	Block->Entities[IDNumber].ModelMatrix.Rc[3][1] =
		Block->Entities[IDNumber].Position.y;
	Block->Entities[IDNumber].ModelMatrix.Rc[3][2] = 
		Block->Entities[IDNumber].Position.z;
}