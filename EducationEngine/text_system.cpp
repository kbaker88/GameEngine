#include "text_system.h"

static TextureStorage Letters[256]; // data and dimensions

Text_MARKER TextMarkers[256];
uint32 TextMarkerCount = 0;

Text_FONT Font[2];
uint32 FontCount = 0;

static char GlobalTextStream[512];
static uint32 GlobalTextStreamLength;
char TextStream[512];
uint32 StreamLength;
char LastStreamGlyp = ' ';

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

void Text_BuildFont(char* FontName)
{
	loaded_bitmap GlyphData;

	Font[FontCount].FontName = FontName;
	void* Bits = 0;
	
	Platform_SetupFont("c:/Windows/Fonts/arial.ttf", FontName, &Bits);
	//"c:/Windows/Fonts/cour.ttf", "Courier New", Character

	for (uint32 Character = 32; Character < 126; Character++)
	{
		GlyphData = Platform_LoadGlyph(Bits, Character);
		Letters[Character].data = (uint8*)GlyphData.Memory;
		Letters[Character].Width = GlyphData.Width;
		Letters[Character].Height = GlyphData.Height;

		Font[FontCount].Glyph[Character].Init((float)Letters[Character].Width,
			(float)Letters[Character].Height);
		Font[FontCount].Glyph[Character].InputTexture(&Letters[Character]);
	}

	StreamLength = 0;
	FontCount++;
}

Text_FONT* Text_GetFont(char *FontName) // TODO : Write a string compare to finish this
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
		if (GlobalTextStream[i] == '`') // TODO : move this in another function?
		{
			// DO NOT COPY THIS GLYPH?
		}
		else
		{
			if (StreamLength < 512)
			{
				if (((GlobalTextStream[i] == 127) || // Delete or Backspace was pressed
					(GlobalTextStream[i] == 8)) &&
					(StreamLength > 0))
				{ 
					StreamLength--;
				}
				else if ((GlobalTextStream[i] == 13) || // Return or Enter was pressed
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
	m4 ModelMatrix = IdentityMatrix();
	ModelMatrix = ScaleMatrix(ModelMatrix, v3(Scale, Scale, 1.0f));
	ModelMatrix = TranslateMatrix(ModelMatrix, Position);

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

		Render_UpdateShaderVariable(3, ShaderID,
			&ModelMatrix.Rc[0][0]);

		Font[0].Glyph[TextStream[i]].Draw();

		ModelMatrix = TranslateMatrix(ModelMatrix,
			v3((Width * Scale / 2) + (NextWidth * Scale / 2), 0.0f, 0.0f));
	}
}

void Text_DrawCharLine(string &Text, v3 &Position, float Scale, uint32 ShaderID)
{
	m4 ModelMatrix = IdentityMatrix();
	ModelMatrix = ScaleMatrix(ModelMatrix, v3(Scale, Scale, 1.0f));
	ModelMatrix = TranslateMatrix(ModelMatrix, Position);

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

		Render_UpdateShaderVariable(3, ShaderID,
			&ModelMatrix.Rc[0][0]);

		Font[0].Glyph[Text.CharStr[i]].Draw();

		ModelMatrix = TranslateMatrix(ModelMatrix,
			v3((Width * Scale / 2) + (NextWidth * Scale / 2), 0.0f, 0.0f) );
	}
}

uint32 Text_CreateMarker(v3 &Position, float Scale, uint32 ShaderID)
{
	TextMarkers[TextMarkerCount].StartPos = Position;
	TextMarkers[TextMarkerCount].ShaderID = ShaderID;
	TextMarkers[TextMarkerCount].Scale = Scale;
	TextMarkers[TextMarkerCount].ModelMatrix = IdentityMatrix();
	TextMarkers[TextMarkerCount].ModelMatrix =
		ScaleMatrix(TextMarkers[TextMarkerCount].ModelMatrix, v3(Scale, Scale, 1.0f));
	TextMarkers[TextMarkerCount].ModelMatrix = 
		TranslateMatrix(TextMarkers[TextMarkerCount].ModelMatrix, Position);
	TextMarkerCount++;

	return (TextMarkerCount - 1);
}

void Text_NewLine(uint32 TextID)
{
	TextMarkers[TextID].ModelMatrix = TranslateMatrix(TextMarkers[TextID].ModelMatrix,
		v3(0.0f, -40.0f, 0.0f));
}




