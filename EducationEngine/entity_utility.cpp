#include "entity_utility.h"

uint32 
Utility_CreateButton(ProgramState* State, float Width, 
	float Height, v3* Position, Texture2D* Texture)
{
#if MEMORY_ON
	// TODO: Fix these to use memory system
#else
	RenderObj_Create(new MyRectangle, &State->RenderObjBlocks[0],
		State->ObjectCount, Width, Height, 0.0f);
#endif
	RenderObj_SetTexture(&State->RenderObjBlocks[0], State->ObjectCount,
		Texture);
	Entity_Create(&State->EntityBlocks[0], State->EntityCount,
		RenderObj_GetObjectPtr(&State->RenderObjBlocks[0], State->ObjectCount),
		Position, 0x101);
	Entity_GetCollisionObjPtr(&State->EntityBlocks[0], State->EntityCount, 0)->
		CollisionCode = State->EntityCount * 10;
	State->ObjectCount++;
	State->EntityCount++;

	return (State->ObjectCount - 1);
}

uint32
Utility_CreateBox(ProgramState* State, float Width, float Height,
	float Depth)
{
#if MEMORY_ON
	// TODO: Fix these to use memory system
#else
	RenderObj_Create(new Box, &State->RenderObjBlocks[0], State->ObjectCount,
		Width, Height, Depth);
#endif
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 
Utility_CreateBox(ProgramState* State, float Width, float Height,
	float Depth, Texture2D* Texture)
{
#if MEMORY_ON
	// TODO: Fix these to use memory system
#else
	RenderObj_Create(new Box, &State->RenderObjBlocks[0], State->ObjectCount,
		Width, Height, Depth);
#endif
	RenderObj_SetTexture(&State->RenderObjBlocks[0], State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32
Utility_CreatePlane(ProgramState* State, uint32 Width,
	uint32 Depth, Texture2D* Texture)
{
#if MEMORY_ON
	// TODO: Fix these to use memory system
#else
	RenderObj_Create(new Plane2D, &State->RenderObjBlocks[0],
		State->ObjectCount, Width, Depth);
#endif
	RenderObj_SetTexture(&State->RenderObjBlocks[0], State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 
Utility_CreateHeightMap(ProgramState* State, 
	Texture2D* HeightMapImg, Texture2D* Texture)
{
#if MEMORY_ON
	// TODO: Fix these to use memory system
#else
	RenderObj_Create(new HeightMap, &State->RenderObjBlocks[0], State->ObjectCount,
		HeightMapImg);
#endif
	RenderObj_SetTexture(&State->RenderObjBlocks[0], State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32
Utility_CreateEntity(ProgramState* State, v3* Position,
	unsigned int ObjectID, uint64 TypesOfObjects)
{
	Entity_Create(&State->EntityBlocks[0], State->EntityCount,
		RenderObj_GetObjectPtr(&State->RenderObjBlocks[0], ObjectID), Position,
		TypesOfObjects);
	State->EntityCount++;

	return (State->EntityCount - 1);
}