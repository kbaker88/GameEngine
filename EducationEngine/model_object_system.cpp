#include "model_object_system.h"

void
ModelObj_CreateBlock(ModelObjBlock* Block, uint32 Size)
{
	if (!Block)
	{
		// TODO: Error
	}
	else
	{
		if (!Block->BlockObjects)
		{
#if MEMORY_ON
			Block->BlockObjects = (RenderObj*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(RenderObj) * Size);
#else
			Block->BlockObjects = new Model*[Size] {};
			Block->BlockSize = Size;
#endif // MEMORY_ON
		}
		else
		{
			//TODO: Error checking
		}
	}
}

void
ModelObj_DeleteBlock(ModelObjBlock* Block)
{
	if (Block)
	{
#if MEMORY_ON
		// TODO: Make cleaning system
#else
		if (Block->BlockObjects)
		{
			for (uint32 i = 0;
				i < Block->BlockSize;
				i++)
			{
				if (Block->BlockObjects[i])
				{
					if (Block->BlockObjects[i]->Data)
					{
						for (uint32 k = 0;
							k < Block->BlockObjects[i]->NumAttribs;
							k++)
						{
							delete[] Block->BlockObjects[i]->Data[k];
							Block->BlockObjects[i]->Data[k] = 0;
						}
						delete[] Block->BlockObjects[i]->Data;
						Block->BlockObjects[i]->Data = 0;
					}
					if (Block->BlockObjects[i]->ArraySize)
					{
						delete[] Block->BlockObjects[i]->ArraySize;
						Block->BlockObjects[i]->ArraySize = 0;
					}
					if (Block->BlockObjects[i]->ArrayOffset)
					{
						delete[] Block->BlockObjects[i]->ArrayOffset;
						Block->BlockObjects[i]->ArrayOffset = 0;
					}
				}
			}
			delete[] Block->BlockObjects;
			Block->BlockObjects = 0;
			Block->BlockSize = 0;
		}
#endif // MEMORY_ON
	}
}

void
ModelObj_Create(Model* ModelObj,
	float* VerticeData, uint32 VerticeDataSize,
	float* ColorData, uint32 ColorDataSize)
{
	ModelObj->NumAttribs = 2;
#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
#endif // MEMORY_ON
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = VerticeDataSize * sizeof(float);
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = ColorDataSize * sizeof(float);
}

void
ModelObj_Delete(Model* ModelObj)
{
#if MEMORY_ON
	// TODO: Implement memory management
#else
	if (ModelObj)
	{
		if (ModelObj->Data)
		{
			for (uint32 Index = 0;
				Index < ModelObj->NumAttribs;
				Index++)
			{
				delete[] ModelObj->Data[Index];
				ModelObj->Data[Index] = 0;
			}
			delete[] ModelObj->Data;
			ModelObj->Data = 0;
		}
		if (ModelObj->ArraySize)
		{
			delete[] ModelObj->ArraySize;
			ModelObj->ArraySize = 0;
		}
		if (ModelObj->ArrayOffset)
		{
			delete[] ModelObj->ArrayOffset;
			ModelObj->ArrayOffset = 0;
		}
		delete ModelObj;
		ModelObj = 0;
	}
	else
	{
		// TODO: Error System
	}
#endif // MEMORY_ON
}

void
ModelObj_CreatePoint(Model* ModelObj, v3 Position,
	v3 Color)
{
	ModelObj->NumAttribs = 2;
	ModelObj->NumVertices = 1;
#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
	ModelObj->ArrayOffset = 0;
	ModelObj->ArrayOffset = Memory_Allocate(ModelObj->ArrayOffset,
		ModelObj->NumAttribs);

	ModelObj->Data[0] = Memory_Allocate(ModelObj->Data[0], 3);
	ModelObj->Data[1] = Memory_Allocate(ModelObj->Data[1], 3);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
	ModelObj->ArrayOffset = new uint32[ModelObj->NumAttribs];

	ModelObj->Data[0] = new float[3]{};
	ModelObj->Data[1] = new float[3]{};
#endif // MEMORY_ON
	// NOTE: Vertice Data
	ModelObj->ArraySize[0] = 3 * sizeof(float);
	ModelObj->ArrayOffset[0] = 3;
	ModelObj->Data[0][0] = Position.x, ModelObj->Data[0][1] = Position.y,
		ModelObj->Data[0][2] = Position.z;

	// NOTE: Color Data
	ModelObj->ArraySize[1] = 3 * sizeof(float);
	ModelObj->ArrayOffset[1] = 3;
	ModelObj->Data[1][0] = Color.x, ModelObj->Data[1][1] = Color.y,
		ModelObj->Data[1][2] = Color.z;
}

void
ModelObj_CreateLine(Model* ModelObj, v3 PositionA,
	v3 PositionB, v3 ColorP1, v3 ColorP2)
{
	ModelObj->NumAttribs = 2;
	ModelObj->NumVertices = 2;
#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
	ModelObj->ArrayOffset = 0;
	ModelObj->ArrayOffset = Memory_Allocate(ModelObj->ArrayOffset,
		ModelObj->NumAttribs);

	ModelObj->Data[0] = Memory_Allocate(ModelObj->Data[0], 6);
	ModelObj->Data[1] = Memory_Allocate(ModelObj->Data[1], 6);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
	ModelObj->ArrayOffset = new uint32[ModelObj->NumAttribs];

	ModelObj->Data[0] = new float[6]{};
	ModelObj->Data[1] = new float[6]{};
#endif // MEMORY_ON
	// NOTE: Vertice Data
	ModelObj->ArraySize[0] = 6 * sizeof(float);
	ModelObj->ArrayOffset[0] = 3;
	ModelObj->Data[0][0] = PositionA.x, ModelObj->Data[0][1] = PositionA.y,
		ModelObj->Data[0][2] = PositionA.z;
	ModelObj->Data[0][3] = PositionB.x, ModelObj->Data[0][4] = PositionB.y,
		ModelObj->Data[0][5] = PositionB.z;

	// NOTE: Color Data
	ModelObj->ArraySize[1] = 6 * sizeof(float);
	ModelObj->ArrayOffset[1] = 3;
	ModelObj->Data[1][0] = ColorP1.x, ModelObj->Data[1][1] = ColorP1.y,
		ModelObj->Data[1][2] = ColorP1.z;
	ModelObj->Data[1][3] = ColorP2.x, ModelObj->Data[1][4] = ColorP2.y,
		ModelObj->Data[1][5] = ColorP2.z;
}

