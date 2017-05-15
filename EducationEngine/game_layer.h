#ifndef PROGRAM_LAYER_H
#define PROGRAM_LAYER_H

#include "title_state.h"

//TODO: Move this to somewhere else, temporary testing
#include "network_layer.h"

uint32 
Game_Main(int32 CommandShow);
void 
Game_Loop();

// NOTE: This is assuming #if defined(_WIN64)
int64
Game_MessageProcessor(void* Window, uint32 Message,
	uint64 wParam, int64 lParam);

#endif
