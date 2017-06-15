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

#endif