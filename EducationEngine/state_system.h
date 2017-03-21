#ifndef STATE_SYSTEM_H
#define STATE_SYSTEM_H

#include "world_system.h"
//#include "game_state.h"

//TODO: Replace other states with this generalized state
struct ProgramState
{
	ProgramState() : CameraArray(NULL), GPUShaderVarArray(NULL),
		ShaderHandles(NULL), Status(0), StateOfProgram(NULL) {}

	//TODO: Maker Camera and Player a special type of entity
	Camera* CameraArray;
	int32* GPUShaderVarArray;
	uint32* ShaderHandles;
	int32 Status;
	uint32 EntityBlockNum;
	uint8* StateOfProgram;
};

void State_CreateCameras(ProgramState* State, uint32 NumberOfCameras);
void State_CreateShaderVariables(ProgramState* State, uint32 NumberOfVars);
void State_CreateShaderHandles(ProgramState* State, uint32 NumberOfHandles);
void State_LinkToProgram(ProgramState* State, uint8* StateOfProgramPtr);

void State_Clean(ProgramState* State);


#endif