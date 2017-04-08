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


static int32 PaethPredictor(int32 Previous, int32 Above, int32 AbovePrev)
{
	int32 p = Previous + Above - AbovePrev;
	int32 pa = Math_AbsVal(p - Previous);
	int32 pb = Math_AbsVal(p - Above);
	int32 pc = Math_AbsVal(p - AbovePrev);
	if ((pa <= pb) && (pa <= pc))
	{
		return Previous;
	}
	else if (pb <= pc)
	{
		return Above;
	}
	else
	{
		return AbovePrev;
	}
}

static void NoFilter(uint32* Index, uint8* Data, uint32* ImgIndex, 
	uint8* ImageData, uint32 ScanLineWidth)
{
	//*Index = *Index + 1;
	for (uint32 LinePosition = 0; LinePosition <
		(ScanLineWidth); LinePosition++)
	{
		ImageData[*ImgIndex] = Data[*Index];
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		ImageData[*ImgIndex] = Data[*Index];
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;
	}
}

static void SubFilter(uint32* Index, uint8* Data, uint32* ImgIndex, 
	uint8* ImageData, uint32 ScanLineWidth)
{
	//*Index = *Index + 1;
	ImageData[*ImgIndex] = Data[*Index];
	*ImgIndex = *ImgIndex + 1;
	*Index = *Index + 1;

	ImageData[*ImgIndex] = Data[*Index];
	*ImgIndex = *ImgIndex + 1;
	*Index = *Index + 1;
	for (uint32 LinePosition = 0; LinePosition < 
		(ScanLineWidth - 1); LinePosition++)
	{
		ImageData[*ImgIndex] =
			Data[*Index] + ImageData[*ImgIndex - 2];
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		ImageData[*ImgIndex] =
			Data[*Index] + ImageData[*ImgIndex - 2];
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;
	}
}

static void UpFilter(uint32* Index, uint8* Data, uint32* ImgIndex,
	uint8* ImageData, uint32 ScanLineWidth)
{
	//*Index = *Index + 1;
	for (uint32 LinePosition = 0;
		LinePosition < ScanLineWidth; 
		LinePosition++)
	{
		ImageData[*ImgIndex] = Data[*Index] + 
			ImageData[*ImgIndex - (ScanLineWidth * 2)];
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		ImageData[*ImgIndex] = Data[*Index] + 
			ImageData[*ImgIndex - (ScanLineWidth * 2)];
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;
	}
}

static void AverageFilter(uint32* Index, uint8* Data, uint32* ImgIndex,
	uint8* ImageData, uint32 ScanLineWidth, uint32 ScanLine)
{
	//*Index = *Index + 1;
	if (ScanLine > 0)
	{
		ImageData[*ImgIndex] = Data[*Index] + 
			(int)Math_Floor(ImageData[*ImgIndex - 
			(ScanLineWidth * 2)] * 0.5f);
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		ImageData[*ImgIndex] = Data[*Index] + 
			(int)Math_Floor(ImageData[*ImgIndex -
			(ScanLineWidth * 2)] * 0.5f);
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		for (uint32 LinePosition = 0; 
			LinePosition < (ScanLineWidth - 1);
			LinePosition++)
		{
			ImageData[*ImgIndex] = Data[*Index] + 
				(int)Math_Floor((ImageData[*ImgIndex - 2] +
				ImageData[*ImgIndex - (ScanLineWidth * 2)]) * 0.5f);
			*ImgIndex = *ImgIndex + 1;
			*Index = *Index + 1;

			ImageData[*ImgIndex] = Data[*Index] + 
				(int)Math_Floor((ImageData[*ImgIndex - 2] +
				ImageData[*ImgIndex - (ScanLineWidth * 2)]) * 0.5f);
			*ImgIndex = *ImgIndex + 1;
			*Index = *Index + 1;
		}
	}
	else
	{
		ImageData[*ImgIndex] = Data[*Index];
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		ImageData[*ImgIndex] = Data[*Index];
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		for (uint32 LinePosition = 0; 
			LinePosition < (ScanLineWidth - 1); 
			LinePosition++)
		{
			ImageData[*ImgIndex] = Data[*Index] + 
				(int)Math_Floor((ImageData[*ImgIndex - 2] + 0) * 0.5f);
			*ImgIndex = *ImgIndex + 1;
			*Index = *Index + 1;

			ImageData[*ImgIndex] = Data[*Index] + 
				(int)Math_Floor((ImageData[*ImgIndex - 2] + 0) * 0.5f);
			*ImgIndex = *ImgIndex + 1;
			*Index = *Index + 1;
		}
	}
}

