#include "asset_system.h"

// NOTE: Temporary variables
static Texture2D* Texture[512];
static uint32 TextureCount = 0;

static Font* Fonts[8];
static uint32 FontCount = 0;

static Model* ModelData[512];
static uint32 ModelCount = 0;

static RenderObject* RenderObj[512];
static uint32 RenderObjCount = 0;

static CollisionObject* CollisionObj[512];
static uint32 CollisionObjCount = 0;

static m4* ModelMatrix[512];
static uint32 ModelMatrixCount = 0;

static AssetLink* AssetLinks[512];
static uint32 AssetLinkCount;
// ENDNOTE


uint32
Asset_LoadFont(char* FontName, char* FilePath)
{
	Fonts[FontCount] = new Font;
	Fonts[FontCount]->Name = FontName;
	// TODO: Fix this to add support for multiple languages
	uint32 GlyphCount = 126 - 32;
	Fonts[FontCount]->GlyphsCount = GlyphCount;

	Texture2D* GlyphTexture = 
		new Texture2D[Fonts[FontCount]->GlyphsCount];
	Fonts[FontCount]->Glyph = 
		new RenderObject[Fonts[FontCount]->GlyphsCount];
	Fonts[FontCount]->GlyphImageID = 
		new uint32[Fonts[FontCount]->GlyphsCount];
	Fonts[FontCount]->Width = 
		new float[Fonts[FontCount]->GlyphsCount];

	void* Bits = 0;
	void* DeviceContext = 0;
	Platform_SetupFont(FilePath, FontName, &Bits, &DeviceContext);
	//"c:/Windows/Fonts/cour.ttf", "Courier New", Character

	for (uint32 GlyphIndex = 0;
		GlyphIndex < Fonts[FontCount]->GlyphsCount;
		GlyphIndex++)
	{
		Platform_LoadGlyph(Bits, GlyphIndex + 32,
			&GlyphTexture[GlyphIndex], DeviceContext);

		Model GlyphRect;
		ModelObj_CreateRectangle(&GlyphRect,
			(float)GlyphTexture[GlyphIndex].Width,
			(float)GlyphTexture[GlyphIndex].Height);

		RenderObj_CreateRenderObject(
			&Fonts[FontCount]->Glyph[GlyphIndex], &GlyphRect);

		Render_BuildTexture(
			&Fonts[FontCount]->GlyphImageID[GlyphIndex],
			GlyphTexture[GlyphIndex].Width,
			GlyphTexture[GlyphIndex].Height,
			GlyphTexture[GlyphIndex].Data);

		Fonts[FontCount]->Width[GlyphIndex] =
			(float)GlyphTexture[GlyphIndex].Width;

#if MEMORY_ON
		// TODO: Make cleanup system
	}
#else
		delete[] GlyphTexture[GlyphIndex].Data;
	}
	delete[] GlyphTexture;
#endif // MEMORY_ON

	// TODO: Call DeleteObject() on Bitmap and Font
	// TODO: Device Context is for win32 only. Generalize
	Platform_ReleaseContext(DeviceContext);

	return FontCount;
}

Font*
Asset_GetFont(uint32 FontID)
{
	return Fonts[FontID];
}

void
Asset_DeleteFont(uint32 FontID)
{
	delete[] Fonts[FontID]->Glyph;
	delete[] Fonts[FontID]->GlyphImageID;
	delete[] Fonts[FontID]->Width;
	delete Fonts[FontID];
}

uint32 
Asset_LoadBMP(char* FileName)
{
	BmpDimensions TextureDimensions;
	unsigned char* BMPData = 0;
	BMPData = Platform_ReadFile(FileName);
	BMPData = GetBmpData(BMPData, TextureDimensions);
#if MEMORY_ON

#else
	Texture[TextureCount] = new Texture2D;
#endif // MEMORY_ON
	Texture[TextureCount]->Data = BMPData;
	Texture[TextureCount]->Width = TextureDimensions.Width;
	Texture[TextureCount]->Height = TextureDimensions.Height;
	TextureCount++;
	return(TextureCount - 1);
}

