#include "heightmap.h"

HeightMap::~HeightMap()
{
	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = 0;
	}
}

void HeightMap::Init(Texture2D* ImageData)
{
	ObjectDescription.NumberOfVertexHandles = 4;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	uint32 TotalVertices = ImageData->Width * ImageData->Height;
	NumberOfVertices = TotalVertices;
	Width = (float)ImageData->Width;
	Depth = (float)ImageData->Height;

	NumberOfIndices = 6 * TotalVertices;

	uint32 Pixel = 0;

	float *VerticePositions = new float[3 * TotalVertices]{};
	float *ColorData = new float[3 * TotalVertices]{};

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
			VerticePositions[VertexIndex + 0] = (float)Row;
			ColorData[VertexIndex + 0] = 1.0f;
			// NOTE: Y, Height
			VerticePositions[VertexIndex + 1] = Height;
			ColorData[VertexIndex + 1] = (float)((float)Pixel / 255.0f);
			// NOTE: Z,  0 to ColumnMax, negative for into the screen
			VerticePositions[VertexIndex + 2] = -(float)Column;
			ColorData[VertexIndex + 2] = 0.0f;
		}
	}

	VerticeFloatArrayPtr = VerticePositions;
	VertexIndex = 0;

	uint32 *Indices = new uint32[NumberOfIndices]{};

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

	float *NormalData = new float[3 * TotalVertices]{};
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
			Line1.x = VerticePositions[Index1 + 0] - VerticePositions[Index2 + 0];
			Line1.y = VerticePositions[Index1 + 1] - VerticePositions[Index2 + 1];
			Line1.z = VerticePositions[Index1 + 2] - VerticePositions[Index2 + 2];

			v3 Line2;
			Line2.x = VerticePositions[Index2 + 0] - VerticePositions[Index3 + 0];
			Line2.y = VerticePositions[Index2 + 1] - VerticePositions[Index3 + 1];
			Line2.z = VerticePositions[Index2 + 2] - VerticePositions[Index3 + 2];

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
		Line1.x = VerticePositions[Index1 + 0] - VerticePositions[Index2 + 0];
		Line1.y = VerticePositions[Index1 + 1] - VerticePositions[Index2 + 1];
		Line1.z = VerticePositions[Index1 + 2] - VerticePositions[Index2 + 2];

		v3 Line2;
		Line2.x = VerticePositions[Index1 + 0] - VerticePositions[Index3 + 0];
		Line2.y = VerticePositions[Index1 + 1] - VerticePositions[Index3 + 1];
		Line2.z = VerticePositions[Index1 + 2] - VerticePositions[Index3 + 2];

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
		Line1.x = VerticePositions[Index1 + 0] - VerticePositions[Index2 + 0];
		Line1.y = VerticePositions[Index1 + 1] - VerticePositions[Index2 + 1];
		Line1.z = VerticePositions[Index1 + 2] - VerticePositions[Index2 + 2];

		v3 Line2;
		Line2.x = VerticePositions[Index1 + 0] - VerticePositions[Index3 + 0];
		Line2.y = VerticePositions[Index1 + 1] - VerticePositions[Index3 + 1];
		Line2.z = VerticePositions[Index1 + 2] - VerticePositions[Index3 + 2];

		v3 Normal = Math_CrossProduct(Line1, Line2);
		NormalData[Row * ImageData->Height - 3 + 0] = Normal.x;
		NormalData[Row * ImageData->Height - 3 + 1] = Normal.y;
		NormalData[Row * ImageData->Height - 3 + 2] = Normal.z;
	}

	NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 0] =
		(NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 0 - 3] +
			NormalData[(ImageData->Width - 2) * (ImageData->Height - 1)  + 0]) * 0.5f;

	NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 1] =
		(NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 1 - 3] +
			NormalData[(ImageData->Width - 2) * (ImageData->Height - 1)  + 1]) * 0.5f;

	NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 2] =
		(NormalData[(ImageData->Width - 1) * (ImageData->Height - 1) + 2 - 3] +
			NormalData[(ImageData->Width - 2) * (ImageData->Height - 1)  + 2]) * 0.5f;

	// Average out the normals
	// v3 T0 = HeightMapPlane.VerticesPtr[(x*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - z)]; // center
	// v3 T1 = HeightMapPlane.VerticesPtr[((x - 1)*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - (z + 1))];
	// v3 T2 = HeightMapPlane.VerticesPtr[((x - 1)*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - z)];
	// v3 T3 = HeightMapPlane.VerticesPtr[(x*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - (z + 1))];
	// v3 T4 = HeightMapPlane.VerticesPtr[(x*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - (z - 1))];
	// v3 T5 = HeightMapPlane.VerticesPtr[((x + 1)*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - z)];
	// v3 T6 = HeightMapPlane.VerticesPtr[((x + 1)*HeightMapPlane.TerrainMaxDepth) + (HeightMapPlane.TerrainMaxDepth - (z - 1))];


	float *TextureCoords = new float[8 * TotalVertices]{};

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

	ObjectDescription.IndiceDescription.Data = Indices;
	ObjectDescription.IndiceDescription.Size = NumberOfIndices * sizeof(uint32);

	ObjectDescription.VertexBufferDescriptions[0].Data = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[1].Data = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[2].Data = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[2].Size = 8 * TotalVertices * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 2;
				  
	ObjectDescription.VertexBufferDescriptions[3].Data = NormalData;
	ObjectDescription.VertexBufferDescriptions[3].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 3;
					  

	Render_ObjectPipelineInit(&ObjectDescription);

	delete[] Indices;
	delete[] ColorData;
	delete[] NormalData;
	delete[] TextureCoords;
}