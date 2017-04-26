#include "text_system.h"

//Text_Font Fonts;
uint32 FontCount = 0;

static char GlobalTextStream[512];
static uint32 GlobalTextStreamLength;
char TextStream[512];
uint32 StreamLength = 0;
char LastStreamGlyp = ' ';

void Text_BuildFont(char* FontName, Texture2D* GlyphArray, Text_Font* Font)
{
	Font->Name = FontName;
	for (uint32 Character = 32; Character < 126; Character++)
	{
		Font->Glyph[Character].Init((float)GlyphArray[Character].Width,
			(float)GlyphArray[Character].Height);
		Font->Glyph[Character].InputTexture(&GlyphArray[Character]);
	
		delete[] GlyphArray[Character].Data;
	}

	StreamLength = 0;
}

void Text_SendToGlobalSystem(char character)
{
	if (GlobalTextStreamLength < 512)
	{
		GlobalTextStream[GlobalTextStreamLength] = character;
		LastStreamGlyp = character;
		GlobalTextStreamLength++;
	}
	else
	{
		GlobalTextStreamLength = 0;
	}
}

void Text_ClearGlobalStream()
{
	GlobalTextStreamLength = 0;
}

char Text_LastGlyphPressed()
{
	return LastStreamGlyp;
}

uint32 Text_GetGlobalStreamLength()
{
	return GlobalTextStreamLength;
}

void Text_GetFromStream()
{
	for (uint32 i = 0; i < GlobalTextStreamLength; i++)
	{
		// TODO : move this in another function?
		if (GlobalTextStream[i] == '`') 
		{
			// DO NOT COPY THIS GLYPH?
		}
		else
		{
			if (StreamLength < 512)
			{
				// NOTE: if Delete or Backspace was pressed
				if (((GlobalTextStream[i] == 127) || 
					(GlobalTextStream[i] == 8)) &&
					(StreamLength > 0))
				{ 
					StreamLength--;
				}
				// NOTE: if Return or Enter was pressed
				else if ((GlobalTextStream[i] == 13) ||
					(GlobalTextStream[i] == 10))
				{ 
					StreamLength = 0;
				}
				else
				{
					TextStream[StreamLength] = GlobalTextStream[i];
					StreamLength++;
				}
			}
		}
	}
}

void Text_DrawStream(v3 &Position, float Scale, uint32 ShaderID, 
	Text_Font *Font)
{
	m4 ModelMatrix = Math_IdentityMatrix();
	ModelMatrix = Math_ScaleMatrix(ModelMatrix, v3(Scale, Scale, 1.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);

	float NextWidth = 0;
	float Width = 0;
	for (uint32 i = 0; i < StreamLength; i++)
	{
		Width = Font->Glyph[TextStream[i]].Width;
		if (i < (StreamLength - 1))
		{
			NextWidth = Font->Glyph[TextStream[i + 1]].Width;
		}
		else
		{
			NextWidth = 0;
		}

		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Font->Glyph[TextStream[i]].Draw();

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3((Width * Scale / 2) + (NextWidth * Scale / 2), 0.0f, 0.0f));
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
		Width = Font->Glyph[Text.CharStr[i]].Width;
		if (i < (Text.Size - 1))
		{
			NextWidth = Font->Glyph[Text.CharStr[i + 1]].Width;
		}
		else
		{
			NextWidth = 0;
		}

		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Font->Glyph[Text.CharStr[i]].Draw();

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3((Width * Scale / 2) + (NextWidth * Scale / 2), 0.0f, 0.0f) );
	}
}