void
ModelObj_CreateRectangle(Model* ModelObj,
	float Width, float Height)
{
	float HalfWidth = Width * 0.5f;
	float HalfHeight = Height * 0.5f;
	ModelObj->NumAttribs = 4;
	ModelObj->NumVertices = 6;

#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
	ModelObj->ArrayOffset = 0;
	ModelObj->ArrayOffset = Memory_Allocate(ModelObj->ArrayOffset,
		ModelObj->NumAttribs);

	ModelObj->Data[0] = Memory_Allocate(ModelObj->Data[0], 18);
	ModelObj->Data[1] = Memory_Allocate(ModelObj->Data[1], 18);
	ModelObj->Data[2] = Memory_Allocate(ModelObj->Data[2], 8);
	ModelObj->Data[3] = Memory_Allocate(ModelObj->Data[3], 18);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs]{};
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs]{};
	ModelObj->ArrayOffset = new uint32[ModelObj->NumAttribs]{};

	ModelObj->Data[0] = new float[18]{};
	ModelObj->Data[1] = new float[18]{};
	ModelObj->Data[2] = new float[12]{};
	ModelObj->Data[3] = new float[18]{};
#endif // MEMORY_ON
	// NOTE: Vertice Data
	ModelObj->ArraySize[0] = 18 * sizeof(float);
	ModelObj->ArrayOffset[0] = 3;
	ModelObj->Data[0][0] = -HalfWidth; ModelObj->Data[0][1] = -HalfHeight; ModelObj->Data[0][2] = 0.0f;
	ModelObj->Data[0][3] = HalfWidth; ModelObj->Data[0][4] = -HalfHeight; ModelObj->Data[0][5] = 0.0f;
	ModelObj->Data[0][6] = HalfWidth; ModelObj->Data[0][7] = HalfHeight; ModelObj->Data[0][8] = 0.0f;
	ModelObj->Data[0][9] = HalfWidth; ModelObj->Data[0][10] = HalfHeight;	ModelObj->Data[0][11] = 0.0f;
	ModelObj->Data[0][12] = -HalfWidth; ModelObj->Data[0][13] = HalfHeight; ModelObj->Data[0][14] = 0.0f;
	ModelObj->Data[0][15] = -HalfWidth; ModelObj->Data[0][16] = -HalfHeight; ModelObj->Data[0][17] = 0.0f;

	// NOTE: Color Data
	ModelObj->ArraySize[1] = 18 * sizeof(float);
	ModelObj->ArrayOffset[1] = 3;
	ModelObj->Data[1][0] = 1.0f; ModelObj->Data[1][1] = 0.0f;  ModelObj->Data[1][2] = 0.0f;
	ModelObj->Data[1][3] = 0.0f; ModelObj->Data[1][4] = 1.0f;  ModelObj->Data[1][5] = 0.0f;
	ModelObj->Data[1][6] = 1.0f; ModelObj->Data[1][7] = 0.0f;  ModelObj->Data[1][8] = 0.0f;
	ModelObj->Data[1][9] = 1.0f; ModelObj->Data[1][10] = 0.0f; ModelObj->Data[1][11] = 0.0f;
	ModelObj->Data[1][12] = 0.0f; ModelObj->Data[1][13] = 0.0f; ModelObj->Data[1][14] = 1.0f;
	ModelObj->Data[1][15] = 1.0f; ModelObj->Data[1][16] = 0.0f; ModelObj->Data[1][17] = 0.0f;

	// NOTE: Texture Coordinates
	ModelObj->ArraySize[2] = 12 * sizeof(float);
	ModelObj->ArrayOffset[2] = 2;
	ModelObj->Data[2][0] = 0.0f; ModelObj->Data[2][1] = 0.0f;
	ModelObj->Data[2][2] = 1.0f; ModelObj->Data[2][3] = 0.0f;
	ModelObj->Data[2][4] = 1.0f; ModelObj->Data[2][5] = 1.0f;
	ModelObj->Data[2][6] = 1.0f; ModelObj->Data[2][7] = 1.0f;
	ModelObj->Data[2][8] = 0.0f; ModelObj->Data[2][9] = 1.0f;
	ModelObj->Data[2][10] = 0.0f; ModelObj->Data[2][11] = 0.0f;

	// NOTE: Normal Data
	ModelObj->ArraySize[3] = 18 * sizeof(float);
	ModelObj->ArrayOffset[3] = 3;
	ModelObj->Data[3][0] = 0.0f; ModelObj->Data[3][1] = 0.0f; ModelObj->Data[3][2] = 1.0f;
	ModelObj->Data[3][3] = 0.0f; ModelObj->Data[3][4] = 0.0f; ModelObj->Data[3][5] = 1.0f;
	ModelObj->Data[3][6] = 0.0f; ModelObj->Data[3][7] = 0.0f; ModelObj->Data[3][8] = 1.0f;
	ModelObj->Data[3][9] = 0.0f; ModelObj->Data[3][10] = 0.0f; ModelObj->Data[3][11] = 1.0f;
	ModelObj->Data[3][12] = 0.0f; ModelObj->Data[3][13] = 0.0f; ModelObj->Data[3][14] = 1.0f;
	ModelObj->Data[3][15] = 0.0f; ModelObj->Data[3][16] = 0.0f; ModelObj->Data[3][17] = 1.0f;
}

