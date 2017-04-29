#include "text_system.h"

void Text_BuildFont(char* FontName, Texture2D* GlyphArray, Text_Font* FontPtr)
{
	FontPtr->Name = FontName;
	FontPtr->GlyphsCount = 255;
	for (uint32 Character = 32; Character < 126; Character++)
	{
		FontPtr->Glyph[Character].Init((float)GlyphArray[Character].Width,
			(float)GlyphArray[Character].Height);
		FontPtr->Glyph[Character].InputTexture(&GlyphArray[Character]);
	
		delete[] GlyphArray[Character].Data;
	}
}

void Text_DeleteFont(Text_Font* FontPtr)
{
	if (FontPtr)
	{
		for (unsigned int Index = 0; Index < FontPtr->GlyphsCount; Index++)
		{
			FontPtr->Glyph[Index].Delete();
		}
		// NOTE: This only deletes one font for now
		delete FontPtr;
	}
	else
	{
		// TODO: Error
	}
}

void Text_InputBox(float Width, float Height, v3 *Position)
{

}

void Text_Box(float Width, float Height, v3 *Position)
{

}

void Text_DrawConsole(v3* Position, float Scale, uint32 ShaderID,
	Text_Font *Font, uint16* ConsoleBuffer, uint32 BufferLength)
{
	m4 ModelMatrix = Math_IdentityMatrix();
	ModelMatrix = Math_ScaleMatrix(ModelMatrix, v3(Scale, Scale, 1.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, *Position);

	float Width = 0, NextWidth = 0;
	for (uint32 i = 0; i < BufferLength; i++)
	{
		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Font->Glyph[ConsoleBuffer[i]].Draw();

		Width = Font->Glyph[ConsoleBuffer[i]].Width;
		if (i < (BufferLength - 1))
		{
			NextWidth = Font->Glyph[ConsoleBuffer[i + 1]].Width;
		}
		else
		{
			NextWidth = 0;
		}

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3(Width * Scale * 0.5f + NextWidth * Scale * 0.5f, 0.0f, 0.0f));
	}
}

void Text_DrawCharLine(string &Text, v3 &Position, float Scale,
	uint32 ShaderID, Text_Font *Font)
{
	m4 ModelMatrix = Math_IdentityMatrix();
	ModelMatrix = Math_ScaleMatrix(ModelMatrix, v3(Scale, Scale, 1.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);

	float NextWidth = 0;
	float Width = 0;
	for (uint32 i = 0; i < Text.Size; i++)
	{
		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Font->Glyph[Text.CharStr[i]].Draw();

		Width = Font->Glyph[Text.CharStr[i]].Width;
		if (i < (Text.Size - 1))
		{
			NextWidth = Font->Glyph[Text.CharStr[i + 1]].Width;
		}
		else
		{
			NextWidth = 0;
		}

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3((Width * Scale * 0.5f) + (NextWidth * Scale * 0.5f), 0.0f, 0.0f) );
	}
}