#include "get_file.h"

GetFile::GetFile()
{

}
GetFile::~GetFile()
{

}

bool GetFile::DoesFileExist(const std::string& fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}

void GetFile::OpenFile(const std::string& fileName)
{
	FILE* openedFile;
	fopen_s(&openedFile, fileName.c_str(), "rb"); // rb = Read Binary

	if (openedFile != NULL)
	{
		size_t inData = fread(&bmpHeader, sizeof(BitmapHeader), 1, openedFile);

		uint32 typeOfFile;
		inData = fread(&typeOfFile, sizeof(typeOfFile), 1, openedFile);

		if (typeOfFile == 40) // Windows type
		{
			inData = fread(&bitmapInfo, sizeof(bitmapInfo), 1, openedFile);

			uint8 bmpData[4];
			uint32 pixel = 0;

			fseek(openedFile, 54, SEEK_SET);

			// uint8 *sourcedest;

			while (fread(&bmpData, sizeof(uint8), 4, openedFile) == 4)
			{
				pixel = (bmpData[3] << 24) | (bmpData[0] << 16) | (bmpData[1] << 8) | (bmpData[2]);
				imageData.push_back(pixel);
			}

			int x = 0;
		}
		else // OS/2 type
		{
			BitmapInfoOS2 bitmapInfo;
			inData = fread(&bitmapInfo, sizeof(bitmapInfo), 1, openedFile);

		}

		fclose(openedFile);
	}
	else
	{
		// Error: File failed to open.
	}
}


int32 GetFile::GetWidth()
{
	return bitmapInfo.bitmapWidth;
}

int32 GetFile::GetHeight()
{
	return bitmapInfo.bitmapHeight;
}

unsigned char* GetFile::GetImageData()
{
	return (unsigned char*)imageData.data();
}