#include "state_system.h"

void State_CreateCameras(ProgramState* State, uint32 NumberOfCameras)
{
	State->CameraArray = new Camera[NumberOfCameras];
}

void State_CreateTimers(ProgramState* State, uint32 NumberOfTimers)
{
	State->TimerArray = new Timer[NumberOfTimers];
}

void State_CreateShaderVariables(ProgramState* State, uint32 NumberOfVars)
{
	State->GPUShaderVarArray = new int32[NumberOfVars];
}

void State_CreateShaderHandles(ProgramState* State, uint32 NumberOfHandles)
{
	State->ShaderHandles = new uint32[NumberOfHandles];
}

void State_LinkToProgram(ProgramState* State, uint8* StateOfProgramPtr)
{
	State->StateOfProgram = StateOfProgramPtr;
}

void State_Clean(ProgramState* State)
{
	if (State->CameraArray)
	{
		delete[] State->CameraArray;
		State->CameraArray = NULL;
	}
	if (State->TimerArray)
	{
		delete[] State->TimerArray;
		State->TimerArray = NULL;
	}
	if (State->GPUShaderVarArray)
	{
		delete[] State->GPUShaderVarArray;
		State->GPUShaderVarArray = NULL;
	}
	if (State->ShaderHandles)
	{
		delete[] State->ShaderHandles;
		State->ShaderHandles = NULL;
	}
	State->StateOfProgram = NULL;
	State->Status = 0;
}