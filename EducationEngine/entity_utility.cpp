#include "entity_utility.h"

void Utility_CreateButton(ProgramState* State, float Width, float Height,
	v3* Position, TextureStorage* Texture)
{
	Object_Create(new MyRectangle, State->ObjectBlockNum, State->ObjectCount,
		Width, Height, 0.0f);
	Object_SetTexture(State->ObjectBlockNum, State->ObjectCount, Texture);
	Entity_Create(State->EntityBlockNum, State->EntityCount,
		State->ObjectBlockNum, State->ObjectCount, Position);
	Entity_GetCollisionObjPtr(State->EntityBlockNum, State->EntityCount)->
		CollisionCode = State->EntityCount * 10;
	State->ObjectCount++;
	State->EntityCount++;
}