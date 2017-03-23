#include "heightmap.h"

HeightMap::HeightMap()
{
	NumberOfIndices = 0;
}

HeightMap::~HeightMap()
{
	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = 0;
	}
}

void HeightMap::Init(TextureStorage* ImageData)
{
	uint32 ConvertedWidth = ImageData->Width;
	uint32 ConvertedDepth = ImageData->Height;
	Width = static_cast<float>(ConvertedWidth);
	Depth = static_cast<float>(ConvertedDepth);
	NumberOfIndices = 6 * ConvertedWidth * ConvertedDepth;

	uint32 BytesPerPixel = (ImageData->NumPixelComps * ImageData->PixelCompSize) / 8;
	uint32 Pixel;
	uint32 TotalVertices = ConvertedWidth * ConvertedDepth;

	float *VerticePositions = new float[3 * TotalVertices]{};
	v3 *Vertices = new v3[TotalVertices]{};
	uint32 Index = 0;
	uint32 ImagePixelIndex = 0;
	for (uint32 Rows = 0; Rows < ConvertedWidth; Rows++)
	{
		for (uint32 Columns = 0; Columns < ConvertedDepth; Columns++)
		{
			VerticePositions[3 * Rows * ConvertedDepth + 3 * Columns + 0] =
				(float)Rows;
			Pixel = ImageData->data[ImagePixelIndex++] << 8 | ImageData->data[ImagePixelIndex++];
			VerticePositions[3 * Rows * ConvertedDepth + 3 * Columns + 1] =
				(float)Pixel / 1000.0f;
			VerticePositions[3 * Rows * ConvertedDepth + 3 * Columns + 2] =
				-(float)Columns;
			Vertices[Index] = v3((float)Rows,
				(float)Pixel / 1000.0f, -(float)Columns);
			Index++;
		}
	}

	VerticeFloatArrayPtr = VerticePositions;

	uint32 *Indices = new uint32[NumberOfIndices]{};
	Index = 0;
	for (uint32 Rows = 0; Rows < ConvertedWidth - 1; Rows++)
	{
		for (uint32 Columns = 0; Columns < ConvertedDepth - 1; Columns++)
		{
			Indices[Index++] = (Rows + 1) * ConvertedDepth + Columns;
			Indices[Index++] = (Rows + 1) * ConvertedDepth + Columns + 1;
			Indices[Index++] = (Rows)* ConvertedDepth + Columns;

			Indices[Index++] = (Rows)* ConvertedDepth + Columns;
			Indices[Index++] = (Rows + 1) * ConvertedDepth + Columns + 1;
			Indices[Index++] = (Rows)* ConvertedDepth + Columns + 1;
		}
	}

	float *ColorData = new float[3 * TotalVertices]{};
	Index = 0;
	ImagePixelIndex = 0;
	for (uint32 Rows = 0; Rows < ConvertedWidth; Rows++)
	{
		for (uint32 Columns = 0; Columns < ConvertedDepth; Columns++)
		{
			ColorData[3 * Rows * ConvertedDepth + 3 * Columns + 0] = 1.0f;
			Pixel = ImageData->data[ImagePixelIndex++] << 8 | ImageData->data[ImagePixelIndex++];
			ColorData[3 * Rows * ConvertedDepth + 3 * Columns + 1] =
				(float)((float)Pixel / 255.0f);
			ColorData[3 * Rows * ConvertedDepth + 3 * Columns + 2] = 0.0f;
			Index++;
		}
	}

	float *NormalData = new float[3 * TotalVertices]{};
	for (uint32 Rows = 0; Rows < ConvertedWidth - 1; Rows++)
	{
		for (uint32 Columns = 0; Columns < ConvertedDepth - 1; Columns++)
		{
			v3 Line1 = Vertices[((Rows + 1) * ConvertedDepth + (Columns + 1))] -
				Vertices[((Rows + 1) * ConvertedDepth + (Columns))];

			v3 Line2 = Vertices[((Rows + 1) * ConvertedDepth + (Columns))] -
				Vertices[((Rows)* ConvertedDepth + (Columns))];

			v3 Normal = CrossProduct(Line1, Line2);
			NormalData[3 * (Rows + 1) * ConvertedDepth + 3 * Columns + 0] = Normal.x;
			NormalData[3 * (Rows + 1) * ConvertedDepth + 3 * Columns + 1] = Normal.y;
			NormalData[3 * (Rows + 1) * ConvertedDepth + 3 * Columns + 2] = Normal.z;
		}
	}

	for (uint32 Columns = 0; Columns < ConvertedDepth - 1; Columns++)
	{
		v3 Line1 = Vertices[Columns + 1] -
			Vertices[Columns];

		v3 Line2 = Vertices[Columns + 1] -
			Vertices[ConvertedDepth + Columns + 1];

		v3 Normal = CrossProduct(Line1, Line2);
		NormalData[3 * (Columns + 1) + 0] = Normal.x;
		NormalData[3 * (Columns + 1) + 1] = Normal.y;
		NormalData[3 * (Columns + 1) + 2] = Normal.z;
	}

	v3 FirstNormal;
	NormalData[0 + 0] = (NormalData[ConvertedDepth + 0] + NormalData[3 + 0]) / 2.0f;
	NormalData[0 + 1] = (NormalData[ConvertedDepth + 1] + NormalData[3 + 1]) / 2.0f;
	NormalData[0 + 2] = (NormalData[ConvertedDepth + 2] + NormalData[3 + 2]) / 2.0f;

	for (uint32 Rows = 2; Rows < ConvertedWidth; Rows++)
	{
		v3 Line1 = Vertices[Rows * ConvertedDepth - 1] -
			Vertices[Rows * ConvertedDepth - 2];

		v3 Line2 = Vertices[Rows * ConvertedDepth - 1] -
			Vertices[(Rows + 1) * ConvertedDepth - 1];

		v3 Normal = CrossProduct(Line1, Line2);
		NormalData[Rows * ConvertedDepth - 3 + 0] = Normal.x;
		NormalData[Rows * ConvertedDepth - 3 + 1] = Normal.y;
		NormalData[Rows * ConvertedDepth - 3 + 2] = Normal.z;
	}

	NormalData[(ConvertedWidth - 1) * (ConvertedDepth - 1) + 0] =
		(NormalData[(ConvertedWidth - 1) * (ConvertedDepth - 1) + 0 - 3] +
			NormalData[(ConvertedWidth - 2) * (ConvertedDepth - 1)  + 0]) / 2.0f;

	NormalData[(ConvertedWidth - 1) * (ConvertedDepth - 1) + 1] =
		(NormalData[(ConvertedWidth - 1) * (ConvertedDepth - 1) + 1 - 3] +
			NormalData[(ConvertedWidth - 2) * (ConvertedDepth - 1)  + 1]) / 2.0f;

	NormalData[(ConvertedWidth - 1) * (ConvertedDepth - 1) + 2] =
		(NormalData[(ConvertedWidth - 1) * (ConvertedDepth - 1) + 2 - 3] +
			NormalData[(ConvertedWidth - 2) * (ConvertedDepth - 1)  + 2]) / 2.0f;

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


	ObjectDescription.NumberOfVertexHandles = 5;

	ObjectDescription.VertexBufferDescriptions[1].FloatData = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[1].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[0].Uint32Data = Indices;
	ObjectDescription.VertexBufferDescriptions[0].Size = NumberOfIndices * sizeof(uint32);
					  
	ObjectDescription.VertexBufferDescriptions[2].FloatData = ColorData;
	ObjectDescription.VertexBufferDescriptions[2].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[3].FloatData = NormalData;
	ObjectDescription.VertexBufferDescriptions[3].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[4].FloatData = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[4].Size = 8 * TotalVertices * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[4].Offset = 2;

	Render_ObjectPipelineInit(ObjectDescription);

	delete[] Indices;
	delete[] ColorData;
	delete[] NormalData;
	delete[] TextureCoords;
}