uint32 
Asset_LoadPNG(char* FileName)
{
	PNGProperties ImageProperties;
	unsigned char* PNGData;
	PNGData = Platform_ReadFile(FileName);
	PNGData = PNG_Extract(PNGData, ImageProperties);
#if MEMORY_ON

#else
	Texture[TextureCount] = new Texture2D;
#endif // MEMORY_ON
	Texture[TextureCount]->Data = PNGData;
	Texture[TextureCount]->Width = ImageProperties.Width;
	Texture[TextureCount]->Height = ImageProperties.Height;
	TextureCount++;
	return(TextureCount - 1);
}

Texture2D*
Asset_GetTexture(uint32 TextureNumber)
{
	return Texture[TextureNumber];
}

void
Asset_Delete(uint32 AssetID)
{
#if DEBUG_MODE
	if (Texture[AssetID])
	{
		if (Texture[AssetID]->Data)
		{
#if MEMORY_ON

#else
			delete[] Texture[AssetID]->Data;
#endif
		}
#if MEMORY_ON

#else
		delete Texture[AssetID];
#endif
		Texture[AssetID] = 0;
	}
	else
	{
		// Report Error
	}
#else
#if MEMORY_ON

#else
	delete[] Texture[AssetID]->Data;
	delete Texture[AssetID];
#endif // MEMORY_ON
#endif // DEBUG_ON
	Texture[AssetID] = 0;
}

void
Asset_DeleteAll()
{
	for (uint32 Index = 0; Index < TextureCount; Index++)
	{
#if MEMORY_ON

#else
		if (Texture[Index])
		{
			if (Texture[Index]->Data)
			{
				delete[] Texture[Index]->Data;
			}
			delete Texture[Index];
			Texture[Index] = 0;
		}
#endif // MEMORY_ON
	}
	TextureCount = 0;
}


uint32
Asset_CreateRect(float Width, float Height)
{
#if MEMORY_ON

#else
	AssetLinks[AssetLinkCount] = new AssetLink;
	ModelData[ModelCount] = new Model;
	RenderObj[RenderObjCount] = new RenderObject;
	CollisionObj[CollisionObjCount] = new CollisionObject;
	ModelMatrix[ModelMatrixCount] = new m4;
	AssetLinks[AssetLinkCount]->ID = new uint32[4];
#endif

	AssetLinks[AssetLinkCount]->ID[0] = ModelCount;
	AssetLinks[AssetLinkCount]->ID[1] = RenderObjCount;
	AssetLinks[AssetLinkCount]->ID[2] = CollisionObjCount;
	AssetLinks[AssetLinkCount]->ID[3] = ModelMatrixCount;


	ModelObj_CreateRectangle( ModelData[ModelCount], 
		160.0f, 40.0f);
	
	RenderObj_CreateRenderObject( RenderObj[RenderObjCount],
		ModelData[ModelCount]);
	
	Collision_FillObject(CollisionObj[CollisionObjCount],
		Width, Height, 0.0f, &v3(0.0f, 0.0f, 0.0f));
	CollisionObj[CollisionObjCount]->CollisionCode = 0;

	AssetLinkCount++;
	ModelCount++;
	RenderObjCount++;
	CollisionObjCount++;
	ModelMatrixCount++;

	return(AssetLinkCount - 1);
}

AssetLink*
Asset_GetLink(uint32 AssetID)
{
	return AssetLinks[AssetID];
}

m4*
Asset_GetModelMatrix(uint32 AssetID)
{
	return ModelMatrix[AssetLinks[AssetID]->ID[3]];
}

RenderObject*
Asset_GetRenderObj(uint32 AssetID)
{
	return RenderObj[AssetLinks[AssetID]->ID[1]];
}

CollisionObject*
Asset_GetCollisionObj(uint32 AssetID)
{
	return CollisionObj[AssetLinks[AssetID]->ID[2]];
}

void
Asset_DeleteObj(uint32 AssetID)
{
	ModelObj_Delete(ModelData[AssetLinks[AssetID]->ID[0]]);
	RenderObj_Delete(RenderObj[AssetLinks[AssetID]->ID[1]]);

#if MEMORY_ON

#else
	delete CollisionObj[AssetLinks[AssetID]->ID[2]];
	CollisionObj[AssetLinks[AssetID]->ID[2]] = 0;
	delete ModelMatrix[AssetLinks[AssetID]->ID[3]];
	ModelMatrix[AssetLinks[AssetID]->ID[3]] = 0;
	delete AssetLinks[AssetID];
	AssetLinks[AssetID] = 0;
#endif
}


