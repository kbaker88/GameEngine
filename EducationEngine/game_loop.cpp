#include "game_loop.h"

static uint8 StateOfProgram = 0;
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
		Text_BuildFont("arial\0");
		//Network_Init();
		StateOfProgram = 1;
	} break;

	case 1: // Title Screen
	{
		if (States[0].Status == 0)
		{
			State_CreateCameras(&States[0], 1);
			State_CreateShaderVariables(&States[0], 5);
			State_CreateShaderHandles(&States[0], 2);
			State_LinkToProgram(&States[0], &StateOfProgram);

			States[0].EntityBlockNum = 0;
			States[0].ObjectBlockNum = 0;
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
			}
		}

		if ((StateOfProgram != 1) || (States[0].Status == -1))
		{
			State_Clean(&States[0]);
		}
	} break;

	case 2: // Game
	{
		if (States[1].Status == 0)
		{
			State_CreateCameras(&States[1], 1);
			State_CreateTimers(&States[1], 1);
			State_CreateShaderVariables(&States[1], 7);
			State_CreateShaderHandles(&States[1], 2);
			State_LinkToProgram(&States[1], &StateOfProgram);

			States[1].EntityBlockNum = 0;
			States[1].ObjectBlockNum = 0;
			States[1].Status = 1;

			Game_Initialize(&States[1]);
		}
		else
		{
			Game_Draw(&States[1]);
			if (Platform_GetStateOfKey(0x25) == 1) // Left Arrow, replace later.
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

	case 3: // Menu Screen
	{
		if (States[2].Status == 0)
		{
			State_CreateCameras(&States[2], 1);
			State_CreateShaderVariables(&States[2], 5);
			State_CreateShaderHandles(&States[2], 2);
			State_LinkToProgram(&States[2], &StateOfProgram);

			States[2].EntityBlockNum = 0;
			States[2].ObjectBlockNum = 0;
			States[2].Status = 1;

			Menu_Initialize(&States[2]);
		}
		else
		{
			Menu_Draw(&States[2]);
			if (Platform_GetStateOfKey(0x25) == 1) // Left Arrow, replace later.
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
