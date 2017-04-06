#ifndef PNG_H
#define PNG_H

#include "platform_layer.h"
#include "math.h"

//TODO: Make more robust

struct PNGProperties
{
	int32 Width;
	int32 Height;
	uint32 NumPixelComps;
	uint32 PixelCompSize;
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

static unsigned char* GetPNGData(unsigned char* Data, PNGProperties& Properties)
{
	unsigned char* ImageData = 0;

	// Check if PNG signature is true
	if ((Data[0] != 137) || (Data[1] != 80) ||
		(Data[2] != 78 ) || (Data[3] != 71))
	{
		return 0;
	}
	if ((Data[4] != 13) || (Data[5] != 10) ||
		(Data[6] != 26) || (Data[7] != 10))
	{
		return 0;
	}
	 // Length 4-byte
	 // IHDR 4-byte
	int32 Legnth = (Data[11]) | (Data[10] << 8) |
			(Data[9] << 16) | (Data[8] << 24);

	int32 Width = (Data[19]) | (Data[18] << 8) |
		(Data[17] << 16) | (Data[16] << 24);

	int32 Height = (Data[23]) | (Data[22] << 8) |
		(Data[21] << 16) | (Data[20] << 24);
	 // CRC 4-byte

	Properties.Width = Width;
	Properties.Height = Height;
	Properties.NumPixelComps = 1;  // Remove hard coding later (16-bit greyscale)
	Properties.PixelCompSize = 16; // Remove hard coding later (16-bit greyscale)

	ImageData = new unsigned char[Properties.Width * Properties.Height * 2];

	int32 BitDepth = Data[24];
	int32 ColorType = Data[25];
	int32 Compression = Data[26];
	int32 Filter = Data[27];
	int32 Interlace = Data[28];
	// 29 30 31 32
	
	uint32 i = 33;
	uint32 ImgIndex = 0;
	while (1)
	{
		int32 Length = (Data[i + 3]) | (Data[i + 2] << 8) |
			(Data[i + 1] << 16) | (Data[i] << 24);

		unsigned char Name[4] = { Data[i + 4] , Data[i + 5] ,
			Data[i + 6] , Data[i + 7] };
		i = i + 7;

		if ((Name[0] == 'I') && (Name[1] == 'E') &&
			(Name[2] == 'N') && (Name[3] == 'D'))
		{
			delete[] Data;
			return ImageData;
		}
		else if ((Name[0] == 'I') && (Name[1] == 'D') &&
			(Name[2] == 'A') && (Name[3] == 'T'))
		{
			// zlib compression info + 2 bytes
			i = i + 3;

			for (uint32 Index = i; Index < (i + Length - 6); Index++) 
			{// for whole data chunk minux z - lib (6 bytes)
				int32 LastBlock = Data[Index];
				if (LastBlock == 0)
				{
					Index++;
					int32 BlockLength = (Data[Index]) | (Data[Index + 1] << 8);
					Index = Index + 2;
					int32 BlockScanlineCount = BlockLength / (Width * 2 + 1);
					Index = Index + 2; // To Ignore the Length's one complement
					for (int32 Scanline = 0; Scanline < BlockScanlineCount; Scanline++)
					{
						if (Data[Index] == 0) // No Filter
						{
							Index++;
							for (int32 LinePosition = 0; LinePosition < (Width); LinePosition++)
							{
								//copy as is
								ImageData[ImgIndex] = Data[Index];
								ImgIndex++;
								Index++;
								ImageData[ImgIndex] = Data[Index];
								ImgIndex++;
								Index++;
							}
						}
						else if (Data[Index] == 1) // Sub
						{
							Index++;
							ImageData[ImgIndex] = Data[Index];
							ImgIndex++;
							Index++;
							ImageData[ImgIndex] = Data[Index];
							ImgIndex++;
							Index++;
							for (int32 LinePosition = 0; LinePosition < (Width - 1); LinePosition++)
							{
								ImageData[ImgIndex] = 
									Data[Index] + ImageData[ImgIndex - 2];
								ImgIndex++;
								Index++;
								ImageData[ImgIndex] = 
									Data[Index] + ImageData[ImgIndex - 2];
								ImgIndex++;
								Index++;
							}
						}
						else if (Data[Index] == 2) // Above
						{
							Index++;
							for (int32 LinePosition = 0; LinePosition < (Width); LinePosition++)
							{
								ImageData[ImgIndex] = 
									Data[Index] + ImageData[ImgIndex - (Width * 2)];
								ImgIndex++;
								Index++;
								ImageData[ImgIndex] = 
									Data[Index] + ImageData[ImgIndex - (Width * 2)];
								ImgIndex++;
								Index++;
							}
						}
						else if (Data[Index] == 3) // Average
						{
							Index++;
							if (Scanline > 0)
							{
								Index++;
								ImageData[ImgIndex] = 
									Data[Index] + (int)Math_Floor((0 + ImageData[ImgIndex - (Width * 2)]) * 0.5f);
								ImgIndex++;
								Index++;
								ImageData[ImgIndex] = 
									Data[Index] + (int)Math_Floor((0 + ImageData[ImgIndex - (Width * 2)]) * 0.5f);
								ImgIndex++;
								Index++;
								for (int32 LinePosition = 0; LinePosition < (Width - 1); LinePosition++)
								{
									ImageData[ImgIndex] = 
										Data[Index] + (int)Math_Floor((ImageData[ImgIndex - 2] + ImageData[ImgIndex - (Width * 2)]) * 0.5f);
									ImgIndex++;
									Index++;
									ImageData[ImgIndex] =
										Data[Index] + (int)Math_Floor((ImageData[ImgIndex - 2] + ImageData[ImgIndex - (Width * 2)]) * 0.5f);
									ImgIndex++;
									Index++;
								}
							}
							else
							{
								Index++;
								ImageData[ImgIndex] = Data[Index];
								ImgIndex++;
								Index++;
								ImageData[ImgIndex] = Data[Index];
								ImgIndex++;
								Index++;
								for (int32 LinePosition = 0; LinePosition < (Width - 1); LinePosition++)
								{
									ImageData[ImgIndex] = 
										Data[Index] + (int)Math_Floor((ImageData[ImgIndex - 2] + 0) * 0.5f);
									ImgIndex++;
									Index++;
									ImageData[ImgIndex] = 
										Data[Index] + (int)Math_Floor((ImageData[ImgIndex - 2] + 0) * 0.5f);
									ImgIndex++;
									Index++;
								}
							}
						}
						else if (Data[Index] == 4) // Paeth
						{
							Index++;
							if (Scanline > 0)
							{
								ImageData[ImgIndex] = Data[Index] + PaethPredictor(0,
									ImageData[ImgIndex - (Width * 2)], 0);
								ImgIndex++;
								Index++;
								ImageData[ImgIndex] = Data[Index] + PaethPredictor(0,
									ImageData[ImgIndex - (Width * 2)], 0);
								ImgIndex++;
								Index++;
								for (int32 LinePosition = 0; LinePosition < (Width - 1); LinePosition++)
								{
									ImageData[ImgIndex] = Data[Index] + PaethPredictor(ImageData[ImgIndex - 2],
										ImageData[ImgIndex - (Width * 2)],
										ImageData[ImgIndex - (Width * 2) - 2]);
									ImgIndex++;
									Index++;
									ImageData[ImgIndex] = Data[Index] + PaethPredictor(ImageData[ImgIndex - 2],
										ImageData[ImgIndex - (Width * 2)],
										ImageData[ImgIndex - (Width * 2) - 2]);
									ImgIndex++;
									Index++;
								}
							}
							else
							{
								ImageData[ImgIndex] = Data[Index] + PaethPredictor(0, 0, 0);
								ImgIndex++;
								Index++;
								ImageData[ImgIndex] = Data[Index] + PaethPredictor(0, 0, 0);
								ImgIndex++;
								Index++;
								for (int32 LinePosition = 0; LinePosition < (Width - 1); LinePosition++)
								{
									//copy above
									ImageData[ImgIndex] = Data[Index] + PaethPredictor(ImageData[ImgIndex - 2], 0, 0);
									ImgIndex++;
									Index++;
									ImageData[ImgIndex] = Data[Index] + PaethPredictor(ImageData[ImgIndex - 2], 0, 0);
									ImgIndex++;
									Index++;
								}
							}
						}
					}
					Index--;
				}
				else if (LastBlock == 1)
				{
					Index++;
					int32 BlockLength = (Data[Index]) | (Data[Index + 1] << 8);
					Index = Index + 2;
					Index = Index + 2; // Length ones compliment
					//i = Index + 4;
					// Do Nothing?
				}
			}
			i = i - 3; // to remove from the start of this if
		}
		else if (i == 900000)
		{
			break; // error
		}


		i = i + Length;
		i = i + 4;
		i++;

	}

	return 0;
}

#endif 