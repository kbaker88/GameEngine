#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include "state_system.h"

struct Cursor_State
{
	Cursor_State() : Position(0.0f, 0.0f), PrevPosition(0.0f, 0.0f),
		PositionOffset(0.0f, 0.0f) {}

	~Cursor_State() {}
	
	v2 Position, PrevPosition, PositionOffset;
};

void 
Input_UpdateMouseState(ProgramState* State, Camera* PlayerCamera);
void 
Input_UpdateKeyStates(ProgramState* State, Camera* PlayerCamera,
	v3* Direction);

#endif
