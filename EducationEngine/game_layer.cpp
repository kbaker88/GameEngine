#include "game_layer.h"

// TODO: Temporary. For testing purposes.
static uint8 StateOfProgram = 0;
ProgramState States[3]{};

#if MODULE_MODE
typedef void Module_State(ProgramState* State);

struct ModuleCalls
{
	Module_State* Load;
	Module_State* Run;
	Module_State* Clean;
};

ModuleCalls Modules[3];
uint32 CurrentModuleID;
#endif
// TODO: Temporary placement, put into state system
Font *GlobalFont;

uint32
Game_Main(int32 CommandShow)
{
	window_properties WindowDimensions;
	WindowDimensions.Width = 1200;
	WindowDimensions.Height = 700;

	Platform_Initialize(&WindowDimensions);
	Platform_InitRenderer();
	Render_Init(&WindowDimensions);
	Platform_ShowWindow(CommandShow);

#if MODULE_MODE
#if MEMORY_ON
	Memory_Initialize(1);
#endif // MEMORY_ON
	DEBUG_Initialize();

	Modules[0].Load = (Module_State*)Title_Initialize;
	Modules[0].Run = (Module_State*)Title_Draw;
	Modules[0].Clean = (Module_State*)Title_Clean;

	Modules[1].Load = (Module_State*)Game_Initialize;
	Modules[1].Run = (Module_State*)Game_Draw;
	Modules[1].Clean = (Module_State*)Game_Clean;

	//Network_Init();
	StateOfProgram = 0;
	CurrentModuleID = 0;
	States[0].StateOfProgram = &StateOfProgram;
	States[1].StateOfProgram = &StateOfProgram;
	States[2].StateOfProgram = &StateOfProgram;
#endif // MODULE_MODE

	// NOTE: Platform_MessageLoop() also calls Game_Loop() internally
	uint32 Message = Platform_MessageLoop();

	// TODO: Temporary, remove this later.
	Asset_DeleteAll(); 
	Text_DeleteFont(GlobalFont); 

	Platform_Cleanup();
	return Message;
}

void
Game_Loop()
{
	int64 LastCounter = Platform_GetCPUCounter();

	CurrentModuleID = (uint32)*States[StateOfProgram].StateOfProgram;
#if MODULE_MODE
	if (States[CurrentModuleID].Status == 1)
	{
		Modules[CurrentModuleID].Run(&States[CurrentModuleID]);
	}
	else if (States[CurrentModuleID].Status == 0)
	{
		Modules[CurrentModuleID].Load(&States[CurrentModuleID]);
	}
	else
	{
		Modules[CurrentModuleID].Clean(&States[CurrentModuleID]);
	}

#else
	switch (StateOfProgram)
	{
	// NOTE: Loading State
	case 0: 
	{
#if MEMORY_ON
		Memory_Initialize(1);
#endif // MEMORY_ON
		DEBUG_Initialize();

		Asset_LoadBMP("Images/startbutton.bmp"); // 0
		Asset_LoadBMP("Images/menubutton.bmp"); // 1
		Asset_LoadBMP("Images/exitbutton.bmp"); // 2
		Asset_LoadBMP("Images/titlebutton.bmp"); // 3
		Asset_LoadBMP("Images/grass2.bmp"); // 4
		Asset_LoadBMP("Images/container.bmp"); // 5
		Asset_LoadBMP("Images/woodfloor.bmp"); // 6
		Asset_LoadPNG("Images/StartHeightMap.png"); // 7
		Asset_LoadBMP("Images/inputbar.bmp"); // 8

		if (!GlobalFont)
		{
#if MEMORY_ON
			GlobalFont = 0;
			GlobalFont = (Font*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(Font));
#else
			GlobalFont = new Font;
#endif // MEMORY_ON
		}
		Text_BuildFont("arial\0", GlobalFont);
		//Network_Init();
		StateOfProgram = 1;
	} break;

	// NOTE: Title Screen
	case 1: 
	{
		if (States[0].Status == 0)
		{
			States[0].NumRenderObjBlocks = 2;
			States[0].NumModelObjBlocks = 2;

			State_CreateRenderObjectBlocks(&States[0],
				States[0].NumRenderObjBlocks, 256);
			State_CreateModelObjectBlocks(&States[0],
				States[0].NumModelObjBlocks, 256);
			State_CreateCollisionObjects(&States[0],
				12);
			State_CreateTextObjs(&States[0],
				TEXT_OBJECTS_PER_PROGSTATE);
			State_CreateCameras(&States[0], 1);
			State_CreateShaderVariables(&States[0], 5);
			State_CreateShaderHandles(&States[0], 2);
			State_CreateTimers(&States[0], 2);
			State_LinkToProgram(&States[0], &StateOfProgram);
			
			States[0].FontArr = GlobalFont;
			States[0].FontCount = 1;
			States[0].Status = 1;

			//Title_Initialize(&States[0]);
			TestCalls[0].Init(&States[0]);
		}
		else
		{
			Title_Draw(&States[0]);
			// NOTE: Right Arrow, replace later.
			if (Platform_GetStateOfKey(0x27) == 1) 
			{
				StateOfProgram = 2;
				Title_Clean(&States[0]);
			}
		}

		if ((StateOfProgram != 1) || (States[0].Status == -1))
		{
			State_Clean(&States[0]);
		}
	} break;

	// NOTE: Game State
	case 2:
	{
		if (States[1].Status == 0)
		{
			State_CreateRenderObjectBlocks(&States[1],
				2, 256);
			State_CreateModelObjectBlocks(&States[1],
				2, 256);
			State_CreateCollisionObjects(&States[1],
				12);
			State_CreatePhysicsObjects(&States[1],
				12);
			State_CreateTextObjs(&States[1],
				TEXT_OBJECTS_PER_PROGSTATE);
			State_CreateCameras(&States[1], 1);
			State_CreateTimers(&States[1], 1);
			State_CreateShaderVariables(&States[1], 7);
			State_CreateShaderHandles(&States[1], 2);
			State_LinkToProgram(&States[1], &StateOfProgram);
			
			States[1].FontArr = GlobalFont;
			States[1].FontCount = 1;

			States[1].Status = 1;
			Game_Initialize(&States[1]);
		}
		else
		{
			Game_Draw(&States[1]);
			// NOTE: Left Arrow, replace later.
			if (Platform_GetStateOfKey(0x25) == 1) 
			{
				StateOfProgram = 1;
				Game_Clean(&States[1]);
			}
		}

		if ((StateOfProgram != 2) || (States[1].Status == -1))
		{
			State_Clean(&States[1]);
		}
	} break;

	// NOTE: Menu State
	case 3: 
	{
		if (States[2].Status == 0)
		{
			State_CreateRenderObjectBlocks(&States[2],
				2, 256);
			State_CreateModelObjectBlocks(&States[2],
				2, 256);
			State_CreateCollisionObjects(&States[2],
				12);
			State_CreateTextObjs(&States[2],
				TEXT_OBJECTS_PER_PROGSTATE);
			State_CreateCameras(&States[2], 1);
			State_CreateShaderVariables(&States[2], 5);
			State_CreateShaderHandles(&States[2], 2);
			State_LinkToProgram(&States[2], &StateOfProgram);

			States[2].FontArr = GlobalFont;
			States[2].FontCount = 1;

			States[2].Status = 1;
			Menu_Initialize(&States[2]);
		}
		else
		{
			Menu_Draw(&States[2]);
			// NOTE: Left Arrow, replace later.
			if (Platform_GetStateOfKey(0x25) == 1) 
			{
				StateOfProgram = 1;
				Menu_Clean(&States[2]);
			}
		}

		if ((StateOfProgram != 3) || (States[2].Status == -1))
		{
			State_Clean(&States[2]);
		}
	} break;

	case 4:
	{
		Platform_EndProgram();
#if MEMORY_ON
		Memory_Clean();
#endif // MEMORY_ON
	} break;

	default:
	{

	} break;
	}
#endif // MODULE_MODE

	// NOTE: Cap at 60FPS
	int64 CounterElapsed = Platform_GetCPUCounter() - LastCounter;
	float SecondsElapsedForFrame = ((float)CounterElapsed /
		(float)Platform_GetTimeFrequency());
	while (SecondsElapsedForFrame < SecondsPerFrame)
	{
		SecondsElapsedForFrame = ((float)(Platform_GetCPUCounter() -
			LastCounter) / (float)Platform_GetTimeFrequency());
	} 
	
	LastCounter = Platform_GetCPUCounter();
}