// NOTE: Test Functions
uint32
Asset_CreateDropMenu(float Width, float LineHeight,
	v3* Position)
{


#if MEMORY_ON

#else
	AssetLinks[AssetLinkCount] = new AssetLink;
	ModelData[ModelCount] = new Model;
	RenderObj[RenderObjCount] = new RenderObject;
	CollisionObj[CollisionObjCount] = new CollisionObject;
	ModelMatrix[ModelMatrixCount] = new m4;
	AssetLinks[AssetLinkCount]->ID = new uint32[4];
#endif

	AssetLinks[AssetLinkCount]->ID[0] = ModelCount;
	AssetLinks[AssetLinkCount]->ID[1] = RenderObjCount;
	AssetLinks[AssetLinkCount]->ID[2] = CollisionObjCount;
	AssetLinks[AssetLinkCount]->ID[3] = ModelMatrixCount;


	ModelObj_CreateRectangle(ModelData[ModelCount],
		20.0f, 20.0f);
	RenderObj_CreateRenderObject(RenderObj[RenderObjCount],
		ModelData[ModelCount]);

	Collision_FillObject(CollisionObj[CollisionObjCount],
		Width, LineHeight, 0.0f, &v3(0.0f, 0.0f, 0.0f));
	CollisionObj[CollisionObjCount]->CollisionCode = 0;

	AssetLinkCount++;
	ModelCount++;
	RenderObjCount++;
	CollisionObjCount++;
	ModelMatrixCount++;

	return(AssetLinkCount - 1);
}

uint32 ScrollBarCount = 0;

struct ScrollBar
{
	Model* ModelData[3];
	RenderObject* RenderData[3];
	CollisionObject* Collision[3];
	m4* ModelMatrix[4];
	v3 Position;
};

ScrollBar* ScrollBars[2];

