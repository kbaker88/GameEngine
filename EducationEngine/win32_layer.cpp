#include "game_layer.h"
#include <Windows.h>

// Accepted values for attribute names for the OpenGL Context(WGL)
#define WGL_CONTEXT_MAJOR_VERSION_ARB				0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB				0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB					0x2093
#define WGL_CONTEXT_FLAGS_ARB						0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB				0x9126

// Accepted values for flag bits for the OpenGL Context(WGL)
#define WGL_CONTEXT_DEBUG_BIT_ARB					0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB		0x0002

// Accepted values for attribute bits for the OpenGL Context(WLG)
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB			0X00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB	0X00000002

// TODO: Why was this needed?
struct platform_handel
{
	HWND Window;
};

struct platform_properties
{
	HINSTANCE Instance;
	HDC DeviceContext;
	int64 TimerFrequency; 
	int64 TimerValue; 
};

static platform_handel PlatformWindow = {};
static platform_properties PlatformProperties = {};

//NOTE: Setting up a function pointer's return and parameter
//      types for creating an OpenGL Context.
typedef HGLRC __stdcall wgl_create_context_attribs_arb(HDC hDC,
					HGLRC hShareContext, const int *attribList);

LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, 
	LPARAM lParam);

// TEMPORARY: Throws these in a struct later? Or get rid of them
uint32 MouseState;
uint8 MouseLeftClickToggle = 0;

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int CommandShow)
{
	return Game_Main(CommandShow);
}

void 
Platform_UpdateWindowSize(uint32 Width, uint32 Height)
{
	RECT CurrentWindowPosition;
	GetWindowRect(PlatformWindow.Window, &CurrentWindowPosition);
	SetWindowPos(PlatformWindow.Window, HWND_TOP,
		CurrentWindowPosition.left, CurrentWindowPosition.top,
		Width, Height, SWP_SHOWWINDOW);
}

void* 
Platform_AllocateMemory(unsigned long long *Size)
{
	_SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	uint64 NumberOfPages = *Size / SystemInfo.dwPageSize;
	*Size = NumberOfPages * SystemInfo.dwPageSize;
	void* MemoryPtr = VirtualAlloc(0, *Size,
		MEM_COMMIT, PAGE_READWRITE);
	
	return MemoryPtr;
}

void
Platform_DeallocateMemory(void* MemoryPtr, unsigned long long *Size)
{
	VirtualFree(MemoryPtr, *Size, MEM_RELEASE);
}

void
Platform_TemporaryError(char* Text)
{
	// TODO: Remove this when engine error system is complete
	// NOTE: Beware of buffer overflow potential.
	char Buffer[256];
	wsprintf(Buffer, Text);
	OutputDebugStringA(Buffer);
}

int8 
Platform_DoesFileExist(char* FileName)
{
	struct stat buffer;
	if (stat(FileName, &buffer) == 0)
	{
		return 1;
	}
	else
	{
		// TODO: ERROR
		return 0; 
	}
}

unsigned char*
Platform_ReadFile(char* FileName)
{
	HANDLE ImageFile;
	ImageFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD FileSize = GetFileSize(ImageFile, 0);
#if MEMORY_ON
	uint8* Buffer = 0;
	Buffer = Memory_Allocate(Buffer, FileSize);
#else
	uint8* Buffer = new uint8[FileSize] {};
#endif
	LPDWORD BytesRead = 0;
	ReadFile(ImageFile, (LPVOID)Buffer, FileSize, BytesRead, 0);
	CloseHandle(ImageFile);
	return Buffer;
}

void 
Platform_SetupFont(char* FileName, char* FontName, 
	void** Bits, void** DeviceContextPtr)
{
	if (!*DeviceContextPtr)
	{
		//TODO: Release this DC later
		*DeviceContextPtr = 
			(void*)CreateCompatibleDC(GetDC(PlatformWindow.Window));
	}

	BITMAPINFO BitmapInfo = {};
	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = 1024;
	BitmapInfo.bmiHeader.biHeight = 1024;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	// NOTE: Returns bottom up space. ptr at top
	HBITMAP Bitmap = CreateDIBSection((HDC)(*DeviceContextPtr),
		&BitmapInfo, DIB_RGB_COLORS, Bits, 0, 0);
	SelectObject((HDC)(*DeviceContextPtr), Bitmap);
	SetBkColor((HDC)(*DeviceContextPtr), RGB(0, 0, 0));

	////////////////////////////////////////////
	AddFontResourceExA(FileName, FR_PRIVATE, 0);

	HFONT Font = CreateFontA(128, 0, 0, 0, FW_NORMAL,
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, FontName);

	SelectObject((HDC)(*DeviceContextPtr), Font);

	TEXTMETRIC TextMetric;
	GetTextMetrics((HDC)(*DeviceContextPtr), &TextMetric);
}

