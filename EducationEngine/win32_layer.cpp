#include "game_loop.h"
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
		return 0; // // TODO: ERROR
	}
}

unsigned char* Platform_ReadFile(char* FileName)
{
	struct stat buffer;
	stat(FileName, &buffer);

	uint8* ReturnValues = new uint8[buffer.st_size]();

	FILE* OpenFile;
	fopen_s(&OpenFile, FileName, "rb"); // rb = Read Binary

	if (OpenFile != 0)
	{
		size_t NumberOfElementsRead = fread(ReturnValues, 
			sizeof(uint8), buffer.st_size, OpenFile);

		fclose(OpenFile);
	}
	else
	{
		Platform_TemporaryError("Failed to open file");
		return NULL; // FAILED TO OPEN
	}

	return ReturnValues;
}

static HDC TestContext = 0; // // TODO: REMOVE

void Platform_SetupFont(char* FileName, char* FontName, void** Bits)
{
	int MaxWidth = 1024;
	int MaxHeight = 1024;

	AddFontResourceExA(FileName, FR_PRIVATE, 0);
	int Height = 128;
	HFONT Font = CreateFontA(Height, 0, 0, 0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		FontName);

	TestContext = CreateCompatibleDC(GetDC(0));

	BITMAPINFO Info = {};
	Info.bmiHeader.biSize = sizeof(Info.bmiHeader);
	Info.bmiHeader.biWidth = MaxWidth;
	Info.bmiHeader.biHeight = MaxHeight;
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = 32;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = 0;
	Info.bmiHeader.biXPelsPerMeter = 0;
	Info.bmiHeader.biYPelsPerMeter = 0;
	Info.bmiHeader.biClrUsed = 0;
	Info.bmiHeader.biClrImportant = 0;

	HBITMAP Bitmap = CreateDIBSection(TestContext,
		&Info, DIB_RGB_COLORS, Bits, 0, 0); // Returns bottom up space. ptr at top

	SelectObject(TestContext, Bitmap); // TODO: Need both?
	SelectObject(TestContext, Font);
	SetBkColor(TestContext, RGB(0, 0, 0)); // Glyph background color

	TEXTMETRIC TextMetric;
	GetTextMetrics(TestContext, &TextMetric);
}

loaded_bitmap Platform_LoadGlyph(void* Bits, uint32 CodePoint)
{
	loaded_bitmap Result = {};

	int MaxWidth = 1024;
	int MaxHeight = 1024;

	wchar_t ConvertToChar = (wchar_t)CodePoint;

	SIZE Size;
	GetTextExtentPoint32W(TestContext, &ConvertToChar, 1, &Size);

	int Width = Size.cx;
	if (Width > MaxWidth)
	{
		Width = MaxWidth;
	}
	int Height = Size.cy;
	if (Height > MaxHeight)
	{
		Height = MaxHeight;
	}

	SetTextColor(TestContext, RGB(255, 255, 255));
	TextOutW(TestContext, 0, 0, &ConvertToChar, 1);

	int32 MinX = 10000;
	int32 MinY = 10000;
	int32 MaxX = -10000;
	int32 MaxY = -10000;
	uint32 *Row = (uint32 *)Bits + (MaxHeight - 1) * MaxWidth;
	for (int32 Y = 0; Y < Height; ++Y)
	{
		uint32 *Pixel = Row;
		for (int32 X = 0; X < Width; ++X)
		{
			if (Pixel != 0)
			{
				if (MinX > X)
				{
					MinX = X;
				}

				if (MinY > Y)
				{
					MinY = Y;
				}

				if (MaxX < X)
				{
					MaxX = X;
				}

				if (MaxY < Y)
				{
					MaxY = Y;
				}
			}
			++Pixel;
		}
		Row -= MaxWidth;
	}

	if (MinX <= MaxX)
	{
		Width = (MaxX - MinX) + 1;
		Height = (MaxY - MinY) + 1;

		Result.Width = Width;
		Result.Height = Height;
		Result.Pitch = Result.Width* 4; //Note: 4 is bytes per pixel
		Result.Memory = malloc(Result.Height*Result.Pitch);
		Result.Free = Result.Memory;

		uint8 *DestRow = (uint8 *)Result.Memory + (Result.Height - 1) * 
			Result.Pitch;
		uint32 *SourceRow = (uint32 *)Bits + (MaxHeight - 1 - MinY) * 
			MaxWidth;
		for (int32 Y = MinY; Y <= MaxY; ++Y)
		{
			uint32 *Source = (uint32 *)SourceRow + MinX;
			uint32 *Dest = (uint32 *)DestRow;
			for (int32 X = MinX; X <= MaxX; ++X)
			{
				uint32 Pixel = *Source;

				uint8 Gray = (uint8)(Pixel & 0xFF);
				uint8 Alpha = Gray;
				*Dest++ = ((Alpha << 24) |
					(Gray << 16) |
					(Gray << 8) |
					(Gray << 0));
				++Source;
			}

			DestRow -= Result.Pitch;
			SourceRow -= MaxWidth;
		}
	}

	return Result;
}

