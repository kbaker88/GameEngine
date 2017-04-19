#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "entity_utility.h"

//TODO: Prevent crashes from calling init,draw,etc without first setting up

void Menu_Initialize(ProgramState* State);
void Menu_Draw(ProgramState* State);
void Menu_Clean(ProgramState* State);
void Menu_CollisionResolve(ProgramState* State, int32 CollisionResult);

#endif