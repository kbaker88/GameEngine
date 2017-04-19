#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "game_state.h"

//TODO: Use glsl uniform buffers
//TODO: Prevent crashes from calling init,draw,etc without first setting up

void Title_Initialize(ProgramState* State);
void Title_Draw(ProgramState* State);
void Title_CollisionResolve(ProgramState* State, int32 CollisionResult);
void Title_Clean(ProgramState* State);

#endif