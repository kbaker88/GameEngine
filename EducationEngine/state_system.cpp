#include "state_system.h"

void 
State_CreateCameras(ProgramState* State, 
	uint32 NumberOfCameras)
{
#if MEMORY_ON
	State->CameraArray = (Camera*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(Camera) * NumberOfCameras);
#else
	State->CameraArray = new Camera[NumberOfCameras];
#endif
}

void
State_CreateTimers(ProgramState* State,
	uint32 NumberOfTimers)
{
#if MEMORY_ON
	State->TimerArray = (Timer*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(Timer) * NumberOfTimers);
#else
	State->TimerArray = new Timer[NumberOfTimers];
#endif
}

void 
State_CreateShaderVariables(ProgramState* State, 
	uint32 NumberOfVars)
{
#if MEMORY_ON
	State->GPUShaderVarArray = 0;
	State->GPUShaderVarArray = 
		Memory_Allocate(State->GPUShaderVarArray, 
		NumberOfVars);
#else
	State->GPUShaderVarArray = new int32[NumberOfVars];
#endif
}

void
State_CreateShaderHandles(ProgramState* State,
	uint32 NumberOfHandles)
{
#if MEMORY_ON
	State->ShaderHandles = 0;
	State->ShaderHandles = 
		Memory_Allocate(State->ShaderHandles,
		NumberOfHandles);
#else
	State->ShaderHandles = new uint32[NumberOfHandles];
#endif
}

void
State_CreateRenderObjectBlocks(ProgramState* State, 
	uint32 NumberOfBlocks, uint32 BlockSize)
{
#if MEMORY_ON
	State->RenderObjBlocks = 0;
	State->RenderObjBlocks =
		(RenderObjBlock*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(RenderObjBlock) * NumberOfBlocks);

#else
	State->RenderObjBlocks =
		new RenderObjBlock[NumberOfBlocks]{};
#endif // MEMORY_ON
	for (uint32 Index = 0;
		Index < NumberOfBlocks;
		Index++)
	{
		RenderObj_CreateBlock(&State->RenderObjBlocks[Index],
			BlockSize);
	}
}

void
State_CreateModelObjectBlocks(ProgramState* State,
	uint32 NumberOfBlocks, uint32 BlockSize)
{
#if MEMORY_ON
	State->ModelObjBlocks = 0;
	State->ModelObjBlocks =
		(ModelObjBlock*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(ModelObjBlock) * NumberOfBlocks);

#else
	State->ModelObjBlocks =
		new ModelObjBlock[NumberOfBlocks]{};
#endif // MEMORY_ON
	for (uint32 Index = 0;
		Index < NumberOfBlocks;
		Index++)
	{
		ModelObj_CreateBlock(&State->ModelObjBlocks[Index],
			BlockSize);
	}
}

void
State_CreateCollisionObjects(ProgramState* State,
	uint32 NumOfObjs)
{
#if MEMORY_ON
	State->CollisionObj = 0;
	State->CollisionObj =
		(CollisionObject*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(CollisionObject) * NumOfObjs);
#else
	State->CollisionObj = new CollisionObject[NumOfObjs];
#endif
}

void
State_CreatePhysicsObjects(ProgramState* State,
	uint32 NumOfObjs)
{
#if MEMORY_ON
	State->PhysicsObj = 0;
	State->PhysicsObj =
		(PhysicsObject*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(PhysicsObject) * NumOfObjs);
#else
	State->PhysicsObj = new PhysicsObject[NumOfObjs];
#endif
}

void
State_CreateTextObjs(ProgramState* State,
	uint32 NumOfObjs)
{
#if MEMORY_ON
	State->TextObjArray = 0;
	State->TextObjArray =
		(Text_Object*)Memory_GetMemPtr();
	Memory_AdvanceItr(sizeof(Text_Object) * NumOfObjs);
#else
	State->TextObjArray = new Text_Object[NumOfObjs];
#endif
}

void 
State_LinkToProgram(ProgramState* State, 
	uint8* StateOfProgramPtr)
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
		for (uint32 i = 0; 
			i < TEXT_OBJECTS_PER_PROGSTATE;
			i++)
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
	if (State->CollisionObj)
	{
		delete[] State->CollisionObj;
		State->CollisionObj = 0;
	}
	if (State->PhysicsObj)
	{
		delete[] State->PhysicsObj;
		State->PhysicsObj = 0;
	}
	if (State->RenderObjBlocks)
	{
		for (uint32 Index = 0;
			Index < State->NumRenderObjBlocks;
			Index++)
		{
			RenderObj_DeleteBlock(&State->RenderObjBlocks[Index]);
		}
		delete[] State->RenderObjBlocks;
		State->RenderObjBlocks = 0;
	}
	if (State->ModelObjBlocks)
	{
		for (uint32 Index = 0;
			Index < State->NumModelObjBlocks;
			Index++)
		{
			ModelObj_DeleteBlock(&State->ModelObjBlocks[Index]);
		}
		delete[] State->ModelObjBlocks;
		State->ModelObjBlocks = 0;
	}
#endif
	State->StateOfProgram = 0;
	State->Status = 0;
}