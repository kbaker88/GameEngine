#ifndef TEXT_H
#define TEXT_H

#include "object_system.h"

struct Text_FONT
{
	char* FontName;
	// TODO: Maybe change this to ID's, add spacing
	MyRectangle Glyph[256]; 
};

struct Text_MARKER
{
	m4 ModelMatrix;
	v3 StartPos, CurrentPos;
	float Scale;
	uint32 ID, ShaderID;
};

// TODO: maybe should be in asset?
void Text_BuildFont(char* FontName);

void Text_SendToGlobalSystem(char character);
void Text_GetFromStream();
void Text_ClearGlobalStream();

void Text_DrawStream(v3 &Position, float Scale,
	uint32 ShaderID);
void Text_DrawCharLine(string &Text, v3 &Position,
	float Scale, uint32 ShaderID);

#endif