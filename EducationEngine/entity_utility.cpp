#include "entity_utility.h"

void Utility_CreateButton(ProgramState* State, float Width, float Height,
	v3* Position, TextureStorage* Texture)
{
	Object_Create(new MyRectangle, &State->ObjectBlocks, State->ObjectCount,
		Width, Height, 0.0f);
	Object_SetTexture(&State->ObjectBlocks, State->ObjectCount, Texture);
	Entity_Create(&State->EntityBlocks, State->EntityCount,
		&State->ObjectBlocks, State->ObjectCount, Position);
	Entity_GetCollisionObjPtr(&State->EntityBlocks, State->EntityCount)->
		CollisionCode = State->EntityCount * 10;
	State->ObjectCount++;
	State->EntityCount++;
}