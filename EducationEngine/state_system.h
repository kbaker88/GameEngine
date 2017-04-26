#ifndef STATE_SYSTEM_H
#define STATE_SYSTEM_H

#include "entity_system.h"

struct ProgramState
{
	ProgramState() : CameraArray(0), TimerArray(0),
		GPUShaderVarArray(0), ShaderHandles(0),
		Status(0), NumEntityBlocks(0), NumObjectBlocks(0),
		StateOfProgram(0), CursorPosition(0.0f, 0.0f),
		ObjectCount(0), EntityCount(0), Fonts(0), 
		FontCount(0) {}

	~ProgramState() {}

	Camera* CameraArray;
	Timer* TimerArray;
	EntityBlock EntityBlocks;
	RenderObjBlock ObjectBlocks;

	Text_Font *Fonts;
	uint32 FontCount;

	v2 CursorPosition;

	int32* GPUShaderVarArray;
	uint32* ShaderHandles;
	int32 Status;
	uint32 NumEntityBlocks, NumObjectBlocks, ObjectCount,
		EntityCount;
	uint8* StateOfProgram;
};

void State_CreateCameras(ProgramState* State, uint32 NumberOfCameras);
void State_CreateTimers(ProgramState* State, uint32 NumberOfTimers);
void State_CreateShaderVariables(ProgramState* State, uint32 NumberOfVars);
void State_CreateShaderHandles(ProgramState* State, uint32 NumberOfHandles);
void State_LinkToProgram(ProgramState* State, uint8* StateOfProgramPtr);

void State_Clean(ProgramState* State);


#endif