void
ModelObj_CreateBox(Model* ModelObj, float Width,
	float Height, float Depth)
{
	float HalfWidth = Width * 0.5f;
	float HalfHeight = Height * 0.5f;
	float HalfDepth = Depth * 0.5f;
	ModelObj->NumAttribs = 4;
	ModelObj->NumVertices = 36;

#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
	ModelObj->ArrayOffset = 0;
	ModelObj->ArrayOffset = Memory_Allocate(ModelObj->ArrayOffset,
		ModelObj->NumAttribs);

	ModelObj->Data[0] = Memory_Allocate(ModelObj->Data[0], 3 * 6 * 6);
	ModelObj->Data[1] = Memory_Allocate(ModelObj->Data[1], 3 * 6 * 6);
	ModelObj->Data[2] = Memory_Allocate(ModelObj->Data[2], 2 * 6 * 6);
	ModelObj->Data[3] = Memory_Allocate(ModelObj->Data[3], 3 * 6 * 6);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
	ModelObj->ArrayOffset = new uint32[ModelObj->NumAttribs];

	ModelObj->Data[0] = new float[3 * 6 * 6];
	ModelObj->Data[1] = new float[3 * 6 * 6];
	ModelObj->Data[2] = new float[2 * 6 * 6];
	ModelObj->Data[3] = new float[3 * 6 * 6];
#endif // MEMORY_ON
	// NOTE: Vertice Data
	ModelObj->ArraySize[0] = 108 * sizeof(float);
	ModelObj->ArrayOffset[0] = 3;
	// NOTE: Back
	ModelObj->Data[0][0] = -HalfWidth; ModelObj->Data[0][1] = -HalfHeight; ModelObj->Data[0][2] = -HalfDepth;
	ModelObj->Data[0][3] = HalfWidth; ModelObj->Data[0][4] = -HalfHeight; ModelObj->Data[0][5] = -HalfDepth;
	ModelObj->Data[0][6] = HalfWidth; ModelObj->Data[0][7] = HalfHeight; ModelObj->Data[0][8] = -HalfDepth;
	ModelObj->Data[0][9] = HalfWidth; ModelObj->Data[0][10] = HalfHeight; ModelObj->Data[0][11] = -HalfDepth;
	ModelObj->Data[0][12] = -HalfWidth; ModelObj->Data[0][13] = HalfHeight; ModelObj->Data[0][14] = -HalfDepth;
	ModelObj->Data[0][15] = -HalfWidth; ModelObj->Data[0][16] = -HalfHeight; ModelObj->Data[0][17] = -HalfDepth;
	// NOTE: Front
	ModelObj->Data[0][18] = -HalfWidth; ModelObj->Data[0][19] = -HalfHeight; ModelObj->Data[0][20] = HalfDepth;
	ModelObj->Data[0][21] = HalfWidth; ModelObj->Data[0][22] = -HalfHeight; ModelObj->Data[0][23] = HalfDepth;
	ModelObj->Data[0][24] = HalfWidth; ModelObj->Data[0][25] = HalfHeight; ModelObj->Data[0][26] = HalfDepth;
	ModelObj->Data[0][27] = HalfWidth; ModelObj->Data[0][28] = HalfHeight; ModelObj->Data[0][29] = HalfDepth;
	ModelObj->Data[0][30] = -HalfWidth; ModelObj->Data[0][31] = HalfHeight; ModelObj->Data[0][32] = HalfDepth;
	ModelObj->Data[0][33] = -HalfWidth; ModelObj->Data[0][34] = -HalfHeight; ModelObj->Data[0][35] = HalfDepth;
	// NOTE: Left
	ModelObj->Data[0][36] = -HalfWidth; ModelObj->Data[0][37] = HalfHeight; ModelObj->Data[0][38] = HalfDepth;
	ModelObj->Data[0][39] = -HalfWidth; ModelObj->Data[0][40] = HalfHeight; ModelObj->Data[0][41] = -HalfDepth;
	ModelObj->Data[0][42] = -HalfWidth; ModelObj->Data[0][43] = -HalfHeight; ModelObj->Data[0][44] = -HalfDepth;
	ModelObj->Data[0][45] = -HalfWidth; ModelObj->Data[0][46] = -HalfHeight; ModelObj->Data[0][47] = -HalfDepth;
	ModelObj->Data[0][48] = -HalfWidth; ModelObj->Data[0][49] = -HalfHeight; ModelObj->Data[0][50] = HalfDepth;
	ModelObj->Data[0][51] = -HalfWidth; ModelObj->Data[0][52] = HalfHeight; ModelObj->Data[0][53] = HalfDepth;
	// NOTE: Right
	ModelObj->Data[0][54] = HalfWidth; ModelObj->Data[0][55] = HalfHeight; ModelObj->Data[0][56] = HalfDepth;
	ModelObj->Data[0][57] = HalfWidth; ModelObj->Data[0][58] = HalfHeight; ModelObj->Data[0][59] = -HalfDepth;
	ModelObj->Data[0][60] = HalfWidth; ModelObj->Data[0][61] = -HalfHeight; ModelObj->Data[0][62] = -HalfDepth;
	ModelObj->Data[0][63] = HalfWidth; ModelObj->Data[0][64] = -HalfHeight; ModelObj->Data[0][65] = -HalfDepth;
	ModelObj->Data[0][66] = HalfWidth; ModelObj->Data[0][67] = -HalfHeight; ModelObj->Data[0][68] = HalfDepth;
	ModelObj->Data[0][69] = HalfWidth; ModelObj->Data[0][70] = HalfHeight; ModelObj->Data[0][71] = HalfDepth;
	// NOTE: Bottom
	ModelObj->Data[0][72] = -HalfWidth; ModelObj->Data[0][73] = -HalfHeight; ModelObj->Data[0][74] = -HalfDepth;
	ModelObj->Data[0][75] = HalfWidth; ModelObj->Data[0][76] = -HalfHeight; ModelObj->Data[0][77] = -HalfDepth;
	ModelObj->Data[0][78] = HalfWidth; ModelObj->Data[0][79] = -HalfHeight; ModelObj->Data[0][80] = HalfDepth;
	ModelObj->Data[0][81] = HalfWidth; ModelObj->Data[0][82] = -HalfHeight; ModelObj->Data[0][83] = HalfDepth;
	ModelObj->Data[0][84] = -HalfWidth; ModelObj->Data[0][85] = -HalfHeight; ModelObj->Data[0][86] = HalfDepth;
	ModelObj->Data[0][87] = -HalfWidth; ModelObj->Data[0][88] = -HalfHeight; ModelObj->Data[0][89] = -HalfDepth;
	// NOTE: Top
	ModelObj->Data[0][90] = -HalfWidth; ModelObj->Data[0][91] = HalfHeight; ModelObj->Data[0][92] = -HalfDepth;
	ModelObj->Data[0][93] = HalfWidth; ModelObj->Data[0][94] = HalfHeight; ModelObj->Data[0][95] = -HalfDepth;
	ModelObj->Data[0][96] = HalfWidth; ModelObj->Data[0][97] = HalfHeight; ModelObj->Data[0][98] = HalfDepth;
	ModelObj->Data[0][99] = HalfWidth; ModelObj->Data[0][100] = HalfHeight; ModelObj->Data[0][101] = HalfDepth;
	ModelObj->Data[0][102] = -HalfWidth; ModelObj->Data[0][103] = HalfHeight; ModelObj->Data[0][104] = HalfDepth;
	ModelObj->Data[0][105] = -HalfWidth; ModelObj->Data[0][106] = HalfHeight; ModelObj->Data[0][107] = -HalfDepth;

	// NOTE: Color Data
	ModelObj->ArraySize[1] = 108 * sizeof(float);
	ModelObj->ArrayOffset[1] = 3;
	for (unsigned int Index = 0;
		Index < 108; 
		Index++)
	{
		ModelObj->Data[1][Index] = 1.0f;
	}

	// NOTE: Texture Coordinates
	ModelObj->ArraySize[2] = 72 * sizeof(float);
	ModelObj->ArrayOffset[2] = 2;
	ModelObj->Data[2][0] = 0.0f; ModelObj->Data[2][1] = 0.0f;
	ModelObj->Data[2][2] = 1.0f; ModelObj->Data[2][3] = 0.0f;
	ModelObj->Data[2][4] = 1.0f; ModelObj->Data[2][5] = 1.0f;
	ModelObj->Data[2][6] = 1.0f; ModelObj->Data[2][7] = 1.0f;
	ModelObj->Data[2][8] = 0.0f; ModelObj->Data[2][9] = 1.0f;
	ModelObj->Data[2][10] = 0.0f; ModelObj->Data[2][11] = 0.0f;

	ModelObj->Data[2][12] = 0.0f; ModelObj->Data[2][13] = 0.0f;
	ModelObj->Data[2][14] = 1.0f; ModelObj->Data[2][15] = 0.0f;
	ModelObj->Data[2][16] = 1.0f; ModelObj->Data[2][17] = 1.0f;
	ModelObj->Data[2][18] = 1.0f; ModelObj->Data[2][19] = 1.0f;
	ModelObj->Data[2][20] = 0.0f; ModelObj->Data[2][21] = 1.0f;
	ModelObj->Data[2][22] = 0.0f; ModelObj->Data[2][23] = 0.0f;

	ModelObj->Data[2][24] = 1.0f; ModelObj->Data[2][25] = 0.0f;
	ModelObj->Data[2][26] = 1.0f; ModelObj->Data[2][27] = 1.0f;
	ModelObj->Data[2][28] = 0.0f; ModelObj->Data[2][29] = 1.0f;
	ModelObj->Data[2][30] = 0.0f; ModelObj->Data[2][31] = 1.0f;
	ModelObj->Data[2][32] = 0.0f; ModelObj->Data[2][33] = 0.0f;
	ModelObj->Data[2][34] = 1.0f; ModelObj->Data[2][35] = 0.0f;

	ModelObj->Data[2][36] = 1.0f; ModelObj->Data[2][37] = 0.0f;
	ModelObj->Data[2][38] = 1.0f; ModelObj->Data[2][39] = 1.0f;
	ModelObj->Data[2][40] = 0.0f; ModelObj->Data[2][41] = 1.0f;
	ModelObj->Data[2][42] = 0.0f; ModelObj->Data[2][43] = 1.0f;
	ModelObj->Data[2][44] = 0.0f; ModelObj->Data[2][45] = 0.0f;
	ModelObj->Data[2][46] = 1.0f; ModelObj->Data[2][47] = 0.0f;

	ModelObj->Data[2][48] = 0.0f; ModelObj->Data[2][49] = 1.0f;
	ModelObj->Data[2][50] = 1.0f; ModelObj->Data[2][51] = 1.0f;
	ModelObj->Data[2][52] = 1.0f; ModelObj->Data[2][53] = 0.0f;
	ModelObj->Data[2][54] = 1.0f; ModelObj->Data[2][55] = 0.0f;
	ModelObj->Data[2][56] = 0.0f; ModelObj->Data[2][57] = 0.0f;
	ModelObj->Data[2][58] = 0.0f; ModelObj->Data[2][59] = 1.0f;

	ModelObj->Data[2][60] = 0.0f; ModelObj->Data[2][61] = 1.0f;
	ModelObj->Data[2][62] = 1.0f; ModelObj->Data[2][63] = 1.0f;
	ModelObj->Data[2][64] = 1.0f; ModelObj->Data[2][65] = 0.0f;
	ModelObj->Data[2][66] = 1.0f; ModelObj->Data[2][67] = 0.0f;
	ModelObj->Data[2][68] = 0.0f; ModelObj->Data[2][69] = 0.0f;
	ModelObj->Data[2][70] = 0.0f; ModelObj->Data[2][71] = 1.0f;

	// NOTE: Normal Data
	ModelObj->ArraySize[3] = 108 * sizeof(float);
	ModelObj->ArrayOffset[3] = 3;
	// NOTE: Back
	ModelObj->Data[3][0] = 0.0f; ModelObj->Data[3][1] = 0.0f; ModelObj->Data[3][2] = -1.0f;
	ModelObj->Data[3][3] = 0.0f; ModelObj->Data[3][4] = 0.0f; ModelObj->Data[3][5] = -1.0f;
	ModelObj->Data[3][6] = 0.0f; ModelObj->Data[3][7] = 0.0f; ModelObj->Data[3][8] = -1.0f;
	ModelObj->Data[3][9] = 0.0f; ModelObj->Data[3][10] = 0.0f; ModelObj->Data[3][11] = -1.0f;
	ModelObj->Data[3][12] = 0.0f; ModelObj->Data[3][13] = 0.0f; ModelObj->Data[3][14] = -1.0f;
	ModelObj->Data[3][15] = 0.0f; ModelObj->Data[3][16] = 0.0f; ModelObj->Data[3][17] = -1.0f;
	// NOTE: Front
	ModelObj->Data[3][18] = 0.0f; ModelObj->Data[3][19] = 0.0f; ModelObj->Data[3][20] = 1.0f;
	ModelObj->Data[3][21] = 0.0f; ModelObj->Data[3][22] = 0.0f; ModelObj->Data[3][23] = 1.0f;
	ModelObj->Data[3][24] = 0.0f; ModelObj->Data[3][25] = 0.0f; ModelObj->Data[3][26] = 1.0f;
	ModelObj->Data[3][27] = 0.0f; ModelObj->Data[3][28] = 0.0f; ModelObj->Data[3][29] = 1.0f;
	ModelObj->Data[3][30] = 0.0f; ModelObj->Data[3][31] = 0.0f; ModelObj->Data[3][32] = 1.0f;
	ModelObj->Data[3][33] = 0.0f; ModelObj->Data[3][34] = 0.0f; ModelObj->Data[3][35] = 1.0f;
	// NOTE: Left
	ModelObj->Data[3][36] = -1.0f; ModelObj->Data[3][37] = 0.0f; ModelObj->Data[3][38] = 0.0f;
	ModelObj->Data[3][39] = -1.0f; ModelObj->Data[3][40] = 0.0f; ModelObj->Data[3][41] = 0.0f;
	ModelObj->Data[3][42] = -1.0f; ModelObj->Data[3][43] = 0.0f; ModelObj->Data[3][44] = 0.0f;
	ModelObj->Data[3][45] = -1.0f; ModelObj->Data[3][46] = 0.0f; ModelObj->Data[3][47] = 0.0f;
	ModelObj->Data[3][48] = -1.0f; ModelObj->Data[3][49] = 0.0f; ModelObj->Data[3][50] = 0.0f;
	ModelObj->Data[3][51] = -1.0f; ModelObj->Data[3][52] = 0.0f; ModelObj->Data[3][53] = 0.0f;
	// NOTE: Right
	ModelObj->Data[3][54] = 1.0f; ModelObj->Data[3][55] = 0.0f; ModelObj->Data[3][56] = 0.0f;
	ModelObj->Data[3][57] = 1.0f; ModelObj->Data[3][58] = 0.0f; ModelObj->Data[3][59] = 0.0f;
	ModelObj->Data[3][60] = 1.0f; ModelObj->Data[3][61] = 0.0f; ModelObj->Data[3][62] = 0.0f;
	ModelObj->Data[3][63] = 1.0f; ModelObj->Data[3][64] = 0.0f; ModelObj->Data[3][65] = 0.0f;
	ModelObj->Data[3][66] = 1.0f; ModelObj->Data[3][67] = 0.0f; ModelObj->Data[3][68] = 0.0f;
	ModelObj->Data[3][69] = 1.0f; ModelObj->Data[3][70] = 0.0f; ModelObj->Data[3][71] = 0.0f;
	// NOTE: Bottom
	ModelObj->Data[3][72] = 0.0f; ModelObj->Data[3][73] = -1.0f; ModelObj->Data[3][74] = 0.0f;
	ModelObj->Data[3][75] = 0.0f; ModelObj->Data[3][76] = -1.0f; ModelObj->Data[3][77] = 0.0f;
	ModelObj->Data[3][78] = 0.0f; ModelObj->Data[3][79] = -1.0f; ModelObj->Data[3][80] = 0.0f;
	ModelObj->Data[3][81] = 0.0f; ModelObj->Data[3][82] = -1.0f; ModelObj->Data[3][83] = 0.0f;
	ModelObj->Data[3][84] = 0.0f; ModelObj->Data[3][85] = -1.0f; ModelObj->Data[3][86] = 0.0f;
	ModelObj->Data[3][87] = 0.0f; ModelObj->Data[3][88] = -1.0f; ModelObj->Data[3][89] = 0.0f;
	// NOTE: Top
	ModelObj->Data[3][90] = 0.0f; ModelObj->Data[3][91] = 1.0f; ModelObj->Data[3][92] = 0.0f;
	ModelObj->Data[3][93] = 0.0f; ModelObj->Data[3][94] = 1.0f; ModelObj->Data[3][95] = 0.0f;
	ModelObj->Data[3][96] = 0.0f; ModelObj->Data[3][97] = 1.0f; ModelObj->Data[3][98] = 0.0f;
	ModelObj->Data[3][99] = 0.0f; ModelObj->Data[3][100] = 1.0f; ModelObj->Data[3][101] = 0.0f;
	ModelObj->Data[3][102] = 0.0f; ModelObj->Data[3][103] = 1.0f; ModelObj->Data[3][104] = 0.0f;
	ModelObj->Data[3][105] = 0.0f; ModelObj->Data[3][106] = 1.0f; ModelObj->Data[3][107] = 0.0f;
}

