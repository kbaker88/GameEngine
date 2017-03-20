#include "game_state.h"

GameState::GameState() : initialized(0)
{
}

GameState::~GameState()
{
}

void GameState::Init(uint8 &programState)
{
	StateOfProgram = &programState;

	InitializeWorld();

	UI.Init();

	initialized = 1;
}

uint8 GameState::CheckInitialization()
{
	return initialized;
}

void GameState::Display()
{
	Render_ClearScreen();

	DrawWorld();
	UI.Draw();

	Text_ClearGlobalStream();
}

void GameState::CleanUp()
{
	WorldCleanUp();
	initialized = 0;
}