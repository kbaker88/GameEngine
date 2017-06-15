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
			Block->BlockObjects = new RenderObject*[Size] {};
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
RenderObj_CreateRenderObject(RenderObject* RenderObj, Model* ModelObj)
{
#if MEMORY_ON
	RenderObject->BufferID = 0;
	RenderObject->BufferID = Memory_Allocate(RenderObject->BufferID,
		ModelObj->NumAttribs);
#else
	RenderObj->BufferID = new uint32[ModelObj->NumAttribs];
#endif
	RenderObj->NumVertices = ModelObj->NumVertices;
	//TODO: Think about creating many VAO's at once.
	Render_CreateVertexArrays(1, &RenderObj->VertexArrayID);
	Render_CreateBuffers(ModelObj->NumAttribs,
		RenderObj->BufferID);
	for (uint32 Index = 0; 
		Index < ModelObj->NumAttribs; 
		Index++)
	{
		Render_FillBuffer(RenderObj->BufferID[Index],
			ModelObj->ArraySize[Index], ModelObj->Data[Index], 0);
	}
	Render_FillVetexArrayObject(RenderObj, ModelObj->NumAttribs,
		ModelObj->ArrayOffset);
}

void
RenderObj_Delete(RenderObject* RenderObj)
{
#if MEMORY_ON
	// TODO: Implement memory management
#else
	if (RenderObj)
	{
		if (RenderObj->BufferID)
		{
			delete[] RenderObj->BufferID;
			RenderObj->BufferID = 0;
		}
		delete RenderObj;
		RenderObj = 0;
	}
	else
	{
		// TODO: Error
	}
#endif
}