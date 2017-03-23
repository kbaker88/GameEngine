#ifndef ASSET_SYSTEM_H
#define ASSET_SYSTEM_H

#include "bmp.h"
#include "png.h"

struct TextureStorage
{
	uint8* data;
	uint32 Width;
	uint32 Height;
	uint32 NumPixelComps;
	uint32 PixelCompSize;
};

static TextureStorage Textures[512];
static uint32 TextureCount;

void Asset_LoadTextures();
void Asset_LoadBMP(char* FileName);

void Asset_LoadPNG(char* FileName);
uint32 Asset_GetTextureCount();
TextureStorage* Asset_GetTexture(uint32 TextureNumber);

void Asset_DeleteAll();
/* TO DO
- Clean up textures glDeleteTextures(1, &Texture); and delete[] TextureData;

*/

#endif