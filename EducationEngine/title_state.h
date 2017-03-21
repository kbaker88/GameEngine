#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "game_state.h"

//TODO: Use glsl uniform buffers
//TODO: Prevent crashes from calling init,draw,etc without setup

void Title_Initialize(ProgramState* State);
void Title_Draw(ProgramState* State);
void Title_Clean(ProgramState* State);

#endif