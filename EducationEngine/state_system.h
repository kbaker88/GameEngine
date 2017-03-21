#ifndef STATE_SYSTEM_H
#define STATE_SYSTEM_H

#include "player.h"
#include "clock.h" // TODO: move this later. 
#include "camera.h" // TODO: move this later. 

struct ProgramState
{
	ProgramState() : CameraArray(NULL), TimerArray(NULL),
		GPUShaderVarArray(NULL), ShaderHandles(NULL),
		Status(0), StateOfProgram(NULL) {}

	//TODO: Make Camera and Player a special type of entity
	Camera* CameraArray;
	Timer* TimerArray;
	int32* GPUShaderVarArray;
	uint32* ShaderHandles;
	int32 Status;
	uint32 EntityBlockNum;
	uint8* StateOfProgram;
};

void State_CreateCameras(ProgramState* State, uint32 NumberOfCameras);
void State_CreateTimers(ProgramState* State, uint32 NumberOfTimers);
void State_CreateShaderVariables(ProgramState* State, uint32 NumberOfVars);
void State_CreateShaderHandles(ProgramState* State, uint32 NumberOfHandles);
void State_LinkToProgram(ProgramState* State, uint8* StateOfProgramPtr);

void State_Clean(ProgramState* State);


#endif