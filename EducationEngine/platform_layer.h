#ifndef PLATFORM_LAYER_H
#define PLATFORM_LAYER_H

//TODO: Remove LEAN AND MEAN later, find a better solution.
//NOTE: LEAN AND MEAN is here for include redundancies in WinSock2.h
//		in the network layer.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DATA_ORIENTED 0 //TODO: Temporary while transfering

#define DEBUG_MODE 1

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
	window_properties() : Width(0), Height(0) {}
	int Width;
	int Height;
};

struct Texture2D
{
	Texture2D() : Data(0), Width(0), Height(0) {}
	uint8* Data;
	uint32 Width;
	uint32 Height;
};


void* 
Platform_AllocateMemory(unsigned long long *Size);
void
Platform_DeallocateMemory(void* MemoryPtr, 
	unsigned long long *Size);

// ERROR SYSTEM
// TODO : REPLACE WITH INGAME TEXT ERROR SYSTEM
void 
Platform_TemporaryError(char* Text); 

// FILE SYSTEM
int8 
Platform_DoesFileExist(char* FileName);
// NOTE: Platform_ReadFile dynamically allocates memory,
//		 make sure to clean.
unsigned char* 
Platform_ReadFile(char* FileName);

// NOTE: Platform_SetupFont obtains a device context,
//		 release it later.
void 
Platform_SetupFont(char* FileName, char* FontName, 
	void** Bits,
	void** HDCPtr);
// NOTE: Platform_LoadGlyph dynamically allocates memory,
//		 make sure to clean.
void 
Platform_LoadGlyph(void* Bits, uint16 Glyph, 
	Texture2D *TextureInfo, void* HDCPtr);

// TIME SYSTEM
int64
Platform_GetCPUCounter();
int64 
Platform_GetTimeFrequency();

// INPUT DEVICES
// TODO: Should this be Long?
void 
Platform_GetCursorPosition(float *X, float *Y);
int8
Platform_GetStateOfKey(int key);

void 
Platform_UpdateMouseState(uint32 State);
uint32
Platform_GetMouseState();

// CORE FUNCTIONALITY
// TODO: Remember to check for already initialized
void 
Platform_Initialize(window_properties *WindowProps);
void
Platform_ShowWindow(int CommandShow);
unsigned int 
Platform_MessageLoop();
void
Platform_ReleaseContext(void* DeviceContext);

// TODO: Put as much of this in renderer file as possible
void
Platform_InitRenderer(); 
void
Platform_UpdateWindowSize(uint32 Width, uint32 Height);
void 
Platform_EndProgram();
void
Platform_Cleanup(); 

#endif
