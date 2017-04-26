#include "game_layer.h"
#include <Windows.h>

#include <sys/types.h>
#include <sys/stat.h>

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, 
	LPARAM lParam);

// TEMPORARY: Throws these in a struct later? Or get rid of them
uint32 MouseState;
uint8 MouseLeftClickToggle = 0;

void Platform_UpdateWindowSize(uint32 Width, uint32 Height)
{
	RECT CurrentWindowPosition;
	GetWindowRect(PlatformWindow.Window, &CurrentWindowPosition);
	SetWindowPos(PlatformWindow.Window, HWND_TOP,
		CurrentWindowPosition.left, CurrentWindowPosition.top,
		Width, Height, SWP_SHOWWINDOW);
}

void Platform_TemporaryError(char* Text)
{
	// TODO: Remove this when engine error system is complete
	// NOTE: Beware of buffer overflow potential.
	char Buffer[256];
	wsprintf(Buffer, Text);
	OutputDebugStringA(Buffer);
}

int8 Platform_DoesFileExist(char* FileName)
{
	struct stat buffer;
	if (stat(FileName, &buffer) == 0)
	{
		return 1;
	}
	else
	{
		return 0; // TODO: ERROR
	}
}

unsigned char* Platform_ReadFile(char* FileName)
{
	struct stat buffer;
	stat(FileName, &buffer);

	uint8* ReturnValues = new uint8[buffer.st_size]();

	FILE* OpenFile;
	// NOTE: rb = Read Binary
	fopen_s(&OpenFile, FileName, "rb");

	if (OpenFile != 0)
	{
		size_t NumberOfElementsRead = fread(ReturnValues, 
			sizeof(uint8), buffer.st_size, OpenFile);

		fclose(OpenFile);
	}
	else
	{
		// TODO: Error
		Platform_TemporaryError("Failed to open file");
		return 0; 
	}

	return ReturnValues;
}

