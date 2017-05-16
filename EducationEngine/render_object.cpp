#include "render_object.h"

#if DATA_ORIENTED

void 
RenderObj_CreateModelObject(Model* ModelObj,
	float* VerticeData, uint32 VerticeDataSize, float* ColorData,
	uint32 ColorDataSize)
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
#endif
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = VerticeDataSize * sizeof(float);
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = ColorDataSize * sizeof(float);
}

void
RenderObj_CreateRenderObject(RenderObj* RenderObject, Model* ModelObj)
{
#if MEMORY_ON
	RenderObject->BufferID = 0;
	RenderObject->BufferID = Memory_Allocate(RenderObject->BufferID,
		ModelObj->NumAttribs);
#else
	RenderObject->BufferID = new uint32[ModelObj->NumAttribs];
#endif

	//TODO: Think about creating many VAO's at once.
	Render_CreateVertexArrays(1, &RenderObject->VertexArrayID);
	Render_CreateBuffers(ModelObj->NumAttribs,
		RenderObject->BufferID);
	for (uint32 Index = 0; Index < ModelObj->NumAttribs; Index++)
	{
		Render_FillBuffer(RenderObject->BufferID[Index],
			ModelObj->ArraySize[Index], ModelObj->Data[Index], 0);
	}
	Render_FillVetexArrayObject(RenderObject, ModelObj->NumAttribs);
}

void
RenderObj_Delete(RenderObj* Object, Model* ModelObj)
{
#if MEMORY_ON
	// TODO: Implement memory management
#else
	if (Object->BufferID)
	{
		delete[] Object->BufferID;
		Object->BufferID = 0;
	}
	if (ModelObj->Data)
	{
		delete[] ModelObj->Data;
		ModelObj->Data = 0;
	}
	if (ModelObj->ArraySize)
	{
		delete[] ModelObj->ArraySize;
		ModelObj->ArraySize = 0;
	}
#endif
}

void
RenderObj_CreateModelPoint(Model* ModelObj, v3 Position,
	v3 Color)
{
	float VerticeData[] =
	{
		Position.x, Position.y, Position.z
	};

	float ColorData[] =
	{
		Color.x, Color.y, Color.z
	};

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
#endif
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = 3 * sizeof(float);
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = 3 * sizeof(float);
}

void
RenderObj_CreateModelLine(Model* ModelObj, v3 PositionA, v3 PositionB,
	v3 ColorP1, v3 ColorP2)
{
	float VerticeData[] =
	{
		PositionA.x, PositionA.y, PositionA.z,
		PositionB.x, PositionB.y, PositionB.z
	};

	float ColorData[] =
	{
		ColorP1.x, ColorP1.y, ColorP1.z,
		ColorP2.x, ColorP2.y, ColorP2.z
	};

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
#endif
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = 6 * sizeof(float);
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = 6 * sizeof(float);
}

