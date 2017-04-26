#include "entity_utility.h"

uint32 Utility_CreateButton(ProgramState* State, float Width, 
	float Height, v3* Position, Texture2D* Texture)
{
	RenderObj_Create(new MyRectangle, &State->ObjectBlocks,
		State->ObjectCount, Width, Height, 0.0f);
	RenderObj_SetTexture(&State->ObjectBlocks, State->ObjectCount,
		Texture);
	Entity_Create(&State->EntityBlocks, State->EntityCount,
		RenderObj_GetObjectPtr(&State->ObjectBlocks, State->ObjectCount),
		Position, 0x101);
	Entity_GetCollisionObjPtr(&State->EntityBlocks, State->EntityCount, 0)->
		CollisionCode = State->EntityCount * 10;
	State->ObjectCount++;
	State->EntityCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreateBox(ProgramState* State, float Width, float Height,
	float Depth)
{
	RenderObj_Create(new Box, &State->ObjectBlocks, State->ObjectCount,
		Width, Height, Depth);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreateBox(ProgramState* State, float Width, float Height,
	float Depth, Texture2D* Texture)
{
	RenderObj_Create(new Box, &State->ObjectBlocks, State->ObjectCount,
		Width, Height, Depth);
	RenderObj_SetTexture(&State->ObjectBlocks, State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreatePlane(ProgramState* State, uint32 Width,
	uint32 Depth, Texture2D* Texture)
{
	RenderObj_Create(new Plane2D, &State->ObjectBlocks,
		State->ObjectCount, Width, Depth);
	RenderObj_SetTexture(&State->ObjectBlocks, State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreateHeightMap(ProgramState* State, 
	Texture2D* HeightMapImg, Texture2D* Texture)
{
	RenderObj_Create(new HeightMap, &State->ObjectBlocks, State->ObjectCount,
		HeightMapImg);
	RenderObj_SetTexture(&State->ObjectBlocks, State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreateEntity(ProgramState* State, v3* Position,
	unsigned int ObjectID)
{
	Entity_Create(&State->EntityBlocks, State->EntityCount,
		RenderObj_GetObjectPtr(&State->ObjectBlocks, ObjectID), Position,
		0x111);
	State->EntityCount++;

	return (State->EntityCount - 1);
}