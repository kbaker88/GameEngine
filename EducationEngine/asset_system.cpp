#include "asset_system.h"

unsigned char* TextureData = NULL;
BmpDimensions TextureDimensions;

PNGProperties ImageProperties;
unsigned char* PNGData = NULL;


void Asset_LoadTextures()
{
	TextureCount = 0;

	Asset_LoadBMP("Images/startbutton.bmp"); // 0
	Asset_LoadBMP("Images/startbuttonover.bmp"); // 1
	Asset_LoadBMP("Images/menubutton.bmp"); // 2
	Asset_LoadBMP("Images/grass2.bmp"); // 3
	Asset_LoadBMP("Images/container.bmp"); // 4
	Asset_LoadBMP("Images/containeractive.bmp"); // 5
	Asset_LoadBMP("Images/menubuttonover.bmp"); // 6
	Asset_LoadBMP("Images/exitbutton.bmp"); // 7
	Asset_LoadBMP("Images/exitbuttonover.bmp"); // 8
	Asset_LoadBMP("Images/titlebutton.bmp"); // 9
	Asset_LoadBMP("Images/titlebuttonover.bmp"); // 10
	Asset_LoadBMP("Images/woodfloor.bmp"); // 11
	Asset_LoadBMP("Images/heightmap.bmp"); // 12
	Asset_LoadBMP("Images/test2heightmap.bmp"); // 13
	Asset_LoadBMP("Images/heightmap02.bmp"); // 14
	Asset_LoadPNG("Images/StartHeightMap.png"); // 15
}

void Asset_LoadBMP(char* FileName)
{
	TextureData = Platform_ReadFile(FileName);
	TextureData = GetBmpData(TextureData, TextureDimensions);
	Textures[TextureCount].data = TextureData;
	Textures[TextureCount].Width = TextureDimensions.Width;
	Textures[TextureCount].Height = TextureDimensions.Height;
	TextureCount++;
}

void Asset_LoadPNG(char* FileName)
{
	PNGData = Platform_ReadFile(FileName);
	PNGData = GetPNGData(PNGData, ImageProperties);
	Textures[TextureCount].data = PNGData;
	Textures[TextureCount].Width = ImageProperties.Width;
	Textures[TextureCount].Height = ImageProperties.Height;
	Textures[TextureCount].NumPixelComps = ImageProperties.NumPixelComps;
	Textures[TextureCount].PixelCompSize = ImageProperties.PixelCompSize;
	TextureCount++;
	//delete[] PNGData;
}
/*
void LoadGlyphs()
{
	loaded_bitmap test;

	uint8* TextureData = NULL;
	BmpDimensions TextureDimensions;

	void* Bits;
	SetupFont("c:/Windows/Fonts/arial.ttf", "Courier New", Bits);
	for (uint32 Character = 'A'; Character < 'Z'; Character++)
	{
		test = LoadGlyphFromPlatform("c:/Windows/Fonts/arial.ttf", "Courier New", Character);
		TextureData = (uint8*)test.Memory;
		TextureDimensions.Width = test.Width;
		TextureDimensions.Height = test.Height;

		Textures[TextureCount].data = TextureData;
		Textures[TextureCount].dimension = TextureDimensions;
		TextureCount++;
	}
}*/

uint32 Asset_GetTextureCount()
{
	return TextureCount;
}

TextureStorage* Asset_GetTexture(uint32 TextureNumber)
{
	return &Textures[TextureNumber];
}