#ifndef MODULE_MATH_H
#define MODULE_MATH_H

#include "input_state.h"

void
Module_Math_Load(ProgramState* State);
void
Module_Math_Run(ProgramState* State);
void
Module_Math_Clean(ProgramState* State);
int64
Module_Math_Message_Handler(void* Window,
	uint32 Message, uint64 wParam, int64 lParam);
#endif