void 
Platform_LoadGlyph(void* Bits, uint16 Glyph,
	Texture2D *Texture, void* DeviceContext)
{
	int BitmapWidth = 1024;
	int BitmapHeight = 1024;

	SIZE Size;
	if (!GetTextExtentPoint32W((HDC)DeviceContext,
		(LPCWSTR)&Glyph, 1, &Size))
	{
		// TODO: Error
	}

	int Width = Size.cx;
	if (Width > BitmapWidth)
	{
		Width = BitmapWidth;
	}
	int Height = Size.cy;
	if (Height > BitmapHeight)
	{
		Height = BitmapHeight;
	}

	if (SetTextColor((HDC)DeviceContext, RGB(255, 255, 255)) ==
		CLR_INVALID)
	{
		// TODO: Error
	}

	if (!TextOutW((HDC)DeviceContext, 0, 0, (LPCWSTR)&Glyph, 1))
	{
		// TODO: Error
	}

	Texture->Width = Width;
	Texture->Height = Height;
	unsigned int Pitch = Texture->Width * 4;
	
	// NOTE: Dynamic allocation
#if MEMORY_ON
	Texture->Data = 0;
	Texture->Data = Memory_Allocate(Texture->Data, Texture->Height* Pitch);
#else
	Texture->Data = new uint8[Texture->Height* Pitch] {};
#endif
	uint8 *DestRow = (uint8 *)Texture->Data +
		(Texture->Height - 1) * Pitch;
	uint32 *SourceRow = (uint32 *)Bits + (BitmapHeight - 1) *
		BitmapWidth;
	for (int32 Y = 0; Y < Height; ++Y)
	{
		uint32 *Source = (uint32 *)SourceRow;
		uint32 *Dest = (uint32 *)DestRow;
		for (int32 X = 0; X < Width; ++X)
		{
			//uint32 Pixel = *Source;
	
			uint8 Alpha = (uint8)(*Source & 0xFF);
			*Dest++ = ((Alpha << 24) |
				(Alpha << 16) |
				(Alpha << 8) |
				(Alpha << 0));
			++Source;
		}
	
		DestRow -= Pitch;
		SourceRow -= BitmapWidth;
	}
}

