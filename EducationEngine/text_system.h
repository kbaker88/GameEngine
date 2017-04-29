#ifndef TEXT_H
#define TEXT_H

#include "renderobject_system.h"

struct Text_Font
{
	Text_Font() : Name(0), GlyphsCount(0) {}
	char* Name;
	// TODO: Maybe change this to ID's, add spacing
	// TODO: Dynamically allocate Glyph array size
	//		 to allow for more languages
	MyRectangle Glyph[255]; 
	unsigned int GlyphsCount;
};

// TODO: Make sure Data is deleted and Glypharray
//	     is deleted if it was dynamically allocated
void Text_BuildFont(char* FontName, 
	Texture2D* GlyphArray, Text_Font* FontPtr);
void Text_DeleteFont(Text_Font* FontPtr);

// TODO: Add Support for UNICODE
void Text_DrawConsole(v3* Position, float Scale, uint32 ShaderID,
	Text_Font *Font, uint16* ConsoleBuffer, uint32 BufferLength);

void Text_DrawCharLine(string &Text, v3 &Position,
	float Scale, uint32 ShaderID, Text_Font *Font);

#endif