void
ModelObj_CreatePlane(Model* ModelObj,
	uint32 Width, uint32 Depth)
{
	ModelObj->NumVertices = Width * Depth * 6;
	// TODO: Remove the verticedata, colordata, etc memory allocs
	//		and directly use the Model Obj's ptrs to alloc to.
	uint32 index = 0;
	uint32 NumberOfSquares = Width * Depth;
	const uint32 floatsPerSquare = 18;
	uint32 Size = floatsPerSquare * NumberOfSquares;
	bool flip = false;

#if MEMORY_ON
	float* VerticeData = 0;
	VerticeData = Memory_Allocate(VerticeData, Size);
	float* ColorData = 0;
	ColorData = Memory_Allocate(ColorData, Size);
	float* TextureCoords = 0;
	TextureCoords = Memory_Allocate(TextureCoords,
		12 * NumberOfSquares);
	float* NormalData = 0;
	NormalData = Memory_Allocate(NormalData, Size);
#else
	float *VerticeData = new float[Size] {};
	float *ColorData = new float[Size] {};
	float *TextureCoords = new float[12 * NumberOfSquares]{};
	float *NormalData = new float[Size] {};
#endif // MEMORY_ON
	 
	for (uint32 i = 0; 
		i < Width;
		i++)
	{
		if (i % 2 == 0)
			flip = false;
		else
			flip = true;

		for (uint32 j = 0;
			j < Depth; 
			j++)
		{
			if (!flip)
			{
				VerticeData[index + 0] = (float)i;
				VerticeData[index + 1] = 0;
				VerticeData[index + 2] = -(float)j - 1.0f;

				VerticeData[index + 3] = (float)i + 1.0f;
				VerticeData[index + 4] = 0;
				VerticeData[index + 5] = -(float)j - 1.0f;

				VerticeData[index + 6] = (float)i + 1.0f;
				VerticeData[index + 7] = 0;
				VerticeData[index + 8] = -(float)j;

				VerticeData[index + 9] = (float)i + 1.0f;
				VerticeData[index + 10] = 0;
				VerticeData[index + 11] = -(float)j;

				VerticeData[index + 12] = (float)i;
				VerticeData[index + 13] = 0;
				VerticeData[index + 14] = -(float)j;

				VerticeData[index + 15] = (float)i;
				VerticeData[index + 16] = 0;
				VerticeData[index + 17] = -(float)j - 1.0f;

				index = index + 18;

				flip = true;
			}
			else
			{
				//new
				VerticeData[index + 0] = (float)i; // f left
				VerticeData[index + 1] = 0;
				VerticeData[index + 2] = -(float)j;

				VerticeData[index + 3] = (float)i; // b left
				VerticeData[index + 4] = 0;
				VerticeData[index + 5] = -(float)j - 1.0f;

				VerticeData[index + 6] = (float)i + 1.0f; // b right
				VerticeData[index + 7] = 0;
				VerticeData[index + 8] = -(float)j - 1.0f;

				VerticeData[index + 9] = (float)i + 1.0f; // b right
				VerticeData[index + 10] = 0;
				VerticeData[index + 11] = -(float)j - 1.0f;

				VerticeData[index + 12] = (float)i + 1.0f; // f right
				VerticeData[index + 13] = 0;
				VerticeData[index + 14] = -(float)j;

				VerticeData[index + 15] = (float)i; // f left
				VerticeData[index + 16] = 0;
				VerticeData[index + 17] = -(float)j;


				index = index + 18;
				flip = false;
			}
		}
	}

	index = 0;

	for (uint32 i = 0;
		i < Width; 
		i++)
	{
		for (uint32 j = 0;
			j < Depth; 
			j++)
		{
			ColorData[index + 0] = 1;
			ColorData[index + 1] = 1;
			ColorData[index + 2] = 1;

			ColorData[index + 3] = 1;
			ColorData[index + 4] = 1;
			ColorData[index + 5] = 1;

			ColorData[index + 6] = 1;
			ColorData[index + 7] = 1;
			ColorData[index + 8] = 1;

			ColorData[index + 9] = 1;
			ColorData[index + 10] = 1;
			ColorData[index + 11] = 1;

			ColorData[index + 12] = 1;
			ColorData[index + 13] = 1;
			ColorData[index + 14] = 1;

			ColorData[index + 15] = 1;
			ColorData[index + 16] = 1;
			ColorData[index + 17] = 1;

			index = index + 18;
		}
	}

	index = 0;
	flip = false;

	for (uint32 i = 0;
		i < Width; 
		i++)
	{
		if (i % 2 == 0)
			flip = false;
		else
			flip = true;

		for (uint32 j = 0;
			j < Depth;
			j++)
		{
			if (!flip)
			{
				TextureCoords[index + 0] = 0.0f;
				TextureCoords[index + 1] = 1.0f;

				TextureCoords[index + 2] = 1.0f;
				TextureCoords[index + 3] = 1.0f;

				TextureCoords[index + 4] = 1.0f;
				TextureCoords[index + 5] = 0.0f;

				TextureCoords[index + 6] = 1.0f;
				TextureCoords[index + 7] = 0.0f;

				TextureCoords[index + 8] = 0.0f;
				TextureCoords[index + 9] = 0.0f;

				TextureCoords[index + 10] = 0.0f;
				TextureCoords[index + 11] = 1.0f;

				index = index + 12;
				flip = true;
			}
			else
			{
				TextureCoords[index + 0] = 0.0f;
				TextureCoords[index + 1] = 0.0f;

				TextureCoords[index + 2] = 0.0f;
				TextureCoords[index + 3] = 1.0f;

				TextureCoords[index + 4] = 1.0f;
				TextureCoords[index + 5] = 1.0f;

				TextureCoords[index + 6] = 1.0f;
				TextureCoords[index + 7] = 1.0f;

				TextureCoords[index + 8] = 1.0f;
				TextureCoords[index + 9] = 0.0f;

				TextureCoords[index + 10] = 0.0f;
				TextureCoords[index + 11] = 0.0f;

				index = index + 12;
				flip = false;
			}
		}
	}

	index = 0;

	for (uint32 i = 0; 
		i < Width;
		i++)
	{
		for (uint32 j = 0;
			j < Depth;
			j++)
		{
			NormalData[index + 0] = 0;
			NormalData[index + 1] = 1;
			NormalData[index + 2] = 0;

			NormalData[index + 3] = 0;
			NormalData[index + 4] = 1;
			NormalData[index + 5] = 0;

			NormalData[index + 6] = 0;
			NormalData[index + 7] = 1;
			NormalData[index + 8] = 0;

			NormalData[index + 9] = 0;
			NormalData[index + 10] = 1;
			NormalData[index + 11] = 0;

			NormalData[index + 12] = 0;
			NormalData[index + 13] = 1;
			NormalData[index + 14] = 0;

			NormalData[index + 15] = 0;
			NormalData[index + 16] = 1;
			NormalData[index + 17] = 0;

			index = index + 18;
		}
	}

	ModelObj->NumAttribs = 4;
#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data,
		ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
	ModelObj->ArrayOffset = 0;
	ModelObj->ArrayOffset = Memory_Allocate(ModelObj->ArrayOffset,
		ModelObj->NumAttribs);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
	ModelObj->ArrayOffset = new uint32[ModelObj->NumAttribs];
#endif // MEMORY_ON
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = Size * sizeof(float);
	ModelObj->ArrayOffset[0] = 3;
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = Size * sizeof(float);
	ModelObj->ArrayOffset[1] = 3;
	ModelObj->Data[2] = TextureCoords;
	ModelObj->ArraySize[2] = 12 * NumberOfSquares * sizeof(float);
	ModelObj->ArrayOffset[2] = 2;
	ModelObj->Data[3] = NormalData;
	ModelObj->ArraySize[3] = Size * sizeof(float);
	ModelObj->ArrayOffset[3] = 3;
}

