#include "game_loop.h"

static uint8 StateOfProgram = 0;
//static TitleState Title;
static MenuState Menu;
static GameState Game;
uint32 CurrentState;
ProgramState States[3];

void Game_Loop()
{
	int64 LastCounter = Platform_GetCPUCounter();

	switch (StateOfProgram)
	{
	case 0: // Loading
	{
		DEBUG_Initialize();
		Asset_LoadTextures(); // Too general, need to make multiple loads
		Object_ResetObjectCounter();
		Text_BuildFont("arial\0");
		StateOfProgram = 1;
	} break;

	case 1: // Title Screen
	{
		if (States[0].Status == 0)
		{
			State_CreateCameras(&States[0], 1);
			State_CreateShaderVariables(&States[0], 4);
			State_CreateShaderHandles(&States[0], 2);
			State_LinkToProgram(&States[0], &StateOfProgram);

			States[0].EntityBlockNum = 0;
			States[0].Status = 1;

			Title_Initialize(&States[0]);
		}
		else
		{
			Title_Draw(&States[0]);
			if (Platform_GetStateOfKey(0x27) == 1) // SPACE BAR (VK_SPACE) REPLACE WITH SELF MADE KEY CODES
			{
				StateOfProgram = 2;
				Title_Clean(&States[0]);
				//Title.CleanUp();
			}
		}

		if (StateOfProgram != 1)
		{
			State_Clean(&States[0]);
		}
		//if (Title.CheckInitialization() == 1)
		//{
		//	Title.Display();
		//	if (Platform_GetStateOfKey(0x27) == 1) // SPACE BAR (VK_SPACE) REPLACE WITH SELF MADE KEY CODES
		//	{
		//		StateOfProgram = 2;
		//		Title.CleanUp();
		//	}
		//}
		//else
		//{
		//	Title.Init(StateOfProgram);
		//}
	} break;

	case 2: // Game
	{
		if (Game.CheckInitialization() == 1)
		{
			Game.Display();
			if (Platform_GetStateOfKey(0x0D) == 1) // Right Arrow, replace later.
			{
				StateOfProgram = 1;
				Game.CleanUp();
			}
		}
		else
		{
			Game.Init(StateOfProgram);
		}
	} break;

	case 3: // Menu Screen
	{
		if (Menu.CheckInitialization() == 1)
		{
			Menu.Display();
			if (Platform_GetStateOfKey(0x25) == 1) // Left Arrow, replace later.
			{
				StateOfProgram = 1;
				Menu.CleanUp();
			}
		}
		else
		{
			Menu.Init(StateOfProgram);
		}
	} break;

	case 4:
	{
		Platform_EndProgram();
	} break;

	default:
	{

	} break;
	}

	// Cap at 60FPS
	int64 CounterElapsed = Platform_GetCPUCounter() - LastCounter;
	float SecondsElapsedForFrame = ((float)CounterElapsed /
		(float)Platform_GetTimeFrequency());
	while (SecondsElapsedForFrame < SecondsPerFrame)
	{
		SecondsElapsedForFrame = ((float)(Platform_GetCPUCounter() - LastCounter) /
			(float)Platform_GetTimeFrequency());
	}
	
	LastCounter = Platform_GetCPUCounter();

}