void Platform_SetupFont(char* FileName, char* FontName, 
	void** Bits, void** DeviceContextPtr)
{
	if (!*DeviceContextPtr)
	{
		//TODO: Release this DC later
		*DeviceContextPtr = (void*)CreateCompatibleDC(GetDC(PlatformWindow.Window));
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

void Platform_LoadGlyph(void* Bits, uint16 Glyph, Texture2D *Texture,
	void* DeviceContext)
{
	int BitmapWidth = 1024;
	int BitmapHeight = 1024;

	SIZE Size;
	if (!GetTextExtentPoint32W((HDC)DeviceContext, (LPCWSTR)&Glyph, 
		1, &Size))
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
	Texture->Data = new uint8[Texture->Height* Pitch] {};

	uint8 *DestRow = (uint8 *)Texture->Data + (Texture->Height - 1) *
		Pitch;
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

int64 Platform_GetCPUCounter()
{
	LARGE_INTEGER Time;
	if (!QueryPerformanceCounter(&Time))
	{
	// TODO: Error
		MessageBox(0, "QueryPerformanceCounter Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
	return Time.QuadPart;
}

int64 Platform_GetTimeFrequency()
{
	if (PlatformProperties.TimerFrequency == 0)
	{
		LARGE_INTEGER Frequency;
		if (!QueryPerformanceFrequency(&Frequency))
		{
			// TODO: Error
			MessageBox(0, "Obtaining Timer Frequency Failed!", 
				"Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
		return Frequency.QuadPart;
	}
	else
	{
		return PlatformProperties.TimerFrequency;
	}
} 

void Platform_GetCursorPosition(float *X, float *Y)
{
	POINT currentCursorPoint;

	if (GetCursorPos(&currentCursorPoint))
	{
		ScreenToClient(PlatformWindow.Window, &currentCursorPoint);

		*X = (float)currentCursorPoint.x;
		*Y = (float)currentCursorPoint.y;
	}
	else
	{
		// TODO: Error Checking
	}
}

int8 Platform_GetStateOfKey(int key)
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

void Platform_UpdateMouseState(uint32 State)
{
	MouseState = State;
}

uint32 Platform_GetMouseState()
{
	uint32 ReturnState = MouseState;
	return MouseState;
}

void Platform_EndProgram()
{
	PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int CommandShow)
{
	return Game_Main(CommandShow);
}

void Platform_Initialize(window_properties *WindowProps)
{
	PlatformProperties.Instance = GetModuleHandle(0);
	if (PlatformProperties.Instance == 0)
	{
		MessageBox(0, "Obtaining Window's Instance Failed!", 
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

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
	WindowsClassStructure.lpszClassName = (const char*)"GameEngine";
	WindowsClassStructure.hIconSm = LoadIcon(0, IDI_APPLICATION);

	if (!RegisterClassEx(&WindowsClassStructure))
	{
		MessageBox(0, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	PlatformWindow.Window = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		(const char*)"GameEngine",
		"First Generation Game Engine",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, WindowProps->Width, WindowProps->Height,
		0, 0, PlatformProperties.Instance, 0);

	if (PlatformWindow.Window == 0)
	{
		MessageBox(0, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	PlatformProperties.DeviceContext = GetDC(PlatformWindow.Window);

	if (PlatformProperties.DeviceContext == 0)
	{
		MessageBox(0, "Obtaining Device Context Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	UpdateWindow(PlatformWindow.Window);

	// TODO: Pull this out into a separate timer init
	PlatformProperties.TimerFrequency = Platform_GetTimeFrequency();
}

void Platform_ShowWindow(int CommandShow)
{
	ShowWindow(PlatformWindow.Window, CommandShow);
}

unsigned int Platform_MessageLoop()
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

		if (!SwapBuffers(PlatformProperties.DeviceContext))
		{
			MessageBox(0, "Swapping Buffers Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
	}
	
	return Message.message;
}

void Platform_InitRenderer()
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
	if (SuggestedPixelFormatIndex == 0)
	{
		MessageBox(0, "Obtaining Suggested Pixel Format Index Failed!", 
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
	if (!DescribePixelFormat(PlatformProperties.DeviceContext,
		SuggestedPixelFormatIndex,
		sizeof(SuggestedPixelFormat), &SuggestedPixelFormat))
	{
		MessageBox(0, 
			"Obtaining Description of a Possible Pixel Format Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	if (!SetPixelFormat(PlatformProperties.DeviceContext,
		SuggestedPixelFormatIndex, &SuggestedPixelFormat))
	{
		MessageBox(0, "Setting Pixel Format Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	HGLRC OpenGLContext = wglCreateContext(PlatformProperties.DeviceContext);
	if (OpenGLContext == 0)
	{
		MessageBox(0, "Obtaining OpenGL Render Context Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	//TODO: Assumes OpenGL right now. Make more generalized for DirectX also
	if (wglMakeCurrent(PlatformProperties.DeviceContext, OpenGLContext))
	{
		//MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
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
				WGL_CONTEXT_DEBUG_BIT_ARB, // TODO: Remove Debug for final
				WGL_CONTEXT_PROFILE_MASK_ARB,
				WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
				0
			};
			HGLRC ShareContext = 0; 
			HGLRC ModernContext =
				wglCreateContextAttribsARB(PlatformProperties.DeviceContext,
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
}

void Platform_ReleaseContext(void* DeviceContext)
{
	if (DeviceContext)
	{
		ReleaseDC(PlatformWindow.Window, (HDC)DeviceContext);
	}
}

void Platform_Cleanup()
{
	wglMakeCurrent(0, 0);
	ReleaseDC(PlatformWindow.Window,
		PlatformProperties.DeviceContext);
}

LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
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
		Text_SendToGlobalSystem((char)wParam);
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			PostQuitMessage(0);
		} break;
		default:
			break;
		}
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
		return DefWindowProc(Window, Message, wParam, lParam);
	}

	return 0;
}
