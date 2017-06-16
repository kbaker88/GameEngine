#ifndef PROGRAM_LAYER_H
#define PROGRAM_LAYER_H

#include "title_state.h"

uint32 
Game_Main(int32 CommandShow);
void 
Game_Loop();
void
Game_Exit(ProgramState* State);

// NOTE: This is assuming #if defined(_WIN64)
int64
Game_MessageProcessor(void* Window, uint32 Message,
	uint64 wParam, int64 lParam);

#endif
