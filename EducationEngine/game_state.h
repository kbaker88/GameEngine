#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "menu_state.h"
/* MovementSpeed * SecondsPerFrame * PixelsPerMeter
   1.4 meters/sec * 1/60 sec/frame * 0.25 pix/meter
*/

void Game_Initialize(ProgramState* State);
void Game_Draw(ProgramState* State);
void Game_Clean(ProgramState* State);

#endif




