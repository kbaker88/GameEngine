#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "menu_state.h"

class GameState
{
public:
	GameState();
	~GameState();

	void Init(uint8 &programState);
	uint8 CheckInitialization();
	void Display();
	void CleanUp();

private:
	UserInterface UI;

	Box DefaultBox;
	Box ActiveBox;

	uint8 *StateOfProgram;
	uint8 initialized;
};

#endif