static void PaethFilter(uint32* Index, uint8* Data, uint32* ImgIndex,
	uint8* ImageData, uint32 ScanLineWidth, uint32 ScanLine)
{
	//*Index = *Index + 1;
	if (ScanLine > 0)
	{
		ImageData[*ImgIndex] = Data[*Index] + PaethPredictor(0,
			ImageData[*ImgIndex - (ScanLineWidth * 2)], 0);
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		ImageData[*ImgIndex] = Data[*Index] + PaethPredictor(0,
			ImageData[*ImgIndex - (ScanLineWidth * 2)], 0);
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		for (uint32 LinePosition = 0;
			LinePosition < (ScanLineWidth - 1);
			LinePosition++)
		{
			ImageData[*ImgIndex] = Data[*Index] +
				PaethPredictor(ImageData[*ImgIndex - 2],
				ImageData[*ImgIndex - (ScanLineWidth * 2)],
				ImageData[*ImgIndex - (ScanLineWidth * 2) - 2]);
			*ImgIndex = *ImgIndex + 1;
			*Index = *Index + 1;

			ImageData[*ImgIndex] = Data[*Index] + 
				PaethPredictor(ImageData[*ImgIndex - 2],
				ImageData[*ImgIndex - (ScanLineWidth * 2)],
				ImageData[*ImgIndex - (ScanLineWidth * 2) - 2]);
			*ImgIndex = *ImgIndex + 1;
			*Index = *Index + 1;
		}
	}
	else
	{
		ImageData[*ImgIndex] = Data[*Index] + 
			PaethPredictor(0, 0, 0);
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		ImageData[*ImgIndex] = Data[*Index] +
			PaethPredictor(0, 0, 0);
		*ImgIndex = *ImgIndex + 1;
		*Index = *Index + 1;

		for (uint32 LinePosition = 0;
			LinePosition < (ScanLineWidth - 1);
			LinePosition++)
		{
			ImageData[*ImgIndex] = Data[*Index] + 
				PaethPredictor(ImageData[*ImgIndex - 2], 0, 0);
			*ImgIndex = *ImgIndex + 1;
			*Index = *Index + 1;

			ImageData[*ImgIndex] = Data[*Index] +
				PaethPredictor(ImageData[*ImgIndex - 2], 0, 0);
			*ImgIndex = *ImgIndex + 1;
			*Index = *Index + 1;
		}
	}
}

