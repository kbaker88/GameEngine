#ifndef ENTITY_UTILITY
#define ENTITY_UTILITY

#include "input_state.h"

uint32 Utility_CreateButton(ProgramState* State, float Width, 
	float Height, v3* Position, Texture2D* Texture);

uint32 Utility_CreateBox(ProgramState* State, float Width,
	float Depth, float Height);

uint32 Utility_CreateBox(ProgramState* State, float Width,
	float Height, float Depth, Texture2D* Texture);

uint32 Utility_CreatePlane(ProgramState* State, uint32 Width,
	uint32 Depth, Texture2D* Texture);

uint32 Utility_CreateHeightMap(ProgramState* State,
	Texture2D* HeightMapImg, Texture2D* Texture);

uint32 Utility_CreateEntity(ProgramState* State, v3* Position,
	unsigned int ObjectID);

#endif
