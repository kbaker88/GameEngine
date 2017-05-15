#ifndef PNG_H
#define PNG_H

#include "platform_layer.h"
#include "math.h"

//TODO: Make more robust. 
//NOTE: Only handles 16-bit greyscale right now.

struct PNGProperties
{
	uint32 Width;
	uint32 Height;
	uint32 WidthInBytes;
	uint32 BytesPerPixel;
	unsigned char BitDepth;
	unsigned char ColorType;
	unsigned char CompressionMethod;
	unsigned char FilterMethod;
	unsigned char InterlaceMethod;
};

unsigned char* 
PNG_Extract(unsigned char* Data,
	PNGProperties& Properties);

#endif 