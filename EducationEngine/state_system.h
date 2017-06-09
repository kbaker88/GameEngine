#ifndef STATE_SYSTEM_H
#define STATE_SYSTEM_H

#include "entity_system.h"

// TODO: Try to reduce size to save space.
struct ProgramState
{
	Camera* CameraArray;
	Timer* TimerArray;
	RenderObjBlock* RenderObjBlocks;
	ModelObjBlock* ModelObjBlocks;
	CollisionObject* CollisionObj;
	PhysicsObject* PhysicsObj;
	// TODO: Temporary system
	Text_Object* TextObjArray;
	Font *FontArr;

	uint32 FontCount, NumRenderObjBlocks,
		NumModelObjBlocks;
	
	uint16 ConsoleGlyph[CONSOLE_BUFFER_LENGTH]{};
	int32 ConsoleItr;
	uint8 ConsoleState;
	uint64 LastKeyPress;

	v2 CursorPosition;

	int32* GPUShaderVarArray;
	uint32* ShaderHandles;
	int32 Status;
	uint8* StateOfProgram;
};

void
State_CreateCameras(ProgramState* State, 
	uint32 NumberOfCameras);
void 
State_CreateTimers(ProgramState* State, 
	uint32 NumberOfTimers);
void
State_CreateShaderVariables(ProgramState* State, 
	uint32 NumberOfVars);
void 
State_CreateShaderHandles(ProgramState* State,
	uint32 NumberOfHandles);
void
State_CreateRenderObjectBlocks(ProgramState* State,
	uint32 NumberOfBlocks, uint32 BlockSize);
void
State_CreateModelObjectBlocks(ProgramState* State,
	uint32 NumberOfBlocks, uint32 BlockSize);
void
State_CreateCollisionObjects(ProgramState* State,
	uint32 NumOfObjs);
void
State_CreatePhysicsObjects(ProgramState* State,
	uint32 NumOfObjs);
void
State_CreateTextObjs(ProgramState* State,
	uint32 NumOfObjs);

void
State_LinkToProgram(ProgramState* State, 
	uint8* StateOfProgramPtr);

void 
State_Clean(ProgramState* State);


#endif