static unsigned char* GetPNGData(unsigned char* Data, 
	PNGProperties& Properties)
{
	unsigned char* ImageData = 0;

	//NOTE: Known and required PNG names.
	unsigned long long TrueSignature = 727905341920923785;
	int32 IHDR = 1229472850;
	int32 IEND = 1229278788;
	int32 IDAT = 1229209940;

	//NOTE: Confirm that the data is from a PNG file.
	unsigned long long* Signature = (unsigned long long*)Data;
	if (*Signature != TrueSignature)
	{ 
		return 0; //TODO: Debug
	}

	//NOTE: Read the IHDR chunk.
	/*
		Width:              4 bytes
		Height:             4 bytes
		Bit depth:          1 byte
		Color type:         1 byte
		Compression method: 1 byte
		Filter method:      1 byte
		Interlace method:   1 byte
	*/
	int32 ChunkLength = Data[11] | (Data[10] << 8) | (Data[9] << 16) |
		(Data[8] << 24);
	int32 ChunkName = Data[15] | (Data[14] << 8) | (Data[13] << 16) |
		(Data[12] << 24);

	if (ChunkName == IHDR)
	{
		int32 Width = (Data[19]) | (Data[18] << 8) | (Data[17] << 16) |
			(Data[16] << 24);
		Properties.Width = Width;
		int32 Height = (Data[23]) | (Data[22] << 8) | (Data[21] << 16) |
			(Data[20] << 24);
		Properties.Height = Height;
		Properties.BitDepth = Data[24];
		Properties.ColorType = Data[25];
		Properties.CompressionMethod = Data[26];
		Properties.FilterMethod = Data[27];
		Properties.InterlaceMethod = Data[28];
	}
	else
	{
		return 0; //TODO: Debug
	}
	int32 ChunkCRC = Data[32] | (Data[31] << 8) | (Data[30] << 16) | (Data[29] << 24);

	if (Properties.Width && Properties.Height)
	{
		ImageData = new unsigned char[Properties.Width * Properties.Height * 2];
	}
	else
	{
		return 0; //TODO: Debug, Technically not an error.
	}

	uint32 i = 33;
	uint32 ImgIndex = 0;

	//NOTE: Loop through all chunks
	while (1)
	{
		ChunkLength = (Data[i + 3]) | (Data[i + 2] << 8) |
			(Data[i + 1] << 16) | (Data[i] << 24);
		ChunkName = Data[i + 7] | (Data[i + 6] << 8) |
			(Data[i + 5] << 16) | (Data[i + 4] << 24);

		i = i + 8;

		if (ChunkName == IEND)
		{
			delete[] Data;
			return ImageData;
		}
		else if (ChunkName == IDAT)
		{
			//NOTE: ZLib Deflate/Inflate Compression information.
			//TODO: Handle more options from just 78 01 (like 78 9C or 78 DA).
			uint8 CompressionMethod = Data[i];
			i++;

			uint8 AdditionalFlags = Data[i + 1];
			i++;

			//TODO: Handle the possibility of a DICTID?

			//NOTE: Index + ChunkLength - Two Flag Bytes - 4 ADLER32 Bytes
			uint32 ZlibDataLength = i + ChunkLength - 6;

			//NOTE: Loop through the image data (Deflate compress algorithm)
			while (i < ZlibDataLength)
			{ 
			//NOTE: Block Header
			/*
			bit 1: Last block boolean.
			bits 2-3:  00 - Raw data between 0 and 65,535 bytes in length.
			           01 - Static Huffman compressed block, pree-agreed tree.
					   10 - Compressed block with the Huffman table supplied.
					   11 - Reserved.
			*/
				int8 BlockHeader = Data[i]; 
				i++;

				if ((BlockHeader << 7) == 0x00)
				{
					int16 BlockLength = (Data[i]) |
						(Data[i + 1] << 8);
					i = i + 2;

					int16 BlockLength1sComplement = (Data[i]) |
						(Data[i + 1] << 8);
					i = i + 2;
	
					int32 BlockScanlineCount =
						BlockLength / (Properties.Width * 2 + 1);
			
					for (int32 ScanLine = 0;
						ScanLine < BlockScanlineCount;
						ScanLine++)
					{
						if (Data[i] == 0) 
						{
							i++;
							NoFilter(&i, Data, &ImgIndex, 
								ImageData, Properties.Width);
						}
						else if (Data[i] == 1) 
						{
							i++;
							SubFilter(&i, Data, &ImgIndex, 
								ImageData, Properties.Width);
						}
						else if (Data[i] == 2) 
						{
							i++;
							UpFilter(&i, Data, &ImgIndex,
								ImageData, Properties.Width);
						}
						else if (Data[i] == 3)
						{
							i++;
							AverageFilter(&i, Data, &ImgIndex,
								ImageData, Properties.Width, ScanLine);
		
						}
						else if (Data[i] == 4) 
						{
							i++;
							PaethFilter(&i, Data, &ImgIndex,
								ImageData, Properties.Width, ScanLine);
						}
					}
				}
				else if ((BlockHeader << 7 ) == 0x80)
				{
					int16 BlockLength = (Data[i]) | 
						(Data[i + 1] << 8);
					i = i + 2;

					int16 BlockLength1sComplement = (Data[i]) |
						(Data[i + 1] << 8);
					i = i + 2;
				}
			}

			int32 ADLER32Checksum = (Data[i + 3]) | (Data[i + 2] << 8) |
				(Data[i + 1] << 16) | (Data[i] << 24);
			i = i + 4;
		}
		else if (i == 900000)
		{
			break; //TODO: Debug
		}
		else
		{
			//NOTE: Increment the index past the chunk data.
			i = i + ChunkLength;
		}
		ChunkCRC = Data[i + 3] | (Data[i + 2] << 8) | 
			(Data[i + 1] << 16) | (Data[i] << 24);;
		i = i + 4;
	}

	return 0;
}

#endif 