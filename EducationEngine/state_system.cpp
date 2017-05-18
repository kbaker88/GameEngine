#include "state_system.h"

void 
State_CreateCameras(ProgramState* State, uint32 NumberOfCameras)
{
#if MEMORY_ON
	State->CameraArray = (Camera*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(Camera) * NumberOfCameras);
#else
	State->CameraArray = new Camera[NumberOfCameras];
#endif
}

void
State_CreateTimers(ProgramState* State, uint32 NumberOfTimers)
{
#if MEMORY_ON
	State->TimerArray = (Timer*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(Timer) * NumberOfTimers);
#else
	State->TimerArray = new Timer[NumberOfTimers];
#endif
}

void 
State_CreateShaderVariables(ProgramState* State, uint32 NumberOfVars)
{
#if MEMORY_ON
	State->GPUShaderVarArray = 0;
	State->GPUShaderVarArray = Memory_Allocate(State->GPUShaderVarArray, 
		NumberOfVars);
#else
	State->GPUShaderVarArray = new int32[NumberOfVars];
#endif
}

void
State_CreateShaderHandles(ProgramState* State, uint32 NumberOfHandles)
{
#if MEMORY_ON
	State->ShaderHandles = 0;
	State->ShaderHandles = Memory_Allocate(State->ShaderHandles,
		NumberOfHandles);
#else
	State->ShaderHandles = new uint32[NumberOfHandles];
#endif
}

void 
State_LinkToProgram(ProgramState* State, uint8* StateOfProgramPtr)
{
	State->StateOfProgram = StateOfProgramPtr;
}

void
State_Clean(ProgramState* State)
{
#if MEMORY_ON
	// TODO: Make cleaning system
#else
	if (State->CameraArray)
	{
		delete[] State->CameraArray;
		State->CameraArray = 0;
	}
	if (State->TimerArray)
	{
		delete[] State->TimerArray;
		State->TimerArray = 0;
	}
	if (State->TextObjArray)
	{
		for (uint32 i = 0; i < TEXT_OBJECTS_PER_PROGSTATE; i++)
		{
			if (State->TextObjArray->Buffer)
			{
				delete[] State->TextObjArray->Buffer;
				State->TextObjArray->Buffer = 0;
			}
		}
		delete[] State->TextObjArray;
		State->TextObjArray = 0;
	}
#if DATA_ORIENTED

#else
	if (State->RenderObjBlocks)
	{
		delete[] State->RenderObjBlocks;
		State->RenderObjBlocks = 0;
	}
#endif
	if (State->EntityBlocks)
	{
		delete[] State->EntityBlocks;
		State->EntityBlocks = 0;
	}
	if (State->GPUShaderVarArray)
	{
		delete[] State->GPUShaderVarArray;
		State->GPUShaderVarArray = 0;
	}
	if (State->ShaderHandles)
	{
		delete[] State->ShaderHandles;
		State->ShaderHandles = 0;
	}
#endif
	State->StateOfProgram = 0;
	State->Status = 0;
}