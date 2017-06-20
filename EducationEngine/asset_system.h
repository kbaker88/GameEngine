#ifndef ASSET_SYSTEM_H
#define ASSET_SYSTEM_H

#include "camera_system.h"

struct Font
{
	// NOTE: Must be NULL terminated.
	char* Name;
	RenderObject* Glyph;
	uint32* GlyphImageID;
	float* Width;
	unsigned int GlyphsCount;
};

struct AssetLink 
{
	uint32* ID;
};

struct DropMenu
{
	//AssetLink*;
	Model** Models;
	RenderObject** RenderObj;
	CollisionObject** CollisionObj;
	m4* ModelMatrix;
};

// TODO: Add Support for UNICODE
uint32
Asset_LoadFont(char* FontName, char* FilePath);
Font* 
Asset_GetFont(uint32 FontID);
void
Asset_DeleteFont(uint32 FontID);
uint32 
Asset_LoadBMP(char* FileName);
uint32
Asset_LoadPNG(char* FileName);
Texture2D* 
Asset_GetTexture(uint32 TextureNumber);
void
Asset_Delete(uint32 AssetID);
void
Asset_DeleteAll();

uint32
Asset_CreateRect(float Width, float Height);
AssetLink*
Asset_GetLink(uint32 AssetID);
m4*
Asset_GetModelMatrix(uint32 AssetID);
RenderObject*
Asset_GetRenderObj(uint32 AssetID);
CollisionObject*
Asset_GetCollisionObj(uint32 AssetID);
void
Asset_DeleteObj(uint32 AssetID);

// NOTE: Test Functions
uint32
Asset_CreateDropMenu(float Width, float LineHeight,
	v3* Position);
uint32
Asset_CreateScrollBar(v3* ScrollBarPosition, float Width,
	float Height);
void
Asset_DrawScrollBar(uint32 ScrollBarID, int32 ShaderVar);
void
Asset_DeleteScrollBar(uint32 ScrollBarID);

#endif