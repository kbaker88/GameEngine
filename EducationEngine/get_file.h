#ifndef GET_FILE_H
#define GET_FILE_H

#include <sys/stat.h>
#include <string>
#include <vector>

#include "globals.h"

#pragma pack(push, 1)
struct BitmapHeader
{
	uint16 headerField;
	uint32 fileSize;
	uint16 reserved;
	uint16 reserved2;
	uint32 offset;
};

struct BitmapInfoWindows
{
	//uint32 infoSize;
	int32 bitmapWidth;
	int32 bitmapHeight;
	uint16 numberOfColorPlanes;
	uint16 bitsPerPixel;
	uint32 compressionMethod;
	uint32 sizeOfBitmapData;
	uint32 horizontalResolution;
	uint32 verticalResolution;
	uint32 numberOfColorsInPalette;
	uint32 numberOfImportantColors;
};

struct BitmapInfoOS2
{
	//uint32 infoSize;
	int16 bitmapWidth;
	int16 bitmapHeight;
	uint16 numberOfColorPlanes;
	uint16 bitsPerPixel;
};
#pragma pack(pop)

class GetFile
{
public:
	GetFile();
	~GetFile();

	bool DoesFileExist(const std::string& fileName);
	void OpenFile(const std::string& fileName);

	int32 GetWidth();
	int32 GetHeight();
	//uint32* GetImageData();
	unsigned char* GetImageData();

private:
	std::vector<uint32> imageData;
	BitmapInfoWindows bitmapInfo;
	BitmapHeader bmpHeader;
};

#endif