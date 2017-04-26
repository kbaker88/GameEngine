#ifndef ASSET_SYSTEM_H
#define ASSET_SYSTEM_H

#include "bmp.h"
#include "png.h"

static Texture2D Textures[512];
static uint32 TextureCount;

void Asset_LoadTextures();
void Asset_LoadFont(char* FontName, char* FilePath,
	Texture2D* GlyphArray);
void Asset_LoadBMP(char* FileName);
void Asset_LoadPNG(char* FileName);
uint32 Asset_GetTextureCount();
Texture2D* Asset_GetTexture(uint32 TextureNumber);

void Asset_DeleteAll();

#endif