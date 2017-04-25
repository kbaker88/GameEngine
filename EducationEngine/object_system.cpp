#include "object_system.h"

void Object_CreateBlock(ObjectBlock* Block, uint32 Size)
{
	if (!Block)
	{
		// TODO: Error
	}
	else
	{
		if (!Block->BlockObjects)
		{
			Block->BlockObjects = new Object*[Size] {};
			Block->BlockSize = Size;
		}
		else
		{
			//TODO: Error checking
		}
	}
}

void Object_DeleteBlock(ObjectBlock* Block)
{
	if (Block->BlockObjects != 0)
	{
		for (uint32 i = 0; i < Block->BlockSize; i++)
		{
			if (Block->BlockObjects[i])
			{
				Block->BlockObjects[i]->Delete();
				delete Block->BlockObjects[i];
			}
		}
		delete[] Block->BlockObjects;
		Block->BlockObjects = 0;
		Block->BlockSize = 0;
	}
}

void Object_Create(Object* NewObject, ObjectBlock* Block,
	uint32 ObjectID)
{
	Block->BlockObjects[ObjectID] = NewObject;
	Block->BlockObjects[ObjectID]->Init();
	Block->BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void Object_Create(Object* NewObject, ObjectBlock* Block, 
	uint32 ObjectID, TextureStorage* HeightMapTexture)
{
	Block->BlockObjects[ObjectID] = NewObject;
	Block->BlockObjects[ObjectID]->Init(HeightMapTexture);
	Block->BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void Object_Create(Object* NewObject, ObjectBlock* Block, 
	uint32 ObjectID, uint32 Width, uint32 Depth)
{
	Block->BlockObjects[ObjectID] = NewObject;
	Block->BlockObjects[ObjectID]->Init(Width, Depth);
	Block->BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void Object_Create(Object* NewObject, ObjectBlock* Block, 
	uint32 ObjectID, float Width, float Height, float Depth)
{
	Block->BlockObjects[ObjectID] = NewObject;
	Block->BlockObjects[ObjectID]->Init(Width, Height, Depth);
	Block->BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void Object_SetTexture(ObjectBlock* Block, uint32 ObjectID,
	TextureStorage* Texture)
{
	Block->BlockObjects[ObjectID]->InputTexture(Texture);
}

Object* Object_GetObjectPtr(ObjectBlock* Block, uint32 ObjectID)
{
	return Block->BlockObjects[ObjectID];
}