int64
Game_MessageProcessor(void* Window, uint32 Message,
	uint64 wParam, int64 lParam)
{
	switch (Message)
	{
	case WM_CHAR:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			if (StateOfProgram)
			{
				if (States[StateOfProgram - 1].ConsoleState)
				{
					States[StateOfProgram - 1].ConsoleState = 0;
				}
				else
				{
					States[StateOfProgram - 1].ConsoleState = 1;
				}
			}
		} break;
		default:
			break;
		}

		if (StateOfProgram)
		{
			if (States[StateOfProgram - 1].ConsoleState)
			{
				if (wParam == VK_BACK)
				{
					if (States[StateOfProgram - 1].ConsoleItr > 0)
					{
						States[StateOfProgram - 1].ConsoleItr--;
						States[StateOfProgram - 1].
							ConsoleGlyph[States[StateOfProgram - 1].
							ConsoleItr] = 0;
					}
					else if (!States[StateOfProgram - 1].ConsoleItr)
					{
						States[StateOfProgram - 1].
							ConsoleGlyph[States[StateOfProgram - 1].
							ConsoleItr] = 0;
					}
					else
					{
						States[StateOfProgram - 1].
							ConsoleGlyph[0] = 0;
					}
				}
				else if (wParam == VK_RETURN)
				{
					States[StateOfProgram - 1].ConsoleItr = 0;
					for (uint32 i = 0;
						i < CONSOLE_BUFFER_LENGTH;
						i++)
					{
						States[StateOfProgram - 1].ConsoleGlyph[i] = 0;
					}
				}
				else if (States[StateOfProgram - 1].ConsoleItr <
					CONSOLE_BUFFER_LENGTH)
				{
					States[StateOfProgram - 1].
						ConsoleGlyph[States[StateOfProgram - 1].
						ConsoleItr] = (uint16)wParam;
					States[StateOfProgram - 1].ConsoleItr++;
				}
			}
			else
			{
				States[StateOfProgram - 1].LastKeyPress = wParam;
			}
		}
	} break;
	default: break;
	}

	return 0;
}