int64
Platform_GetCPUCounter()
{
	LARGE_INTEGER Time;
#if DEBUG_MODE
	if (!QueryPerformanceCounter(&Time))
	{
	// TODO: Error
		MessageBox(0, "QueryPerformanceCounter Failed!", 
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
#else
	QueryPerformanceCounter(&Time);
#endif
	return Time.QuadPart;
}

int64 
Platform_GetTimeFrequency()
{
	if (PlatformProperties.TimerFrequency == 0)
	{
		LARGE_INTEGER Frequency;
#if DEBUG_MODE
		if (!QueryPerformanceFrequency(&Frequency))
		{
			// TODO: Error
			MessageBox(0, "Obtaining Timer Frequency Failed!", 
				"Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
#else
		QueryPerformanceFrequency(&Frequency);
#endif
		return Frequency.QuadPart;
	}
	else
	{
		return PlatformProperties.TimerFrequency;
	}
} 

void 
Platform_GetCursorPosition(float *X, float *Y)
{
	POINT currentCursorPoint;

	if (GetCursorPos(&currentCursorPoint))
	{
		ScreenToClient(PlatformWindow.Window,
			&currentCursorPoint);

		*X = (float)currentCursorPoint.x;
		*Y = (float)currentCursorPoint.y;
	}
	else
	{
		// TODO: Error Checking
	}
}

int8 
Platform_GetStateOfKey(int key)
{
	int8 result = 0;

	if (GetAsyncKeyState(key) & 0x8000)
	{
		result = 1;
		return result;
	}
	else
	{
		return result;
	}
}

void 
Platform_UpdateMouseState(uint32 State)
{
	MouseState = State;
}

uint32
Platform_GetMouseState()
{
	uint32 ReturnState = MouseState;
	return MouseState;
}

void 
Platform_EndProgram()
{
	PostQuitMessage(0);
}

void 
Platform_Initialize(window_properties *WindowProps)
{
	PlatformProperties.Instance = GetModuleHandle(0);
#if DEBUG_MODE
	if (PlatformProperties.Instance == 0)
	{
		MessageBox(0, "Obtaining Window's Instance Failed!", 
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
#endif

	WNDCLASSEX WindowsClassStructure;
	WindowsClassStructure.cbSize = sizeof(WNDCLASSEX);
	WindowsClassStructure.style = CS_OWNDC;
	WindowsClassStructure.lpfnWndProc = WndProc;
	WindowsClassStructure.cbClsExtra = 0;
	WindowsClassStructure.cbWndExtra = 0;
	WindowsClassStructure.hInstance = PlatformProperties.Instance;
	WindowsClassStructure.hIcon = LoadIcon(0, IDI_APPLICATION);
	WindowsClassStructure.hCursor = LoadCursor(0, IDC_ARROW);
	WindowsClassStructure.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	WindowsClassStructure.lpszMenuName = 0;
	WindowsClassStructure.lpszClassName = "GameEngine";
	WindowsClassStructure.hIconSm = LoadIcon(0, IDI_APPLICATION);

#if DEBUG_MODE
	if (!RegisterClassEx(&WindowsClassStructure))
	{
		MessageBox(0, "Window Registration Failed!", 
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
#else
	RegisterClassEx(&WindowsClassStructure);
#endif

	PlatformWindow.Window = CreateWindowEx(
		WS_EX_CLIENTEDGE, "GameEngine",
		"First Generation Game Engine",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, WindowProps->Width,
		WindowProps->Height, 0, 0, PlatformProperties.Instance, 0);

#if DEBUG_MODE
	if (PlatformWindow.Window == 0)
	{
		MessageBox(0, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
#endif

	PlatformProperties.DeviceContext = GetDC(PlatformWindow.Window);

#if DEBUG_MODE
	if (PlatformProperties.DeviceContext == 0)
	{
		MessageBox(0, "Obtaining Device Context Failed!", 
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
#endif

	UpdateWindow(PlatformWindow.Window);

	// TODO: Pull this out into a separate timer init
	PlatformProperties.TimerFrequency = 
		Platform_GetTimeFrequency();
}

void 
Platform_ShowWindow(int CommandShow)
{
	ShowWindow(PlatformWindow.Window, CommandShow);
}

unsigned int
Platform_MessageLoop()
{
	MSG Message = {};

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		Game_Loop();

#if DEBUG_MODE
		if (!SwapBuffers(PlatformProperties.DeviceContext))
		{
			MessageBox(0, "Swapping Buffers Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
#else
		SwapBuffers(PlatformProperties.DeviceContext);
#endif
	}
	
	return Message.message;
}

void 
Platform_InitRenderer()
{
	PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
	DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
	DesiredPixelFormat.nVersion = 1;
	DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | 
		PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
	DesiredPixelFormat.cColorBits = 32;
	DesiredPixelFormat.cAlphaBits = 8;
	DesiredPixelFormat.cDepthBits = 16;
	DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

	int SuggestedPixelFormatIndex =
		ChoosePixelFormat(PlatformProperties.DeviceContext,
			&DesiredPixelFormat);
#if DEBUG_MODE
	if (SuggestedPixelFormatIndex == 0)
	{
		MessageBox(0, 
			"Obtaining Suggested Pixel Format Index Failed!", 
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
#endif

	PIXELFORMATDESCRIPTOR SuggestedPixelFormat;

#if DEBUG_MODE
	if (!DescribePixelFormat(PlatformProperties.DeviceContext,
		SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat),
		&SuggestedPixelFormat))
	{
		MessageBox(0, 
			"Obtaining Description of a Pixel Format Failed!",
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	if (!SetPixelFormat(PlatformProperties.DeviceContext,
		SuggestedPixelFormatIndex, &SuggestedPixelFormat))
	{
		MessageBox(0, "Setting Pixel Format Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
#else
	DescribePixelFormat(PlatformProperties.DeviceContext,
		SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat),
		&SuggestedPixelFormat);
	SetPixelFormat(PlatformProperties.DeviceContext,
		SuggestedPixelFormatIndex, &SuggestedPixelFormat);
#endif

	 HGLRC OpenGLContext = 
		wglCreateContext(PlatformProperties.DeviceContext);
#if DEBUG_MODE
	if (OpenGLContext == 0)
	{
		MessageBox(0, "Obtaining OpenGL Render Context Failed!",
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
#endif

	//TODO: Assumes OpenGL right now. Generalize to other APIs.
#if DEBUG_MODE
	if (wglMakeCurrent(PlatformProperties.DeviceContext,
		OpenGLContext))
	{
		wgl_create_context_attribs_arb *wglCreateContextAttribsARB =
			(wgl_create_context_attribs_arb *)wglGetProcAddress(
				"wglCreateContextAttribsARB");

		if (wglCreateContextAttribsARB)
		{
			int Attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_FLAGS_ARB, 
				// TODO: Remove Debug for final
				WGL_CONTEXT_DEBUG_BIT_ARB, 
				WGL_CONTEXT_PROFILE_MASK_ARB,
				WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
				0
			};
			HGLRC ShareContext = 0; 
			HGLRC ModernContext =
				wglCreateContextAttribsARB(
					PlatformProperties.DeviceContext,
					ShareContext, Attribs);

			if (ModernContext)
			{
				if (wglMakeCurrent(PlatformProperties.DeviceContext,
					ModernContext))
				{
					wglDeleteContext(OpenGLContext);
					OpenGLContext = ModernContext;
				}
			}
		}
		else
		{
			MessageBox(0, "wglMakeCurrent Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
	}
	else
	{
		// TODO: Error
	}
#else
	wglMakeCurrent(PlatformProperties.DeviceContext,
		OpenGLContext);

	wgl_create_context_attribs_arb *wglCreateContextAttribsARB =
		(wgl_create_context_attribs_arb *)wglGetProcAddress(
			"wglCreateContextAttribsARB");

	int Attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB,
		//WGL_CONTEXT_DEBUG_BIT_ARB, ?
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0
	};
	HGLRC ShareContext = 0;
	HGLRC ModernContext =
		wglCreateContextAttribsARB(
			PlatformProperties.DeviceContext,
			ShareContext, Attribs);

	if (wglMakeCurrent(PlatformProperties.DeviceContext,
		ModernContext))
	{
		wglDeleteContext(OpenGLContext);
		OpenGLContext = ModernContext;
	}
#endif
}

void
Platform_ReleaseContext(void* DeviceContext)
{
	if (DeviceContext)
	{
		ReleaseDC(PlatformWindow.Window, (HDC)DeviceContext);
	}
}

void 
Platform_Cleanup()
{
	wglMakeCurrent(0, 0);
	ReleaseDC(PlatformWindow.Window,
		PlatformProperties.DeviceContext);
}

LRESULT CALLBACK 
WndProc(HWND Window, UINT Message,
	WPARAM wParam, LPARAM lParam)
{

	LRESULT Result = Game_MessageProcessor(Window, Message,
		(uint64)wParam, (int64)lParam);

	switch (Message)
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		RECT ClientRect;
		GetClientRect(Window, &ClientRect);
		WindowProperties.Width = ClientRect.right - ClientRect.left;
		WindowProperties.Height = ClientRect.bottom - ClientRect.top;
		Render_UpdateWindow(WindowProperties);
	} break;
	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		HDC DeviceContext = BeginPaint(Window, &paint);

		RECT ClientRect;
		GetClientRect(Window, &ClientRect);
		WindowProperties.Width = ClientRect.right - ClientRect.left;
		WindowProperties.Height = ClientRect.bottom - ClientRect.top;

		EndPaint(Window, &paint);
		Render_UpdateWindow(WindowProperties);
	}	break;
	case WM_CHAR:
	{
	} break;
	case WM_LBUTTONUP:
	{
		if (MouseLeftClickToggle == 1)
		{
			Platform_UpdateMouseState(1);
		} 
		else
		{
			Platform_UpdateMouseState(0);
		}
		MouseLeftClickToggle = 0;
	} break;
	case WM_LBUTTONDOWN:
	{
		MouseLeftClickToggle = 1;
	} break;
	default:
		return DefWindowProc(Window, Message,
			wParam, lParam);
	}

	return Result;
}