void
ModelObj_CreateHeightmap(Model* ModelObj, Texture2D* ImageData)
{
	ModelObj->NumAttribs = 4;
	ModelObj->NumVertices = ImageData->Width * ImageData->Height * 6;
	float Width = (float)ImageData->Width;
	float Depth = (float)ImageData->Height;

	float *VerticeData = new float[3 * ModelObj->NumVertices]{};
	float *ColorData = new float[3 * ModelObj->NumVertices]{};
	float *TextureCoords = new float[2 * ModelObj->NumVertices]{};
	float *NormalData = new float[3 * ModelObj->NumVertices]{};

	uint32 HeightValue;
	uint32 PixelIndex = 0;

	bool flip = false;
	uint32 index = 0;

	v3 LineA, LineB, Normal;

	for (uint32 i = 0;
		i < Width - 1;
		i++)
	{
		if (i % 2 == 0)
			flip = true;
		else
			flip = false;

		for (uint32 j = 0;
			j < Depth - 1;
			j++)
		{
			if (!flip)
			{
				HeightValue = ImageData->Data[(2 * i * ImageData->Height) + (2 * j)] << 8 |
					ImageData->Data[(2 * i * ImageData->Height) + (2 * j) + 1];
				float Height1 = (float)HeightValue / 1000.0f;
				HeightValue = ImageData->Data[(2 * (i + 1) * ImageData->Height) + (2 * j)] << 8 |
					ImageData->Data[(2 * (i + 1) * ImageData->Height) + (2 * j) + 1];
				float Height2 = (float)HeightValue / 1000.0f;
				HeightValue = ImageData->Data[(2 * i * ImageData->Height) + (2 * (j + 1))] << 8 |
					ImageData->Data[(2 * i * ImageData->Height) + (2 * (j + 1)) + 1];
				float Height3 = (float)HeightValue / 1000.0f;
				HeightValue = ImageData->Data[(2 * (i + 1) * ImageData->Height) + (2 * (j + 1))] << 8 |
					ImageData->Data[(2 * (i + 1) * ImageData->Height) + (2 * (j + 1)) + 1];
				float Height4 = (float)HeightValue / 1000.0f;

				float XAxis = (float)i;
				float ZAxis = -(float)j;
				VerticeData[index + 0] = XAxis;
				VerticeData[index + 1] = Height1;
				VerticeData[index + 2] = ZAxis;
				ColorData[index + 0] = 1.0f;
				ColorData[index + 1] = ((float)HeightValue / 255.0f);
				ColorData[index + 2] = 0.0f;

				VerticeData[index + 3] = XAxis;
				VerticeData[index + 4] = Height3;
				VerticeData[index + 5] = ZAxis - 1.0f;
				ColorData[index + 3] = 1.0f;
				ColorData[index + 4] = ((float)HeightValue / 255.0f);
				ColorData[index + 5] = 0.0f;

				VerticeData[index + 6] = XAxis + 1.0f;
				VerticeData[index + 7] = Height2;
				VerticeData[index + 8] = ZAxis;
				ColorData[index + 6] = 1.0f;
				ColorData[index + 7] = ((float)HeightValue / 255.0f);
				ColorData[index + 8] = 0.0f;

				LineA = v3(VerticeData[index + 6] - VerticeData[index + 0],
						   VerticeData[index + 7] - VerticeData[index + 1],
						   VerticeData[index + 8] - VerticeData[index + 2]);

				LineB = v3(VerticeData[index + 3] - VerticeData[index + 0],
						   VerticeData[index + 4] - VerticeData[index + 1],
						   VerticeData[index + 5] - VerticeData[index + 2]);
				
				Normal = Math_Normalize(Math_CrossProduct(LineA, LineB));
				NormalData[index + 0] = Normal.x;
				NormalData[index + 1] = Normal.y;
				NormalData[index + 2] = Normal.z;
				NormalData[index + 3] = Normal.x;
				NormalData[index + 4] = Normal.y;
				NormalData[index + 5] = Normal.z;
				NormalData[index + 6] = Normal.x;
				NormalData[index + 7] = Normal.y;
				NormalData[index + 8] = Normal.z;


				VerticeData[index + 9] = XAxis + 1.0f;
				VerticeData[index + 10] = Height4;
				VerticeData[index + 11] = ZAxis - 1.0f;
				ColorData[index + 9] = 1.0f;
				ColorData[index + 10] = ((float)HeightValue / 255.0f);
				ColorData[index + 11] = 0.0f;

				VerticeData[index + 12] = XAxis + 1;
				VerticeData[index + 13] = Height2;
				VerticeData[index + 14] = ZAxis;
				ColorData[index + 12] = 1.0f;
				ColorData[index + 13] = ((float)HeightValue / 255.0f);
				ColorData[index + 14] = 0.0f;

				VerticeData[index + 15] = XAxis;
				VerticeData[index + 16] = Height3;
				VerticeData[index + 17] = ZAxis - 1.0f;
				ColorData[index + 15] = 1.0f;
				ColorData[index + 16] = ((float)HeightValue / 255.0f);
				ColorData[index + 17] = 0.0f;

				LineA = v3(VerticeData[index + 15] - VerticeData[index + 9],
						   VerticeData[index + 16] - VerticeData[index + 10],
						   VerticeData[index + 17] - VerticeData[index + 11]);

				LineB = v3(VerticeData[index + 12] - VerticeData[index + 9],
						   VerticeData[index + 13] - VerticeData[index + 10],
						   VerticeData[index + 14] - VerticeData[index + 11]);

				Normal = Math_Normalize(Math_CrossProduct(LineA, LineB));
				NormalData[index + 9]  = Normal.x;
				NormalData[index + 10] = Normal.y;
				NormalData[index + 11] = Normal.z;
				NormalData[index + 12] = Normal.x;
				NormalData[index + 13] = Normal.y;
				NormalData[index + 14] = Normal.z;
				NormalData[index + 15] = Normal.x;
				NormalData[index + 16] = Normal.y;
				NormalData[index + 17] = Normal.z;

				index = index + 18;

				flip = true;
			}
			else
			{
				HeightValue = ImageData->Data[(2 * i * ImageData->Height) + (2 * j)] << 8 |
					ImageData->Data[(2 * i * ImageData->Height) + (2 * j) + 1];
				float Height1 = (float)HeightValue / 1000.0f;
				HeightValue = ImageData->Data[(2 * (i + 1) * ImageData->Height) + (2 * j)] << 8 |
					ImageData->Data[(2 * (i + 1) * ImageData->Height) + (2 * j) + 1];
				float Height2 = (float)HeightValue / 1000.0f;
				HeightValue = ImageData->Data[(2 * i * ImageData->Height) + (2 * (j + 1))] << 8 |
					ImageData->Data[(2 * i * ImageData->Height) + (2 * (j + 1)) + 1];
				float Height3 = (float)HeightValue / 1000.0f;
				HeightValue = ImageData->Data[(2 * (i + 1) * ImageData->Height) + (2 * (j + 1))] << 8 |
					ImageData->Data[(2 * (i + 1) * ImageData->Height) + (2 * (j + 1)) + 1];
				float Height4 = (float)HeightValue / 1000.0f;


				float XAxis = (float)i;
				float ZAxis = -(float)j;
				VerticeData[index + 0] = XAxis; // f left
				VerticeData[index + 1] = Height1;
				VerticeData[index + 2] = ZAxis;
				ColorData[index + 0] = 1.0f;
				ColorData[index + 1] = ((float)HeightValue / 255.0f);
				ColorData[index + 2] = 0.0f;

				VerticeData[index + 3] = XAxis; // b left
				VerticeData[index + 4] = Height3;
				VerticeData[index + 5] = ZAxis - 1.0f;
				ColorData[index + 3] = 1.0f;
				ColorData[index + 4] = ((float)HeightValue / 255.0f);
				ColorData[index + 5] = 0.0f;

				VerticeData[index + 6] = XAxis + 1.0f; // b right
				VerticeData[index + 7] = Height4;
				VerticeData[index + 8] = ZAxis - 1.0f;
				ColorData[index + 6] = 1.0f;
				ColorData[index + 7] = ((float)HeightValue / 255.0f);
				ColorData[index + 8] = 0.0f;

				LineA = v3(VerticeData[index + 6] - VerticeData[index + 0],
						   VerticeData[index + 7] - VerticeData[index + 1],
						   VerticeData[index + 8] - VerticeData[index + 2]);

				LineB = v3(VerticeData[index + 3] - VerticeData[index + 0],
						   VerticeData[index + 4] - VerticeData[index + 1],
						   VerticeData[index + 5] - VerticeData[index + 2]);

				Normal = Math_Normalize(Math_CrossProduct(LineA, LineB));
				NormalData[index + 0] = Normal.x;
				NormalData[index + 1] = Normal.y;
				NormalData[index + 2] = Normal.z;
				NormalData[index + 3] = Normal.x;
				NormalData[index + 4] = Normal.y;
				NormalData[index + 5] = Normal.z;
				NormalData[index + 6] = Normal.x;
				NormalData[index + 7] = Normal.y;
				NormalData[index + 8] = Normal.z;

				VerticeData[index + 9] = XAxis + 1.0f; // b right
				VerticeData[index + 10] = Height4;
				VerticeData[index + 11] = ZAxis - 1.0f;
				ColorData[index + 9] = 1.0f;
				ColorData[index + 10] = ((float)HeightValue / 255.0f);
				ColorData[index + 11] = 0.0f;

				VerticeData[index + 12] = XAxis + 1.0f; // f right
				VerticeData[index + 13] = Height2;
				VerticeData[index + 14] = ZAxis;
				ColorData[index + 12] = 1.0f;
				ColorData[index + 13] = ((float)HeightValue / 255.0f);
				ColorData[index + 14] = 0.0f;

				VerticeData[index + 15] = XAxis; // f left
				VerticeData[index + 16] = Height1;
				VerticeData[index + 17] = ZAxis;
				ColorData[index + 15] = 1.0f;
				ColorData[index + 16] = ((float)HeightValue / 255.0f);
				ColorData[index + 17] = 0.0f;

				LineA = v3(VerticeData[index + 15] - VerticeData[index + 9],
					VerticeData[index + 16] - VerticeData[index + 10],
					VerticeData[index + 17] - VerticeData[index + 11]);

				LineB = v3(VerticeData[index + 12] - VerticeData[index + 9],
					VerticeData[index + 13] - VerticeData[index + 10],
					VerticeData[index + 14] - VerticeData[index + 11]);

				Normal = Math_Normalize(Math_CrossProduct(LineA, LineB));
				NormalData[index + 9] = Normal.x;
				NormalData[index + 10] = Normal.y;
				NormalData[index + 11] = Normal.z;
				NormalData[index + 12] = Normal.x;
				NormalData[index + 13] = Normal.y;
				NormalData[index + 14] = Normal.z;
				NormalData[index + 15] = Normal.x;
				NormalData[index + 16] = Normal.y;
				NormalData[index + 17] = Normal.z;

				index = index + 18;
				flip = false;
			}
		}
	}

	index = 0;
	for (uint32 i = 0;
		i < Width;
		i++)
	{
		if (i % 2 == 0)
			flip = false;
		else
			flip = true;

		for (uint32 j = 0;
			j < Depth;
			j++)
		{
			if (!flip)
			{
				TextureCoords[index + 0] = 0.0f;
				TextureCoords[index + 1] = 1.0f;

				TextureCoords[index + 2] = 1.0f;
				TextureCoords[index + 3] = 1.0f;

				TextureCoords[index + 4] = 1.0f;
				TextureCoords[index + 5] = 0.0f;

				TextureCoords[index + 6] = 1.0f;
				TextureCoords[index + 7] = 0.0f;

				TextureCoords[index + 8] = 0.0f;
				TextureCoords[index + 9] = 0.0f;

				TextureCoords[index + 10] = 0.0f;
				TextureCoords[index + 11] = 1.0f;

				index = index + 12;
				flip = true;
			}
			else
			{
				TextureCoords[index + 0] = 0.0f;
				TextureCoords[index + 1] = 0.0f;

				TextureCoords[index + 2] = 0.0f;
				TextureCoords[index + 3] = 1.0f;

				TextureCoords[index + 4] = 1.0f;
				TextureCoords[index + 5] = 1.0f;

				TextureCoords[index + 6] = 1.0f;
				TextureCoords[index + 7] = 1.0f;

				TextureCoords[index + 8] = 1.0f;
				TextureCoords[index + 9] = 0.0f;

				TextureCoords[index + 10] = 0.0f;
				TextureCoords[index + 11] = 0.0f;

				index = index + 12;
				flip = false;
			}
		}
	}

#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
	ModelObj->ArrayOffset = 0;
	ModelObj->ArrayOffset = Memory_Allocate(ModelObj->ArrayOffset,
		ModelObj->NumAttribs);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
	ModelObj->ArrayOffset = new uint32[ModelObj->NumAttribs];
#endif // MEMORY_ON
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = 3 * ModelObj->NumVertices * sizeof(float);
	ModelObj->ArrayOffset[0] = 3;
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = 3 * ModelObj->NumVertices * sizeof(float);
	ModelObj->ArrayOffset[1] = 3;
	ModelObj->Data[2] = TextureCoords;
	ModelObj->ArraySize[2] = 2 * ModelObj->NumVertices * sizeof(float);
	ModelObj->ArrayOffset[2] = 2;
	ModelObj->Data[3] = NormalData;
	ModelObj->ArraySize[3] = 3 * ModelObj->NumVertices * sizeof(float);
	ModelObj->ArrayOffset[3] = 3;
}