#ifndef STATE_SYSTEM_H
#define STATE_SYSTEM_H

#include "game_state.h"

//TODO: Replace other states with this generalized state
struct ProgramState
{
	Camera* CameraArray;
	int32* GPUShaderVariableArray;
	uint8 ProgramState;
};

#endif