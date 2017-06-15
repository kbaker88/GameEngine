#ifndef TEXT_H
#define TEXT_H

#include "asset_system.h"

#define CONSOLE_BUFFER_LENGTH 256

// TODO: Temporary
#define TEXT_OBJECTS_PER_PROGSTATE 256

struct Text_Object
{
	Text_Object() : Buffer(0), Length(0), 
		CollisionResult(0), Itr(0), Font(0),
		XScale(0), YScale(0) {}
	v3 Position;
	Font* Font;
	uint16* Buffer;
	float XScale;
	float YScale;
	uint32 Itr;
	uint32 Length;
	int32 CollisionResult;
};

void
Text_SetFont(uint32 FontID);
void
Text_SetFontSize(float Size);
void
Text_SetShaderVarID(uint32 ShaderVariableID);
void 
Text_CreateObj(Text_Object* TextObj, float Scale,
	v3 *Position, uint32 MaxLength, Font* Font);
void
Text_InputBoxUpdate(Text_Object* TextObj, 
	uint16 Glyph, bool SecPing);
void 
Text_Draw(Text_Object* TextObj, uint32 ShaderID);
void 
Text_DrawConsole(v3* Position, float Scale,
	uint32 ShaderID, Font *Font, uint16* ConsoleBuffer, 
	uint32 BufferLength);
void
Text_DrawCharLine(string &Text, v3 &Position);
float
Text_SpacingWidth(Font* TextFont, uint16 A,
	uint16 B);

#endif