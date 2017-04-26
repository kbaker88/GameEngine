#ifndef TEXT_H
#define TEXT_H

#include "object_system.h"

struct Text_Font
{
	char* Name;
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

// TODO: Make sure Data is deleted and Glypharray
//	     is deleted if it was dynamically allocated
//void Text_BuildFont(char* FontName);
void Text_BuildFont(char* FontName, 
	Texture2D* GlyphArray, Text_Font* Font);

void Text_SendToGlobalSystem(char character);
void Text_GetFromStream();
void Text_ClearGlobalStream();

void Text_DrawStream(v3 &Position, float Scale,
	uint32 ShaderID, Text_Font *Font);
void Text_DrawCharLine(string &Text, v3 &Position,
	float Scale, uint32 ShaderID, Text_Font *Font);

#endif