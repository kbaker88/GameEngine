#include "object_system.h"

struct ObjectBlock
{
	ObjectBlock() : BlockObjects(NULL), BlockSize(0) {}
	Object** BlockObjects;
	uint32 BlockSize;
};

//TODO: Find a better spot for this.
ObjectBlock ObjectBlocks[3];

void Object_CreateBlock(uint32 BlockID, uint32 Size)
{
	ObjectBlocks[BlockID].BlockObjects = new Object*[Size] {};
	ObjectBlocks[BlockID].BlockSize = Size;
}

void Object_DeleteBlock(uint32 BlockID)
{
	if (ObjectBlocks[BlockID].BlockObjects)
	{
		for (uint32 i = 0; i < ObjectBlocks[BlockID].BlockSize; i++)
		{
			if (ObjectBlocks[BlockID].BlockObjects[i])
			{
				ObjectBlocks[BlockID].BlockObjects[i]->Delete();
				delete ObjectBlocks[BlockID].BlockObjects[i];
			}
		}
		delete[] ObjectBlocks[BlockID].BlockObjects;
		ObjectBlocks[BlockID].BlockObjects = NULL;
		ObjectBlocks[BlockID].BlockSize = 0;
	}
}

void Object_Create(Object* NewObject, uint32 BlockID, uint32 ObjectID)
{
	ObjectBlocks[BlockID].BlockObjects[ObjectID] = NewObject;
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->Init();
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void Object_Create(Object* NewObject, uint32 BlockID, uint32 ObjectID, TextureStorage* HeightMapTexture)
{
	ObjectBlocks[BlockID].BlockObjects[ObjectID] = NewObject;
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->Init(HeightMapTexture);
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void Object_Create(Object* NewObject, uint32 BlockID, uint32 ObjectID, uint32 Width, uint32 Depth)
{
	ObjectBlocks[BlockID].BlockObjects[ObjectID] = NewObject;
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->Init(Width, Depth);
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void Object_Create(Object* NewObject, uint32 BlockID, uint32 ObjectID, float Width, float Height, float Depth)
{
	ObjectBlocks[BlockID].BlockObjects[ObjectID] = NewObject;
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->Init(Width, Height, Depth);
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void Object_SetTexture(uint32 BlockID, uint32 ObjectID, TextureStorage* Texture)
{
	ObjectBlocks[BlockID].BlockObjects[ObjectID]->InputTexture(Texture);
}

Object* Object_GetObjectPtr(uint32 BlockID, uint32 ObjectID)
{
	return ObjectBlocks[BlockID].BlockObjects[ObjectID];
}