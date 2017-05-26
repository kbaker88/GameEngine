#include "game_layer.h"

static uint8 StateOfProgram = 0;
ProgramState States[3];

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

	// NOTE: Platform_MessageLoop() also calls Game_Loop() internally
	uint32 Message = Platform_MessageLoop();

	// TODO: Temporary, remove this later.
	Asset_DeleteAll(); 
	// TODO: Temporary, remove this later.
	Text_DeleteFont(GlobalFont); 

	Platform_Cleanup();
	return Message;
}

void
Game_Loop()
{
	int64 LastCounter = Platform_GetCPUCounter();

	switch (StateOfProgram)
	{
	// NOTE: Loading State
	case 0: 
	{
#if MEMORY_ON
		Memory_Initialize(1);
#endif // MEMORY_ON
		DEBUG_Initialize();

		// TODO: Too general, need to make multiple loads
		Asset_LoadTextures(); 

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
		Texture2D FontGlyphs[255];
		Asset_LoadFont("arial\0", "c:/Windows/Fonts/arial.ttf\0",
			FontGlyphs);
		Text_BuildFont("arial\0", FontGlyphs, GlobalFont);

		//Network_Init();
		StateOfProgram = 1;
	} break;

	// NOTE: Title Screen
	case 1: 
	{
		if (States[0].Status == 0)
		{
#if MEMORY_ON
			States[0].EntityBlocks = 0;
			States[0].EntityBlocks = (EntityBlock*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(EntityBlock) * 2);

			States[0].TextObjArray = 0;
			States[0].TextObjArray = (Text_Object*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(Text_Object) * TEXT_OBJECTS_PER_PROGSTATE);
#if DATA_ORIENTED

#else
			States[0].RenderObjBlocks = 0;
			States[0].RenderObjBlocks = (RenderObjBlock*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(RenderObjBlock) * 2);
#endif // DATA_ORIENTED
#else
			States[0].EntityBlocks = new EntityBlock[2]{};
			States[0].RenderObjBlocks = new RenderObjBlock[2]{};
			States[0].TextObjArray =
				new Text_Object[TEXT_OBJECTS_PER_PROGSTATE]{};
#if DATA_ORIENTED
			States[0].ModelObjBlocks = new ModelObjBlock[2]{};
#endif // DATA_ORIENTED
#endif // MEMORY_ON
			State_CreateCameras(&States[0], 1);
			State_CreateShaderVariables(&States[0], 5);
			State_CreateShaderHandles(&States[0], 2);
			State_CreateTimers(&States[0], 2);
			State_LinkToProgram(&States[0], &StateOfProgram);

			Entity_CreateBlock(&States[0].EntityBlocks[0], 256);
#if DATA_ORIENTED
			RenderObj_CreateBlock(&States[0].RenderObjBlocks[0], 256);
			ModelObj_CreateBlock(&States[0].ModelObjBlocks[0], 256);
#else
			RenderObj_CreateBlock(&States[0].RenderObjBlocks[0], 256);
#endif // DATA_ORIENTED
			States[0].NumEntityBlocks = 1;
			States[0].NumObjectBlocks = 1;
			
			States[0].FontArr = GlobalFont;
			States[0].FontCount = 1;
			States[0].Status = 1;

			Title_Initialize(&States[0]);
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
#if MEMORY_ON
			States[1].EntityBlocks = 0;
			States[1].EntityBlocks = (EntityBlock*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(EntityBlock) * 2);
#if DATA_ORIENTED
			
#else
			States[1].RenderObjBlocks = 0;
			States[1].RenderObjBlocks = (RenderObjBlock*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(RenderObjBlock) * 2);
#endif // DATA_ORIENTED
#else
			States[1].EntityBlocks = new EntityBlock[2]{};
#if DATA_ORIENTED

#else
			States[1].RenderObjBlocks = new RenderObjBlock[2]{};
#endif // DATA_ORIENTED
#endif // MEMORY_ON
			State_CreateCameras(&States[1], 1);
			State_CreateTimers(&States[1], 1);
			State_CreateShaderVariables(&States[1], 7);
			State_CreateShaderHandles(&States[1], 2);
			State_LinkToProgram(&States[1], &StateOfProgram);

			Entity_CreateBlock(&States[1].EntityBlocks[0], 256);
#if DATA_ORIENTED

#else
			RenderObj_CreateBlock(&States[1].RenderObjBlocks[0], 256);
#endif // DATA_ORIENTED
			States[1].NumEntityBlocks = 1;
			States[1].NumObjectBlocks = 1;
			
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
#if MEMORY_ON
			States[2].EntityBlocks = 0;
			States[2].EntityBlocks = (EntityBlock*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(EntityBlock) * 2);
#if DATA_ORIENTED

#else
			States[2].RenderObjBlocks = 0;
			States[2].RenderObjBlocks = (RenderObjBlock*)Memory_GetMemPtr();
			Memory_AdvanceItr(sizeof(RenderObjBlock) * 2);
#endif // DATA_ORIENTED
#else
			States[2].EntityBlocks = new EntityBlock[2]{};
#if DATA_ORIENTED

#else
			States[2].RenderObjBlocks = new RenderObjBlock[2]{};
#endif // DATA_ORIENTED
#endif // MEMORY_ON
			State_CreateCameras(&States[2], 1);
			State_CreateShaderVariables(&States[2], 5);
			State_CreateShaderHandles(&States[2], 2);
			State_LinkToProgram(&States[2], &StateOfProgram);

			Entity_CreateBlock(&States[2].EntityBlocks[0], 256);
#if DATA_ORIENTED

#else
			RenderObj_CreateBlock(&States[2].RenderObjBlocks[0], 256);
#endif // DATA_ORIENTED
			States[2].NumEntityBlocks = 1;
			States[2].NumObjectBlocks = 1;

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