// TODO: Set a minimum height and width;
uint32
Asset_CreateScrollBar(v3* ScrollBarPosition, float Width,
	float Height)
{
	ScrollBars[ScrollBarCount] = new ScrollBar;

	for (uint8 Index = 0;
		Index < 3;
		Index++)
	{
		ScrollBars[ScrollBarCount]->ModelData[Index] = 
			new Model;
		ScrollBars[ScrollBarCount]->RenderData[Index] =
			new RenderObject;
		ScrollBars[ScrollBarCount]->Collision[Index] =
			new CollisionObject;
		ScrollBars[ScrollBarCount]->ModelMatrix[Index] =
			new m4;

		*ScrollBars[ScrollBarCount]->ModelMatrix[Index] =
			Math_IdentityMatrix();
	}
	ScrollBars[ScrollBarCount]->ModelMatrix[3] =
		new m4;
	*ScrollBars[ScrollBarCount]->ModelMatrix[3] =
		Math_IdentityMatrix();

	ScrollBars[ScrollBarCount]->Position =
		*ScrollBarPosition;

	ModelObj_CreateRectangle(
		ScrollBars[ScrollBarCount]->ModelData[0],
		20.0f, 20.0f);
	RenderObj_CreateRenderObject(
		ScrollBars[ScrollBarCount]->RenderData[0],
		ScrollBars[ScrollBarCount]->ModelData[0]);

	ModelObj_CreateRectangle(
		ScrollBars[ScrollBarCount]->ModelData[1],
		20.0f, Height - 40.0f);
	RenderObj_CreateRenderObject(
		ScrollBars[ScrollBarCount]->RenderData[1],
		ScrollBars[ScrollBarCount]->ModelData[1]);

	ModelObj_CreateRectangle(
		ScrollBars[ScrollBarCount]->ModelData[2],
		20.0f, 60.0f);
	RenderObj_CreateRenderObject(
		ScrollBars[ScrollBarCount]->RenderData[2],
		ScrollBars[ScrollBarCount]->ModelData[2]);

	ScrollBarPosition->z = 0.9f;
	*ScrollBars[ScrollBarCount]->ModelMatrix[0] =
		Math_TranslateMatrix(
			ScrollBars[ScrollBarCount]->ModelMatrix[0],
			ScrollBarPosition);
	Collision_FillObject(
		ScrollBars[ScrollBarCount]->Collision[0],
		20.0f, 20.0f, 0.0f, ScrollBarPosition);
	ScrollBars[ScrollBarCount]->
		Collision[0]->CollisionCode = 0;

	ScrollBarPosition->y += 20.0f;
	ScrollBarPosition->z = 0.8f;
	*ScrollBars[ScrollBarCount]->ModelMatrix[1] =
		Math_TranslateMatrix(
			ScrollBars[ScrollBarCount]->ModelMatrix[1],
			ScrollBarPosition);
	Collision_FillObject(
		ScrollBars[ScrollBarCount]->Collision[1],
		20.0f, Height - 40.0f, 0.0f, ScrollBarPosition);
	ScrollBars[ScrollBarCount]->
		Collision[1]->CollisionCode = 0;

	ScrollBarPosition->y += Height - 80.0f;
	ScrollBarPosition->z = 0.9f;
	*ScrollBars[ScrollBarCount]->ModelMatrix[2] =
		Math_TranslateMatrix(
			ScrollBars[ScrollBarCount]->ModelMatrix[2],
			ScrollBarPosition);
	Collision_FillObject(
		ScrollBars[ScrollBarCount]->Collision[2],
		20.0f, 60.0f, 0.0f, ScrollBarPosition);
	ScrollBars[ScrollBarCount]->
		Collision[2]->CollisionCode = 0;

	ScrollBarPosition->z = 0.9f;
	ScrollBarPosition->y += 60.0f;
	*ScrollBars[ScrollBarCount]->ModelMatrix[3] =
		Math_TranslateMatrix(
			ScrollBars[ScrollBarCount]->ModelMatrix[3],
			ScrollBarPosition);
	Collision_FillObject(
		ScrollBars[ScrollBarCount]->Collision[0],
		20.0f, 20.0f, 0.0f, ScrollBarPosition);
	ScrollBars[ScrollBarCount]->
		Collision[0]->CollisionCode = 0;

	ScrollBarCount++;

	return(ScrollBarCount - 1);
}

uint32
Asset_ScrollBarCollision(uint32 ScrollBarID)
{
	if (ScrollBars[ScrollBarCount]->Collision[1])
	{
		return 0;
	}
}

void
Asset_DrawScrollBar(uint32 ScrollBarID, int32 ShaderVar)
{
	Render_UpdateShaderVariable(ShaderVar, 44,
		&ScrollBars[ScrollBarID]->ModelMatrix[0]->Rc[0][0],
		1, 0);
	Render_Draw(ScrollBars[ScrollBarID]->RenderData[0]);

	Render_UpdateShaderVariable(ShaderVar, 44,
		&ScrollBars[ScrollBarID]->ModelMatrix[1]->Rc[0][0],
		1, 0);
	Render_Draw(ScrollBars[ScrollBarID]->RenderData[1]);
	
	Render_UpdateShaderVariable(ShaderVar, 44,
		&ScrollBars[ScrollBarID]->ModelMatrix[2]->Rc[0][0],
		1, 0);
	Render_Draw(ScrollBars[ScrollBarID]->RenderData[2]);
	
	Render_UpdateShaderVariable(ShaderVar, 44,
		&ScrollBars[ScrollBarID]->ModelMatrix[3]->Rc[0][0],
		1, 0);
	Render_Draw(ScrollBars[ScrollBarID]->RenderData[0]);

}

void
Asset_DeleteScrollBar(uint32 ScrollBarID)
{
	for (uint8 Index = 0;
		Index < 3;
		Index++)
	{
		delete ScrollBars[ScrollBarID]->ModelData[Index];
		delete ScrollBars[ScrollBarID]->RenderData[Index];
		delete ScrollBars[ScrollBarID]->Collision[Index];
		delete ScrollBars[ScrollBarID]->ModelMatrix[Index];
	}
	delete ScrollBars[ScrollBarID]->ModelMatrix[3];
	delete ScrollBars[ScrollBarID];
	ScrollBars[ScrollBarID] = 0;
}