int64 Platform_GetCPUCounter()
{
	LARGE_INTEGER Time;
	if (!QueryPerformanceCounter(&Time))
	{
	// TODO: Error
		MessageBox(NULL, "QueryPerformanceCounter Failed!", "Error!",
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
			MessageBox(NULL, "Obtaining Timer Frequency Failed!", 
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
	WindowProperties.Width = 1200;
	WindowProperties.Height = 700;
	
	Platform_Initialize();
	Platform_InitRenderer();

	Render_Init(WindowProperties);
	ShowWindow(PlatformWindow.Window, CommandShow);

	MSG Message = {};

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		Game_Loop();

		if (!SwapBuffers(PlatformProperties.DeviceContext))
		{
			MessageBox(NULL, "Swapping Buffers Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
	}

	Asset_DeleteAll(); //TODO: Temporary, remove this later.
	Platform_Cleanup();

	return Message.message;
}

void Platform_Initialize()
{
	PlatformProperties.Instance = GetModuleHandle(NULL);
	if (PlatformProperties.Instance == NULL)
	{
		MessageBox(NULL, "Obtaining Window's Instance Failed!", 
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
	WindowsClassStructure.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WindowsClassStructure.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowsClassStructure.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	WindowsClassStructure.lpszMenuName = NULL;
	WindowsClassStructure.lpszClassName = (const char*)"GameEngine";
	WindowsClassStructure.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&WindowsClassStructure))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	PlatformWindow.Window = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		(const char*)"GameEngine",
		"First Generation Game Engine",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, WindowProperties.Width,
		WindowProperties.Height,
		NULL, NULL, PlatformProperties.Instance, NULL);

	if (PlatformWindow.Window == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	PlatformProperties.DeviceContext = GetDC(PlatformWindow.Window);

	if (PlatformProperties.DeviceContext == NULL)
	{
		MessageBox(NULL, "Obtaining Device Context Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	UpdateWindow(PlatformWindow.Window);
	PlatformProperties.TimerFrequency = Platform_GetTimeFrequency();
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
		MessageBox(NULL, "Obtaining Suggested Pixel Format Index Failed!", 
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
	if (!DescribePixelFormat(PlatformProperties.DeviceContext,
		SuggestedPixelFormatIndex,
		sizeof(SuggestedPixelFormat), &SuggestedPixelFormat))
	{
		MessageBox(NULL, 
			"Obtaining Description of a Possible Pixel Format Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	if (!SetPixelFormat(PlatformProperties.DeviceContext,
		SuggestedPixelFormatIndex, &SuggestedPixelFormat))
	{
		MessageBox(NULL, "Setting Pixel Format Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	HGLRC OpenGLContext = wglCreateContext(PlatformProperties.DeviceContext);
	if (OpenGLContext == NULL)
	{
		MessageBox(NULL, "Obtaining OpenGL Render Context Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	//TODO: Assumes OpenGL right now. Make more generalized.
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
				WGL_CONTEXT_FLAGS_ARB, //WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB |
				WGL_CONTEXT_DEBUG_BIT_ARB, // REMOVE DEBUG ON PERFORMANCE
				WGL_CONTEXT_PROFILE_MASK_ARB,
				WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
				0
			};
			HGLRC ShareContext = 0; // a second context for later.
			HGLRC ModernContext =
				wglCreateContextAttribsARB(PlatformProperties.DeviceContext,
					ShareContext, Attribs);

			if (ModernContext)
			{
				if (wglMakeCurrent(PlatformProperties.DeviceContext,
					ModernContext))
				{
					//	ModernContext = true;
					wglDeleteContext(OpenGLContext);
					OpenGLContext = ModernContext;
				}
			}
		}
		else
		{
			MessageBox(NULL, "wglMakeCurrent Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
	}
	else
	{
		// ERROR?
	}
}

void Platform_Cleanup()
{
	wglMakeCurrent(NULL, NULL);
	//wglDeleteContext(RenderContext);
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
