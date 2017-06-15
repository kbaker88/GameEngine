#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "menu_state.h"

//TODO: Prevent crashes from calling init,draw,etc without first setting up

/* MovementSpeed * SecondsPerFrame * PixelsPerMeter
   1.4 meters/sec * 1/60 sec/frame * 0.25 pix/meter
*/

void 
Game_Initialize(ProgramState* State);
void 
Game_Draw(ProgramState* State);
void 
Game_Clean(ProgramState* State);
int64
Game_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam);

#endif




