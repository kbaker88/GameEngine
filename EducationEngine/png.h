#ifndef PNG_H
#define PNG_H

#include "platform_layer.h"
#include "math.h"

//TODO: Make more robust

struct PNGProperties
{
	int32 Width;
	int32 Height;
	unsigned char BitDepth;
	unsigned char ColorType;
	unsigned char CompressionMethod;
	unsigned char FilterMethod;
	unsigned char InterlaceMethod;
};


//static int32 PaethPredictor(int32 Previous, int32 Above, int32 AbovePrev);
//
//static void NoFilter(uint32* Index, uint8* Data, uint32* ImgIndex,
//	uint8* ImageData, uint32 ScanLineWidth);
//
//static void SubFilter(uint32* Index, uint8* Data, uint32* ImgIndex,
//	uint8* ImageData, uint32 ScanLineWidth);
//
//static void UpFilter(uint32* Index, uint8* Data, uint32* ImgIndex,
//	uint8* ImageData, uint32 ScanLineWidth);
//
//static void AverageFilter(uint32* Index, uint8* Data, uint32* ImgIndex,
//	uint8* ImageData, uint32 ScanLineWidth, uint32 ScanLine);
//
//static void PaethFilter(uint32* Index, uint8* Data, uint32* ImgIndex,
//	uint8* ImageData, uint32 ScanLineWidth, uint32 ScanLine);
//
unsigned char* PNG_Extract(unsigned char* Data,
	PNGProperties& Properties);

#endif 