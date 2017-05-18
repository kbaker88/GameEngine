#ifndef BMP_H
#define BMP_H

#include "utility.h"
/*#pragma pack(push, 1)

#pragma pack(pop)*/


// MEMORY LEAK?
struct BmpDimensions
{
	int32 Width;
	int32 Height;
};

static unsigned char* 
GetBmpData(unsigned char* Data, BmpDimensions& Dimensions)
{
	unsigned char* ImageData = 0;

	uint32 SizeOFHeader = (Data[14]) | (Data[15] << 8) |
		(Data[16] << 16) | (Data[17] << 24);

	if (SizeOFHeader == 40) // Windows Type
	{
		Dimensions.Width = (Data[18]) | (Data[19] << 8) |
			(Data[20] << 16) | (Data[21] << 24);

		Dimensions.Height = (Data[22]) | (Data[23] << 8) |
			(Data[24] << 16) | (Data[25] << 24);

		uint32 pixel = 0;
		ImageData = new unsigned char[Dimensions.Width * Dimensions.Height * 4]();

		int PixelItr = 54;

		for (int i = 0; i < Dimensions.Width * Dimensions.Height; i++)
		{
			pixel = (Data[PixelItr + 3] << 24) | (Data[PixelItr] << 16) | (Data[PixelItr + 1] << 8) | (Data[PixelItr + 2]);

			ImageData[PixelItr - 54 + 3] = Data[PixelItr + 3];
			ImageData[PixelItr - 54 + 2] = Data[PixelItr + 0];
			ImageData[PixelItr - 54 + 1] = Data[PixelItr + 1];
			ImageData[PixelItr - 54 + 0] = Data[PixelItr + 2];

			PixelItr = PixelItr + 4;
		}

#if MEMORY_ON
	// TODO: Make a deallocation function.
#else
		delete[] Data;
#endif

	}
	else
	{
		// OS/2 type
	}

	return ImageData;
}

#endif