#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "utility.h"

uint8 Memory_Init();

uint32 Memory_Add(void* Data);
uint8 Memory_Delete(uint32 DataID);
void* Memory_Search(uint32 DataID);
//Entity Entities[256]; // remove from entity_system.h
//Camera Cameras[8]; // remove from world_system and user_interface
//Object
//ObjectInstance Temp;
//PhysicsObject
//CollisionObject
//Camera
//Player
//Assets


/*
Entities[256];
- uint32 ID
- ObjectInst
- PhysicsObject*
- CollisionObject*
- bool active

ObjectInstance;
- Object*
- m4 ModelMatrix
- v3 Position
- uint32 ID

Object;
- PipelineObjectDescription ObjectDescription
- float* VerticeFloatArrayPtr
- uint32 NUmVertices
- uint32 numindicies
- float width, height, depth
- uint32 TextureID
- uint32 ID

PhysicsObject;
- uint32 ID
- v3 *Position
- v3 Velocity
- v3 Acceleration
- v3 ForceSum
- v3 MoveDirection
- float AccelerationRate, Mass

CollisionObject;
- uint32 ID
- uint32 NumVertices
- float* VerticesPtr
- v3 *Position
- float Width, Height, Depth

Camera;
- v3 Position
- v3 UpDirection
- v3 FrontDirection
- m4 ViewMatrix
- m4 ProjectMatrix
- uint32 ID

Player;
- v3 FrontDirect
- v3 UpDirect
- v3 Direction
- v2 CursorOffset
- v2 PrevCursorPosition
- uint32 PlayerEntityID
- uint32 ObjectID
- float pitch, yaw

*/

#endif