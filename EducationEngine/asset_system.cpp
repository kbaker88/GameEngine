#include "asset_system.h"

void Asset_LoadTextures()
{
	TextureCount = 0;

	Asset_LoadBMP("Images/startbutton.bmp"); // 0
	Asset_LoadBMP("Images/menubutton.bmp"); // 1
	Asset_LoadBMP("Images/exitbutton.bmp"); // 2
	Asset_LoadBMP("Images/titlebutton.bmp"); // 3
	Asset_LoadBMP("Images/grass2.bmp"); // 4
	Asset_LoadBMP("Images/container.bmp"); // 5
	Asset_LoadBMP("Images/woodfloor.bmp"); // 6
	Asset_LoadPNG("Images/StartHeightMap.png"); // 7
	Asset_LoadBMP("Images/inputbar.bmp"); // 8
}

void Asset_LoadBMP(char* FileName)
{
	BmpDimensions TextureDimensions;
	unsigned char* BMPData = 0;
	BMPData = Platform_ReadFile(FileName);
	BMPData = GetBmpData(BMPData, TextureDimensions);
	Textures[TextureCount].Data = BMPData; 
	Textures[TextureCount].Width = TextureDimensions.Width;
	Textures[TextureCount].Height = TextureDimensions.Height;
	TextureCount++;
}

void Asset_LoadPNG(char* FileName)
{
	PNGProperties ImageProperties;
	unsigned char* PNGData;
	PNGData = Platform_ReadFile(FileName);
	PNGData = PNG_Extract(PNGData, ImageProperties);
	Textures[TextureCount].Data = PNGData;
	Textures[TextureCount].Width = ImageProperties.Width;
	Textures[TextureCount].Height = ImageProperties.Height;

	TextureCount++;
}

uint32 Asset_GetTextureCount()
{
	return TextureCount;
}

Texture2D* Asset_GetTexture(uint32 TextureNumber)
{
	return &Textures[TextureNumber];
}

void Asset_DeleteAll()
{
	for (uint32 Index = 0; Index < TextureCount; Index++)
	{
		delete[] Textures[Index].Data;
		Textures[Index].Data = 0;
		Textures[Index].Height = 0;
		Textures[Index].Width = 0;
	}
	TextureCount = 0;
}