#include "entity_utility.h"

uint32 Utility_CreateButton(ProgramState* State, float Width, 
	float Height, v3* Position, Texture2D* Texture)
{
	Object_Create(new MyRectangle, &State->ObjectBlocks,
		State->ObjectCount, Width, Height, 0.0f);
	Object_SetTexture(&State->ObjectBlocks, State->ObjectCount,
		Texture);
	Entity_Create(&State->EntityBlocks, State->EntityCount,
		&State->ObjectBlocks, State->ObjectCount, Position);
	Entity_GetCollisionObjPtr(&State->EntityBlocks, State->EntityCount)->
		CollisionCode = State->EntityCount * 10;
	State->ObjectCount++;
	State->EntityCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreateBox(ProgramState* State, float Width, float Height,
	float Depth)
{
	Object_Create(new Box, &State->ObjectBlocks, State->ObjectCount,
		Width, Height, Depth);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreateBox(ProgramState* State, float Width, float Height,
	float Depth, Texture2D* Texture)
{
	Object_Create(new Box, &State->ObjectBlocks, State->ObjectCount,
		Width, Height, Depth);
	Object_SetTexture(&State->ObjectBlocks, State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreatePlane(ProgramState* State, uint32 Width,
	uint32 Depth, Texture2D* Texture)
{
	Object_Create(new Plane2D, &State->ObjectBlocks,
		State->ObjectCount, Width, Depth);
	Object_SetTexture(&State->ObjectBlocks, State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreateHeightMap(ProgramState* State, 
	Texture2D* HeightMapImg, Texture2D* Texture)
{
	Object_Create(new HeightMap, &State->ObjectBlocks, State->ObjectCount,
		HeightMapImg);
	Object_SetTexture(&State->ObjectBlocks, State->ObjectCount,
		Texture);
	State->ObjectCount++;

	return (State->ObjectCount - 1);
}

uint32 Utility_CreateEntity(ProgramState* State, v3* Position,
	unsigned int ObjectID)
{
	Entity_Create(&State->EntityBlocks, State->EntityCount,
		&State->ObjectBlocks, ObjectID, Position);
	State->EntityCount++;

	return (State->EntityCount - 1);
}