void
RenderObj_CreateModelRectangle(Model* ModelObj, float Width, float Height)
{
	float HalfWidth = Width * 0.5f;
	float HalfHeight = Height * 0.5f;

	float VerticeData[] = {
		-HalfWidth , -HalfHeight ,  0.0f,  // bottom left 0
		 HalfWidth , -HalfHeight ,  0.0f,  // bottom right 1
		 HalfWidth ,  HalfHeight ,  0.0f,  // top right 2
		-HalfWidth ,  HalfHeight ,  0.0f,  // top left 3
	};

	uint32 Indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	float ColorData[]{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	float TextureCoords[]{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	float NormalData[]{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	ModelObj->NumAttribs = 4;
#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
#endif
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = 12 * sizeof(float);
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = 12 * sizeof(float);
	ModelObj->Data[2] = TextureCoords;
	ModelObj->ArraySize[2] = 8 * sizeof(float);
	ModelObj->Data[3] = NormalData;
	ModelObj->ArraySize[3] = 12 * sizeof(float);

	ModelObj->IndiceData = Indices;
	ModelObj->IndiceCount = 6;
}

void 
RenderObj_CreateModelBox(Model* ModelObj, float Width, float Height, 
	float Depth)
{
	float HalfWidth = Width * 0.5f;
	float HalfHeight = Height * 0.5f;
	float HalfDepth = Depth * 0.5f;

	float VerticeData[] =
	{	 // Back
		-HalfWidth, -HalfHeight, -HalfDepth, // left, bottom, back
		 HalfWidth, -HalfHeight, -HalfDepth, // right, bottom, back
		 HalfWidth,  HalfHeight, -HalfDepth, // right, top, back
		 HalfWidth,  HalfHeight, -HalfDepth, // right, top, back
		-HalfWidth,  HalfHeight, -HalfDepth, // left, top, back
		-HalfWidth, -HalfHeight, -HalfDepth, // left, bottom, back
													  // Front									  
		-HalfWidth, -HalfHeight,  HalfDepth, // Left, Bottom, Front
		 HalfWidth, -HalfHeight,  HalfDepth, // Right, Bottom, Front
		 HalfWidth,  HalfHeight,  HalfDepth, // Right, Top, Front
		 HalfWidth,  HalfHeight,  HalfDepth, // Right, Top, Front
		-HalfWidth,  HalfHeight,  HalfDepth, // Left, Top, Front
		-HalfWidth, -HalfHeight,  HalfDepth, // Left, Bottom, Front
		// Left
		-HalfWidth,  HalfHeight,  HalfDepth,
		-HalfWidth,  HalfHeight, -HalfDepth,
		-HalfWidth, -HalfHeight, -HalfDepth,
		-HalfWidth, -HalfHeight, -HalfDepth,
		-HalfWidth, -HalfHeight,  HalfDepth,
		-HalfWidth,  HalfHeight,  HalfDepth,
		// Right
		 HalfWidth,  HalfHeight,  HalfDepth,
		 HalfWidth,  HalfHeight, -HalfDepth,
		 HalfWidth, -HalfHeight, -HalfDepth,
		 HalfWidth, -HalfHeight, -HalfDepth,
		 HalfWidth, -HalfHeight,  HalfDepth,
		 HalfWidth,  HalfHeight,  HalfDepth,
		// Bottom
		-HalfWidth, -HalfHeight, -HalfDepth,
		 HalfWidth, -HalfHeight, -HalfDepth,
		 HalfWidth, -HalfHeight,  HalfDepth,
		 HalfWidth, -HalfHeight,  HalfDepth,
		-HalfWidth, -HalfHeight,  HalfDepth,
		-HalfWidth, -HalfHeight, -HalfDepth,
		// Top
		-HalfWidth,  HalfHeight, -HalfDepth,
		 HalfWidth,  HalfHeight, -HalfDepth,
		 HalfWidth,  HalfHeight,  HalfDepth,
		 HalfWidth,  HalfHeight,  HalfDepth,
		-HalfWidth,  HalfHeight,  HalfDepth,
		-HalfWidth,  HalfHeight, -HalfDepth,
	};

	float ColorData[]
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	float TextureCoords[]
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	float NormalData[]
	{
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	ModelObj->NumAttribs = 4;
#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
#endif
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = 108 * sizeof(float);
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = 108 * sizeof(float);
	ModelObj->Data[2] = TextureCoords;
	ModelObj->ArraySize[2] = 72 * sizeof(float);
	ModelObj->Data[3] = NormalData;
	ModelObj->ArraySize[3] = 108 * sizeof(float);
}

void
RenderObj_CreateModelPlane(Model* ModelObj, uint32 Width, uint32 Depth)
{
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
	float *VerticeData = new float[Size]{};
	float *ColorData = new float[Size]{};
	float *TextureCoords = new float[12 * NumberOfSquares]{};
	float *NormalData = new float[Size]{};
#endif;

	for (uint32 i = 0; i < Width; i++)
	{
		if (i % 2 == 0)
			flip = false;
		else
			flip = true;

		for (uint32 j = 0; j < Depth; j++)
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

	for (uint32 i = 0; i < Width; i++)
	{
		for (uint32 j = 0; j < Depth; j++)
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

	for (uint32 i = 0; i < Width; i++)
	{
		if (i % 2 == 0)
			flip = false;
		else
			flip = true;

		for (uint32 j = 0; j < Depth; j++)
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

	for (uint32 i = 0; i < Width; i++)
	{
		for (uint32 j = 0; j < Depth; j++)
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
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
#endif
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = Size * sizeof(float);
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = Size * sizeof(float);
	ModelObj->Data[2] = TextureCoords;
	ModelObj->ArraySize[2] = 12 * NumberOfSquares * sizeof(float);
	ModelObj->Data[3] = NormalData;
	ModelObj->ArraySize[3] = Size * sizeof(float);
}

void
RenderObj_CreateModelHeightmap(Model* ModelObj, Texture2D* ImageData)
{
	uint32 TotalVertices = ImageData->Width * ImageData->Height;
	uint32 NumberOfVertices = TotalVertices;
	float Width = (float)ImageData->Width;
	float Depth = (float)ImageData->Height;

	uint32 NumberOfIndices = 6 * TotalVertices;

	uint32 Pixel = 0;

#if MEMORY_ON
	float* VerticeData = 0;
	VerticeData = Memory_Allocate(VerticeData,
		3 * TotalVertices);
	float* ColorData = 0;
	ColorData = Memory_Allocate(ColorData, 3 * TotalVertices);
	float* TextureCoords = 0;
	TextureCoords = Memory_Allocate(TextureCoords, 8 * TotalVertices);
	float* NormalData = 0;
	NormalData = Memory_Allocate(NormalData, 3 * TotalVertices);
	uint32* Indices = 0;
	Indices = Memory_Allocate(Indices, NumberOfIndices);
#else
	float *VerticeData = new float[3 * TotalVertices]{};
	float *ColorData = new float[3 * TotalVertices]{};
	float *TextureCoords = new float[8 * TotalVertices]{};
	float *NormalData = new float[3 * TotalVertices]{};
	uint32 *Indices = new uint32[NumberOfIndices]{};
#endif
	uint32 VertexIndex = 0;
	uint32 ImagePixelIndex = 0;
	for (uint32 Row = 0; Row < ImageData->Width; Row++)
	{
		for (uint32 Column = 0; Column < ImageData->Height; Column++)
		{
			Pixel = ImageData->Data[ImagePixelIndex++] << 8 | ImageData->Data[ImagePixelIndex++];
			float Height = (float)Pixel / 1000.0f;
			VertexIndex = 3 * Row * ImageData->Height + 3 * Column;

			// NOTE: X,  0 to RowMax
			VerticeData[VertexIndex + 0] = (float)Row;
			ColorData[VertexIndex + 0] = 1.0f;
			// NOTE: Y, Height
			VerticeData[VertexIndex + 1] = Height;
			ColorData[VertexIndex + 1] = (float)((float)Pixel / 255.0f);
			// NOTE: Z,  0 to ColumnMax, negative for into the screen
			VerticeData[VertexIndex + 2] = -(float)Column;
			ColorData[VertexIndex + 2] = 0.0f;
		}
	}

	VertexIndex = 0;

	for (uint32 Rows = 0; Rows < ImageData->Width - 1; Rows++)
	{
		for (uint32 Columns = 0; Columns < ImageData->Height - 1; Columns++)
		{
			Indices[VertexIndex++] = (Rows + 1) * ImageData->Height + Columns;
			Indices[VertexIndex++] = (Rows + 1) * ImageData->Height + Columns + 1;
			Indices[VertexIndex++] = (Rows)* ImageData->Height + Columns;

			Indices[VertexIndex++] = (Rows)* ImageData->Height + Columns;
			Indices[VertexIndex++] = (Rows + 1) * ImageData->Height + Columns + 1;
			Indices[VertexIndex++] = (Rows)* ImageData->Height + Columns + 1;
		}
	}

	for (uint32 Row = 0; Row < ImageData->Width - 1; Row++)
	{
		for (uint32 Column = 0; Column < ImageData->Height - 1; Column++)
		{
			//v3 Line1 = Vertices[((Rows + 1) * ConvertedDepth + (Columns + 1))] -
			//	Vertices[((Rows + 1) * ConvertedDepth + (Columns))];
			//
			//v3 Line2 = Vertices[((Rows + 1) * ConvertedDepth + (Columns))] -
			//	Vertices[((Rows)* ConvertedDepth + (Columns))];
			uint32 Index1 = 3 * ((Row + 1) * ImageData->Height + (Column + 1));
			uint32 Index2 = 3 * ((Row + 1) * ImageData->Height + Column);
			uint32 Index3 = 3 * (Row * ImageData->Height + Column);

			v3 Line1;
			Line1.x = VerticeData[Index1 + 0] - VerticeData[Index2 + 0];
			Line1.y = VerticeData[Index1 + 1] - VerticeData[Index2 + 1];
			Line1.z = VerticeData[Index1 + 2] - VerticeData[Index2 + 2];

			v3 Line2;
			Line2.x = VerticeData[Index2 + 0] - VerticeData[Index3 + 0];
			Line2.y = VerticeData[Index2 + 1] - VerticeData[Index3 + 1];
			Line2.z = VerticeData[Index2 + 2] - VerticeData[Index3 + 2];

			v3 Normal = Math_CrossProduct(Line1, Line2);
			NormalData[3 * (Row + 1) * ImageData->Height + 3 * Column + 0] = Normal.x;
			NormalData[3 * (Row + 1) * ImageData->Height + 3 * Column + 1] = Normal.y;
			NormalData[3 * (Row + 1) * ImageData->Height + 3 * Column + 2] = Normal.z;
		}
	}

	for (uint32 Column = 0; Column < ImageData->Height - 1; Column++)
	{
		//v3 Line1 = Vertices[Columns + 1] -
		//	Vertices[Columns];
		//
		//v3 Line2 = Vertices[Columns + 1] -
		//	Vertices[ConvertedDepth + Columns + 1];

		uint32 Index1 = 3 * (Column + 1);
		uint32 Index2 = 3 * Column;
		uint32 Index3 = 3 * (ImageData->Height + (Column + 1));

		v3 Line1;
		Line1.x = VerticeData[Index1 + 0] - VerticeData[Index2 + 0];
		Line1.y = VerticeData[Index1 + 1] - VerticeData[Index2 + 1];
		Line1.z = VerticeData[Index1 + 2] - VerticeData[Index2 + 2];

		v3 Line2;
		Line2.x = VerticeData[Index1 + 0] - VerticeData[Index3 + 0];
		Line2.y = VerticeData[Index1 + 1] - VerticeData[Index3 + 1];
		Line2.z = VerticeData[Index1 + 2] - VerticeData[Index3 + 2];

		v3 Normal = Math_CrossProduct(Line1, Line2);
		NormalData[3 * (Column + 1) + 0] = Normal.x;
		NormalData[3 * (Column + 1) + 1] = Normal.y;
		NormalData[3 * (Column + 1) + 2] = Normal.z;
	}

	v3 FirstNormal;
	NormalData[0 + 0] = (NormalData[ImageData->Height + 0] + NormalData[3 + 0]) * 0.5f;
	NormalData[0 + 1] = (NormalData[ImageData->Height + 1] + NormalData[3 + 1]) * 0.5f;
	NormalData[0 + 2] = (NormalData[ImageData->Height + 2] + NormalData[3 + 2]) * 0.5f;

	for (uint32 Row = 2; Row < ImageData->Width; Row++)
	{
		//v3 Line1 = Vertices[Rows * ConvertedDepth - 1] -
		//	Vertices[Rows * ConvertedDepth - 2];
		//
		//v3 Line2 = Vertices[Rows * ConvertedDepth - 1] -
		//	Vertices[(Rows + 1) * ConvertedDepth - 1];
		uint32 Index1 = 3 * (Row * ImageData->Height - 1);
		uint32 Index2 = 3 * (Row * ImageData->Height - 2);
		uint32 Index3 = 3 * ((Row + 1) * ImageData->Height - 1);

		v3 Line1;
		Line1.x = VerticeData[Index1 + 0] - VerticeData[Index2 + 0];
		Line1.y = VerticeData[Index1 + 1] - VerticeData[Index2 + 1];
		Line1.z = VerticeData[Index1 + 2] - VerticeData[Index2 + 2];

		v3 Line2;
		Line2.x = VerticeData[Index1 + 0] - VerticeData[Index3 + 0];
		Line2.y = VerticeData[Index1 + 1] - VerticeData[Index3 + 1];
		Line2.z = VerticeData[Index1 + 2] - VerticeData[Index3 + 2];

		v3 Normal = Math_CrossProduct(Line1, Line2);
		NormalData[Row * ImageData->Height - 3 + 0] = Normal.x;
		NormalData[Row * ImageData->Height - 3 + 1] = Normal.y;
		NormalData[Row * ImageData->Height - 3 + 2] = Normal.z;
	}

	NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 0] =
		(NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 0 - 3] +
			NormalData[(ImageData->Width - 2) * (ImageData->Height - 1) + 0]) * 0.5f;

	NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 1] =
		(NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 1 - 3] +
			NormalData[(ImageData->Width - 2) * (ImageData->Height - 1) + 1]) * 0.5f;

	NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 2] =
		(NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 2 - 3] +
			NormalData[(ImageData->Width - 2) * (ImageData->Height - 1) + 2]) * 0.5f;

	// Average out the normals
	// v3 T0 = HeightMapPlane.VerticesPtr[(x*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - z)]; // center
	// v3 T1 = HeightMapPlane.VerticesPtr[((x - 1)*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - (z + 1))];
	// v3 T2 = HeightMapPlane.VerticesPtr[((x - 1)*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - z)];
	// v3 T3 = HeightMapPlane.VerticesPtr[(x*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - (z + 1))];
	// v3 T4 = HeightMapPlane.VerticesPtr[(x*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - (z - 1))];
	// v3 T5 = HeightMapPlane.VerticesPtr[((x + 1)*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - z)];
	// v3 T6 = HeightMapPlane.VerticesPtr[((x + 1)*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - (z - 1))];

	for (uint32 i = 0; i < TotalVertices; i++)
	{
		TextureCoords[i + 0] = 1.0f;
		TextureCoords[i + 1] = 1.0f;

		TextureCoords[i + 2] = 1.0f;
		TextureCoords[i + 3] = 0.0f;

		TextureCoords[i + 4] = 0.0f;
		TextureCoords[i + 5] = 0.0f;

		TextureCoords[i + 6] = 0.0f;
		TextureCoords[i + 7] = 1.0f;
	}

	ModelObj->NumAttribs = 4;
#if MEMORY_ON
	ModelObj->Data = 0;
	ModelObj->Data = Memory_Allocate(ModelObj->Data, ModelObj->NumAttribs);
	ModelObj->ArraySize = 0;
	ModelObj->ArraySize = Memory_Allocate(ModelObj->ArraySize,
		ModelObj->NumAttribs);
#else
	ModelObj->Data = new float*[ModelObj->NumAttribs];
	ModelObj->ArraySize = new uint32[ModelObj->NumAttribs];
#endif
	ModelObj->Data[0] = VerticeData;
	ModelObj->ArraySize[0] = 3 * TotalVertices * sizeof(float);
	ModelObj->Data[1] = ColorData;
	ModelObj->ArraySize[1] = 3 * TotalVertices * sizeof(float);
	ModelObj->Data[2] = TextureCoords;
	ModelObj->ArraySize[2] = 8 * TotalVertices * sizeof(float);
	ModelObj->Data[3] = NormalData;
	ModelObj->ArraySize[3] = 3 * TotalVertices * sizeof(float);

	ModelObj->IndiceData = Indices;
	ModelObj->IndiceCount = NumberOfIndices;
}

#else
RenderObject::~RenderObject()
{
	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = NULL;
	}
}

void RenderObject::Init() {}

void RenderObject::Init(Texture2D* ImageData) {}

void RenderObject::Init(uint32 width, uint32 depth) {}

void RenderObject::Init(float width, float height, float depth) {}

void RenderObject::InputTexture(Texture2D* Texture)
{
	Render_SetTexture(Texture->Data, Texture->Width,
		Texture->Height, &TextureID);
}

void RenderObject::Draw()
{
	if (NumberOfIndices)
	{
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID,
			TextureID, NumberOfIndices);
	}
	else
	{
		Render_DrawObject(ObjectDescription.VertexArrayObjectID, 
			TextureID, NumberOfVertices);
	}
}

void RenderObject::Draw(uint8 Choice)
{
	if (Choice == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID,
			TextureID, NumberOfIndices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID,
			TextureID, NumberOfIndices);
	}
}

void RenderObject::Delete()
{
	Render_DeleteBuffers(ObjectDescription.NumberOfVertexHandles,
		ObjectDescription.VertexArrayObjectID,
		ObjectDescription.VertexBufferObjectHandleIDs);
	Render_DeleteVertexArrays(1, &ObjectDescription.VertexArrayObjectID);
	Render_DeleteTexture(1, &TextureID);

	if (ObjectDescription.VertexBufferDescriptions)
	{
		delete[] ObjectDescription.VertexBufferDescriptions;
		ObjectDescription.VertexBufferDescriptions = 0;
	}

	if (ObjectDescription.VertexBufferObjectHandleIDs)
	{
		delete[] ObjectDescription.VertexBufferObjectHandleIDs;
		ObjectDescription.VertexBufferObjectHandleIDs = 0;
	}

	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = 0;
	}
}
#endif