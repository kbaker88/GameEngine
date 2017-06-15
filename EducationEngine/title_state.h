#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "game_state.h"

void 
Title_Initialize(ProgramState* State);
void 
Title_Draw(ProgramState* State);
void 
Title_Clean(ProgramState* State);
int64
Title_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam);

#endif