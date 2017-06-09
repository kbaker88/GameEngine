#ifndef MODEL_OBJECT_SYSTEM_H
#define MODEL_OBJECT_SYSTEM_H

#include "render_layer.h"
#include "asset_system.h"

struct Model // Byte Total: 44
{
	float** Data;						// Bytes 8
	uint32* IndiceData;					// Bytes 8
	// NOTE: ArraySize is the size in bytes.
	uint32* ArraySize;					// Bytes 8
	uint32* ArrayOffset;				// Bytes 8
	uint32 NumAttribs, IndiceCount,
		NumVertices;					// Bytes 12
};

struct ModelObjBlock // Byte Total: 12
{
	Model** BlockObjects; // Bytes 8
	uint32 BlockSize;	  // Bytes 4
};

void
ModelObj_CreateBlock(ModelObjBlock* Block, 
	uint32 Size);
void
ModelObj_DeleteBlock(ModelObjBlock* Block);

void
ModelObj_Create(Model* ModelObj,
	float* VerticeData, uint32 VerticeDataSize, 
	float* ColorData, uint32 ColorDataSize);
void
ModelObj_Delete(Model* ModelObj);

void
ModelObj_CreatePoint(Model* ModelObj, v3 Position,
	v3 Color);
void
ModelObj_CreateLine(Model* ModelObj, v3 PositionA, 
	v3 PositionB, v3 ColorP1, v3 ColorP2);
void
ModelObj_CreateRectangle(Model* ModelObj,
	float Width, float Height);
void
ModelObj_CreateBox(Model* ModelObj, float Width,
	float Height, float Depth);
void
ModelObj_CreatePlane(Model* ModelObj,
	uint32 Width, uint32 Depth);
void
ModelObj_CreateHeightmap(Model* ModelObj, 
	Texture2D* ImageData);

#endif