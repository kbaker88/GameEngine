#ifndef PLATFORM_LAYER_H
#define PLATFORM_LAYER_H

//TODO: Remove this later, find a better solution.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define Assert(Expression) / if (!(Expression)) {*(int *)0 = 0;}
#define Kilobytes(value) ((value) * 1024)
#define Megabytes(value) (Kilobytes(value) * 1024)
#define Gigabytes(value) (Megabytes(value) * 1024)

static const float SecondsPerFrame = (1.0f / 60.0f);
static const float UnitsPerMeter = 0.25f;

typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int  uint32;
typedef long long int64;
typedef unsigned long long uint64;

struct platform_handel;
struct platform_properties;

struct window_properties
{
	int Width;
	int Height;
};

// ERROR SYSTEM
// TODO : REPLACE WITH INGAME TEXT ERROR SYSTEM
void Platform_TemporaryError(char* Text); 

// FILE SYSTEM
struct loaded_bitmap
{
	int32 Width;
	int32 Height;
	int32 Pitch;
	void *Memory;

	void *Free;
};

int8 Platform_DoesFileExist(char* FileName);
unsigned char* Platform_ReadFile(char* FileName);

void Platform_SetupFont(char* FileName, char* FontName, void** Bits);
loaded_bitmap Platform_LoadGlyph(void* Bits, uint32 CodePoint);

// TIME SYSTEM
int64 Platform_GetCPUCounter();
int64 Platform_GetTimeFrequency();

// INPUT DEVICES
// TODO: Should this be Long?
void Platform_GetCursorPosition(float *X, float *Y);
int8 Platform_GetStateOfKey(int key);

void Platform_UpdateMouseState(uint32 State);
uint32 Platform_GetMouseState();

// CORE FUNCTIONALITY
// TODO: Remember to check for already initialized
void Platform_Initialize(); 
// TODO: Put as much of this in renderer file as possible
void Platform_InitRenderer(); 
void Platform_UpdateWindowSize(uint32 Width, uint32 Height);
void Platform_EndProgram();
void Platform_Cleanup(); 

#endif
