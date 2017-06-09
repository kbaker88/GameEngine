#include "render_object.h"

void
RenderObj_CreateBlock(RenderObjBlock* Block, uint32 Size)
{
	if (!Block)
	{
		// TODO: Error
	}
	else
	{
		if (!Block->BlockObjects)
		{
#if MEMORY_ON
			Block->BlockObjects = (RenderObject*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(RenderObject) * Size);
#else
			Block->BlockObjects = new RenderObj*[Size] {};
			Block->BlockSize = Size;
#endif
		}
		else
		{
			//TODO: Error checking
		}
	}
}

void
RenderObj_DeleteBlock(RenderObjBlock* Block)
{
	// TODO: Are there OpenGL cleanups that need to take place here?
	if (Block)
	{
#if MEMORY_ON
		// TODO: Make cleaning system
#else
		if (Block->BlockObjects)
		{
			for (uint32 i = 0;
				i < Block->BlockSize;
				i++)
			{
				if (Block->BlockObjects[i])
				{
					if (Block->BlockObjects[i]->BufferID)
					{
						delete[] Block->BlockObjects[i]->BufferID;
						Block->BlockObjects[i]->BufferID = 0;
					}
					delete Block->BlockObjects[i];
					Block->BlockObjects[i] = 0;
				}
			}
			delete[] Block->BlockObjects;
			Block->BlockObjects = 0;
			Block->BlockSize = 0;
		}
#endif
	}
}

void
RenderObj_CreateRenderObject(RenderObj* RenderObject, Model* ModelObj)
{
#if MEMORY_ON
	RenderObject->BufferID = 0;
	RenderObject->BufferID = Memory_Allocate(RenderObject->BufferID,
		ModelObj->NumAttribs);
#else
	RenderObject->BufferID = new uint32[ModelObj->NumAttribs];
#endif
	RenderObject->NumVertices = ModelObj->NumVertices;
	//TODO: Think about creating many VAO's at once.
	Render_CreateVertexArrays(1, &RenderObject->VertexArrayID);
	Render_CreateBuffers(ModelObj->NumAttribs,
		RenderObject->BufferID);
	for (uint32 Index = 0; 
		Index < ModelObj->NumAttribs; 
		Index++)
	{
		Render_FillBuffer(RenderObject->BufferID[Index],
			ModelObj->ArraySize[Index], ModelObj->Data[Index], 0);
	}
	Render_FillVetexArrayObject(RenderObject, ModelObj->NumAttribs,
		ModelObj->ArrayOffset);
}

void
RenderObj_Delete(RenderObj* Object)
{
#if MEMORY_ON
	// TODO: Implement memory management
#else
	if (Object)
	{
		if (Object->BufferID)
		{
			delete[] Object->BufferID;
			Object->BufferID = 0;
		}
		delete Object;
		Object = 0;
	}
	else
	{
		// TODO: Error
	}
#endif
}