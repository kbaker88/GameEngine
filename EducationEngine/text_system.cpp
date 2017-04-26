#include "text_system.h"

Text_FONT Font[2];
uint32 FontCount = 0;

static char GlobalTextStream[512];
static uint32 GlobalTextStreamLength;
char TextStream[512];
uint32 StreamLength;
char LastStreamGlyp = ' ';

void Text_BuildFont(char* FontName)
{
	Font[FontCount].FontName = FontName;
	void* Bits = 0;

	void* DeviceContext = 0;
	Platform_SetupFont("c:/Windows/Fonts/arial.ttf", FontName, &Bits, &DeviceContext);
	//"c:/Windows/Fonts/cour.ttf", "Courier New", Character

	for (uint32 Character = 32; Character < 126; Character++)
	{
		Texture2D GlyphData;
		Platform_LoadGlyph(Bits, Character, &GlyphData, DeviceContext);

		Font[FontCount].Glyph[Character].Init((float)GlyphData.Width,
			(float)GlyphData.Height);
		Font[FontCount].Glyph[Character].InputTexture(&GlyphData);

		delete[] GlyphData.Data;
	}

	// NOTE: Device Context is for win32 only
	Platform_ReleaseContext(DeviceContext);

	StreamLength = 0;
	FontCount++;
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

// TODO : Write a string compare to finish this
Text_FONT* Text_GetFont(char *FontName) 
{
	return &Font[0];
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

void Text_DrawStream(v3 &Position, float Scale, uint32 ShaderID)
{
	m4 ModelMatrix = Math_IdentityMatrix();
	ModelMatrix = Math_ScaleMatrix(ModelMatrix, v3(Scale, Scale, 1.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);

	float NextWidth = 0;
	float Width = 0;
	for (uint32 i = 0; i < StreamLength; i++)
	{
		Width = Font[0].Glyph[TextStream[i]].Width;
		if (i < (StreamLength - 1))
		{
			NextWidth = Font[0].Glyph[TextStream[i + 1]].Width;
		}
		else
		{
			NextWidth = 0;
		}

		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Font[0].Glyph[TextStream[i]].Draw();

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3((Width * Scale / 2) + (NextWidth * Scale / 2), 0.0f, 0.0f));
	}
}

void Text_DrawCharLine(string &Text, v3 &Position, float Scale, uint32 ShaderID)
{
	m4 ModelMatrix = Math_IdentityMatrix();
	ModelMatrix = Math_ScaleMatrix(ModelMatrix, v3(Scale, Scale, 1.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);

	float NextWidth = 0;
	float Width = 0;
	for (uint32 i = 0; i < Text.Size; i++)
	{
		Width = Font[0].Glyph[Text.CharStr[i]].Width;
		if (i < (Text.Size - 1))
		{
			NextWidth = Font[0].Glyph[Text.CharStr[i + 1]].Width;
		}
		else
		{
			NextWidth = 0;
		}

		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Font[0].Glyph[Text.CharStr[i]].Draw();

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3((Width * Scale / 2) + (NextWidth * Scale / 2), 0.0f, 0.0f) );
	}
}