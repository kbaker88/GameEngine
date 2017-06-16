#include "game_layer.h"

// NOTE: Temporary placement
ProgramState States[5]{};
typedef void 
Module_State(ProgramState* State);
typedef int64
Module_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam);
// ENDNOTE

struct ModuleCalls
{
	Module_State* Load;
	Module_State* Run;
	Module_State* Clean;
	Module_Message_Handler* MsgHandler;
};

ModuleCalls Modules[5];
uint32 CurrentModuleID;

uint32
Game_Main(int32 CommandShow)
{
	window_properties WindowDimensions;
	WindowDimensions.Width = 1200;
	WindowDimensions.Height = 700;

#if MEMORY_ON
	Memory_Initialize(1);
#endif // MEMORY_ON

	Platform_Initialize(&WindowDimensions);
	Platform_InitRenderer();
	Render_Init(&WindowDimensions);
	Platform_ShowWindow(CommandShow);
	//Network_Init();
	DEBUG_Initialize();

	CurrentModuleID = 0;
	Modules[0].Load = (Module_State*)Title_Initialize;
	Modules[0].Run = (Module_State*)Title_Draw;
	Modules[0].Clean = (Module_State*)Title_Clean;
	Modules[0].MsgHandler = 
		(Module_Message_Handler*)Title_Message_Handler;

	Modules[1].Load = (Module_State*)Game_Initialize;
	Modules[1].Run = (Module_State*)Game_Draw;
	Modules[1].Clean = (Module_State*)Game_Clean;
	Modules[1].MsgHandler =
		(Module_Message_Handler*)Game_Message_Handler;

	Modules[2].Load = (Module_State*)Menu_Initialize;
	Modules[2].Run = (Module_State*)Menu_Draw;
	Modules[2].Clean = (Module_State*)Menu_Clean;
	Modules[2].MsgHandler =
		(Module_Message_Handler*)Menu_Message_Handler;

	Modules[3].Load = (Module_State*)Game_Exit;
	Modules[3].Run = 0;
	Modules[3].Clean = 0;
	Modules[3].MsgHandler = 
		(Module_Message_Handler*)Title_Message_Handler;

	Modules[4].Load = (Module_State*)Module_Math_Load;
	Modules[4].Run = (Module_State*)Module_Math_Run;
	Modules[4].Clean = (Module_State*)Module_Math_Clean;
	Modules[4].MsgHandler =
		(Module_Message_Handler*)Module_Math_Message_Handler;


	// NOTE: Platform_MessageLoop() also calls Game_Loop() internally
	uint32 Message = Platform_MessageLoop();

	Platform_Cleanup();
	return Message;
}

void
Game_Loop()
{
	int64 LastCounter = Platform_GetCPUCounter();

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

	CurrentModuleID = States[CurrentModuleID].StateID;
}

void
Game_Exit(ProgramState* State)
{
	State->Status = 0;
	State->StateID = 3;
	Platform_EndProgram();
}


int64
Game_MessageProcessor(void* Window, uint32 Message,
	uint64 wParam, int64 lParam)
{
	if (States[CurrentModuleID].Status == 1)
	{
		Modules[CurrentModuleID].MsgHandler(Window, Message,
			wParam, lParam);
	}

	//switch (Message)
	//{
	//case WM_CHAR:
	//{
	//	switch (wParam)
	//	{
	//	case VK_ESCAPE:
	//	{
	//		if (StateOfProgram)
	//		{
	//			if (States[StateOfProgram].ConsoleState)
	//			{
	//				States[StateOfProgram].ConsoleState = 0;
	//			}
	//			else
	//			{
	//				States[StateOfProgram].ConsoleState = 1;
	//			}
	//		}
	//	} break;
	//	default:
	//		break;
	//	}
	//
	//	if (StateOfProgram)
	//	{
	//		if (States[StateOfProgram].ConsoleState)
	//		{
	//			if (wParam == VK_BACK)
	//			{
	//				if (States[StateOfProgram].ConsoleItr > 0)
	//				{
	//					States[StateOfProgram].ConsoleItr--;
	//					States[StateOfProgram].
	//						ConsoleGlyph[States[StateOfProgram].
	//						ConsoleItr] = 0;
	//				}
	//				else if (!States[StateOfProgram].ConsoleItr)
	//				{
	//					States[StateOfProgram].
	//						ConsoleGlyph[States[StateOfProgram].
	//						ConsoleItr] = 0;
	//				}
	//				else
	//				{
	//					States[StateOfProgram].
	//						ConsoleGlyph[0] = 0;
	//				}
	//			}
	//			else if (wParam == VK_RETURN)
	//			{
	//				States[StateOfProgram].ConsoleItr = 0;
	//				for (uint32 i = 0;
	//					i < CONSOLE_BUFFER_LENGTH;
	//					i++)
	//				{
	//					States[StateOfProgram].ConsoleGlyph[i] = 0;
	//				}
	//			}
	//			else if (States[StateOfProgram].ConsoleItr <
	//				CONSOLE_BUFFER_LENGTH)
	//			{
	//				States[StateOfProgram].
	//					ConsoleGlyph[States[StateOfProgram].
	//					ConsoleItr] = (uint16)wParam;
	//				States[StateOfProgram].ConsoleItr++;
	//			}
	//		}
	//		else
	//		{
	//			States[StateOfProgram].LastKeyPress = wParam;
	//		}
	//	}
	//} break;
	//default: break;
	//}

	return 0;
}