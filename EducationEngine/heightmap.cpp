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
	TerrainMaxWidth = ImageData->Width;
	TerrainMaxDepth = ImageData->Height;

	NumberOfIndices = 6 * TerrainMaxWidth * TerrainMaxDepth;

	uint32 BytesPerPixel = (ImageData->NumPixelComps * ImageData->PixelCompSize) / 8;
	uint32 Pixel;
	uint32 TotalVertices = TerrainMaxWidth * TerrainMaxDepth;

	float *VerticePositions = new float[3 * TotalVertices]{};
	v3 *Vertices = new v3[TotalVertices]{};
	uint32 Index = 0;
	uint32 ImagePixelIndex = 0;
	for (uint32 Rows = 0; Rows < TerrainMaxWidth; Rows++)
	{
		for (uint32 Columns = 0; Columns < TerrainMaxDepth; Columns++)
		{
			VerticePositions[3 * Rows * TerrainMaxDepth + 3 * Columns + 0] =
				(float)Rows;
			Pixel = ImageData->data[ImagePixelIndex++] << 8 | ImageData->data[ImagePixelIndex++];
			VerticePositions[3 * Rows * TerrainMaxDepth + 3 * Columns + 1] =
				(float)Pixel / 1000.0f;
			VerticePositions[3 * Rows * TerrainMaxDepth + 3 * Columns + 2] =
				-(float)Columns;
			Vertices[Index] = v3((float)Rows,
				(float)Pixel / 1000.0f, -(float)Columns);
			Index++;
		}
	}

	VerticeFloatArrayPtr = VerticePositions;

	uint32 *Indices = new uint32[NumberOfIndices]{};
	Index = 0;
	for (uint32 Rows = 0; Rows < TerrainMaxWidth - 1; Rows++)
	{
		for (uint32 Columns = 0; Columns < TerrainMaxDepth - 1; Columns++)
		{
			Indices[Index++] = (Rows + 1) * TerrainMaxDepth + Columns;
			Indices[Index++] = (Rows + 1) * TerrainMaxDepth + Columns + 1;
			Indices[Index++] = (Rows)* TerrainMaxDepth + Columns;

			Indices[Index++] = (Rows)* TerrainMaxDepth + Columns;
			Indices[Index++] = (Rows + 1) * TerrainMaxDepth + Columns + 1;
			Indices[Index++] = (Rows)* TerrainMaxDepth + Columns + 1;
		}
	}

	float *ColorData = new float[3 * TotalVertices]{};
	Index = 0;
	ImagePixelIndex = 0;
	for (uint32 Rows = 0; Rows < TerrainMaxWidth; Rows++)
	{
		for (uint32 Columns = 0; Columns < TerrainMaxDepth; Columns++)
		{
			ColorData[3 * Rows * TerrainMaxDepth + 3 * Columns + 0] = 1.0f;
			Pixel = ImageData->data[ImagePixelIndex++] << 8 | ImageData->data[ImagePixelIndex++];
			ColorData[3 * Rows * TerrainMaxDepth + 3 * Columns + 1] =
				(float)((float)Pixel / 255.0f);
			ColorData[3 * Rows * TerrainMaxDepth + 3 * Columns + 2] = 0.0f;
			Index++;
		}
	}

	float *NormalData = new float[3 * TotalVertices]{};
	for (uint32 Rows = 0; Rows < TerrainMaxWidth - 1; Rows++)
	{
		for (uint32 Columns = 0; Columns < TerrainMaxDepth - 1; Columns++)
		{
			v3 Line1 = Vertices[((Rows + 1) * TerrainMaxDepth + (Columns + 1))] -
				Vertices[((Rows + 1) * TerrainMaxDepth + (Columns))];

			v3 Line2 = Vertices[((Rows + 1) * TerrainMaxDepth + (Columns))] -
				Vertices[((Rows)* TerrainMaxDepth + (Columns))];

			v3 Normal = CrossProduct(Line1, Line2);
			NormalData[3 * (Rows + 1) * TerrainMaxDepth + 3 * Columns + 0] = Normal.x;
			NormalData[3 * (Rows + 1) * TerrainMaxDepth + 3 * Columns + 1] = Normal.y;
			NormalData[3 * (Rows + 1) * TerrainMaxDepth + 3 * Columns + 2] = Normal.z;
		}
	}

	for (uint32 Columns = 0; Columns < TerrainMaxDepth - 1; Columns++)
	{
		v3 Line1 = Vertices[Columns + 1] -
			Vertices[Columns];

		v3 Line2 = Vertices[Columns + 1] -
			Vertices[TerrainMaxDepth + Columns + 1];

		v3 Normal = CrossProduct(Line1, Line2);
		NormalData[3 * (Columns + 1) + 0] = Normal.x;
		NormalData[3 * (Columns + 1) + 1] = Normal.y;
		NormalData[3 * (Columns + 1) + 2] = Normal.z;
	}

	v3 FirstNormal;
	NormalData[0 + 0] = (NormalData[TerrainMaxDepth + 0] + NormalData[3 + 0]) / 2.0f;
	NormalData[0 + 1] = (NormalData[TerrainMaxDepth + 1] + NormalData[3 + 1]) / 2.0f;
	NormalData[0 + 2] = (NormalData[TerrainMaxDepth + 2] + NormalData[3 + 2]) / 2.0f;

	for (uint32 Rows = 2; Rows < TerrainMaxWidth; Rows++)
	{
		v3 Line1 = Vertices[Rows * TerrainMaxDepth - 1] -
			Vertices[Rows * TerrainMaxDepth - 2];

		v3 Line2 = Vertices[Rows * TerrainMaxDepth - 1] -
			Vertices[(Rows + 1) * TerrainMaxDepth - 1];

		v3 Normal = CrossProduct(Line1, Line2);
		NormalData[Rows * TerrainMaxDepth - 3 + 0] = Normal.x;
		NormalData[Rows * TerrainMaxDepth - 3 + 1] = Normal.y;
		NormalData[Rows * TerrainMaxDepth - 3 + 2] = Normal.z;
	}

	NormalData[(TerrainMaxWidth - 1) * (TerrainMaxDepth - 1) + 0] = 
		(NormalData[(TerrainMaxWidth - 1) * (TerrainMaxDepth - 1) + 0 - 3] + 
			NormalData[(TerrainMaxWidth - 2) * (TerrainMaxDepth - 1)  + 0]) / 2.0f;

	NormalData[(TerrainMaxWidth - 1) * (TerrainMaxDepth - 1) + 1] = 
		(NormalData[(TerrainMaxWidth - 1) * (TerrainMaxDepth - 1) + 1 - 3] +
			NormalData[(TerrainMaxWidth - 2) * (TerrainMaxDepth - 1)  + 1]) / 2.0f;

	NormalData[(TerrainMaxWidth - 1) * (TerrainMaxDepth - 1) + 2] = 
		(NormalData[(TerrainMaxWidth - 1) * (TerrainMaxDepth - 1) + 2 - 3] + 
			NormalData[(TerrainMaxWidth - 2) * (TerrainMaxDepth - 1)  + 2]) / 2.0f;

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

	ObjectDescription.Description[1].Data = VerticePositions;
	ObjectDescription.Description[1].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.Description[1].Offset = 3;

	ObjectDescription.Description[0].IndiceData = Indices;
	ObjectDescription.Description[0].Size = NumberOfIndices * sizeof(uint32);

	ObjectDescription.Description[2].Data = ColorData;
	ObjectDescription.Description[2].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.Description[2].Offset = 3;

	ObjectDescription.Description[3].Data = NormalData;
	ObjectDescription.Description[3].Size = 3 * TotalVertices * sizeof(float);
	ObjectDescription.Description[3].Offset = 3;

	ObjectDescription.Description[4].Data = TextureCoords;
	ObjectDescription.Description[4].Size = 8 * TotalVertices * sizeof(float);
	ObjectDescription.Description[4].Offset = 2;

	Render_ObjectPipelineInit(ObjectDescription);

	delete[] Indices;
	delete[] ColorData;
	delete[] NormalData;
	delete[] TextureCoords;
}

void HeightMap::InputTexture(TextureStorage* Texture)
{
	Render_SetTexture(Texture->data, Texture->Width,
		Texture->Height, TextureID);
}

void HeightMap::UpdateColorVertice(PipelineObjectDescription& Description, 
	uint32 ArrayPosition, float* Color)
{
	float* GPUMemoryMapPtr = (float*)Render_GetObjectShaderDataPtr(
		Description, ArrayPosition, 1);
	*GPUMemoryMapPtr = Color[0];
	*++GPUMemoryMapPtr = Color[1];
	*++GPUMemoryMapPtr = Color[2];

	Render_UnmapShaderDataPtr();
}

void HeightMap::DrawIndices()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Render_DrawObjectIndices(ObjectDescription, TextureID, NumberOfIndices);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}