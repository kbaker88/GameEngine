#include "text_system.h"

// NOTE: Temporary?
Font* TextFont;
float TextScale;
uint32 ShaderVarID;


void
Text_SetFont(uint32 FontID)
{
	TextFont = Asset_GetFont(FontID);
}

void
Text_SetFontSize(float Size)
{
	TextScale = Size;
}

void
Text_SetShaderVarID(uint32 ShaderVariableID)
{
	ShaderVarID = ShaderVariableID;
}

void
Text_CreateObj(Text_Object* TextObj, float Scale,
	v3 *Position, uint32 MaxLength, Font* Font)
{
#if MEMORY_ON
	TextObj->Buffer = 0;
	TextObj->Buffer = Memory_Allocate(TextObj->Buffer, MaxLength);
#else
	TextObj->Buffer = new uint16[MaxLength];
#endif // MEMORY_ON
	TextObj->Length = MaxLength;
	TextObj->Font = Font;
	TextObj->XScale = Scale;
	TextObj->YScale = Scale;
	TextObj->Position = *Position;
	TextObj->CollisionResult = 0;
}

void
Text_InputBoxUpdate(Text_Object* TextObj, uint16 Glyph, 
	bool SecPing)
{
	if (TextObj)
	{
		if (!TextObj->CollisionResult)
		{

		}
		else
		{
			if ((Glyph == VK_BACK) && (TextObj->Itr > 0))
			{
				TextObj->Itr--;
				TextObj->Buffer[TextObj->Itr] = 0;
			}
			else if (TextObj->Itr < TextObj->Length)
			{
				if (Glyph)
				{
					TextObj->Buffer[TextObj->Itr] = Glyph;
					TextObj->Itr++;
				}
				if (SecPing)
				{
					TextObj->Buffer[TextObj->Itr] = '|';
				}
				else
				{
					TextObj->Buffer[TextObj->Itr] = ' ';
				}
			}
		}
	}
	else
	{
		// TODO: Error
	}
}

void 
Text_Draw(Text_Object* TextObj, uint32 ShaderID)
{
	if (TextObj)
	{
		m4 ModelMatrix = Math_ScaleMatrix(Math_IdentityMatrix(),
			v3(TextObj->XScale, TextObj->YScale, 1.0f));
		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			TextObj->Position);

		float Width = 0;
		uint32 ItrLength = TextObj->Itr;
		if (TextObj->Itr && (TextObj->Itr < TextObj->Length))
		{
			ItrLength++;
		}
		for (uint32 i = 0; i < ItrLength; i++)
		{
			Render_UpdateShaderVariable(ShaderID, 44,
				&ModelMatrix.Rc[0][0], 1, 0);

			Width = Text_SpacingWidth(TextObj->Font, 
				TextObj->Buffer[i], TextObj->Buffer[i + 1]);

			ModelMatrix = Math_TranslateMatrix(ModelMatrix,
				v3(Width * TextObj->XScale, 0.0f, 0.0f));
		}
	}
	else
	{
		// TODO: Error
	}
}

void
Text_DrawConsole(v3* Position, float Scale, uint32 ShaderID,
	Font *Font, uint16* ConsoleBuffer, uint32 BufferLength)
{
	m4 ModelMatrix = Math_ScaleMatrix(Math_IdentityMatrix(),
		v3(Scale, Scale, 1.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, *Position);

	float Width = 0;
	for (uint32 i = 0; i < BufferLength; i++)
	{
		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Width = Text_SpacingWidth(Font, ConsoleBuffer[i], 
			ConsoleBuffer[i + 1]);

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3(Width * Scale, 0.0f, 0.0f));
	}
}

void
Text_DrawCharLine(string &Text, v3 &Position)
{
	m4 ModelMatrix = Math_ScaleMatrix(Math_IdentityMatrix(),
		v3(TextScale, TextScale, 0.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);

	uint32 TextEndingIndex = Text.Size - 1;
	float Width = 0;
	for (uint32 i = 0; i < TextEndingIndex; i++)
	{
		Render_UpdateShaderVariable(ShaderVarID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Render_BindTexture(
			TextFont->GlyphImageID[Text.CharStr[i] - 32]);
		Render_Draw(&TextFont->Glyph[Text.CharStr[i] - 32]);
		Render_BindTexture(0);

		Width = Text_SpacingWidth(TextFont, Text.CharStr[i] - 32,
			Text.CharStr[i + 1] - 32);

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3(Width * TextScale, 0.0f, 0.0f) );
	}
	Render_UpdateShaderVariable(ShaderVarID, 44,
		&ModelMatrix.Rc[0][0], 1, 0);

	Render_BindTexture(
		TextFont->GlyphImageID[Text.CharStr[TextEndingIndex] - 32]);
	Render_Draw(&TextFont->Glyph[Text.CharStr[TextEndingIndex] - 32]);
	Render_BindTexture(0);

	Width = TextFont->Width[Text.CharStr[TextEndingIndex] - 32] * 0.5f;

	ModelMatrix = Math_TranslateMatrix(ModelMatrix,
		v3(Width * TextScale, 0.0f, 0.0f));
}

float 
Text_SpacingWidth(Font* TextFont, uint16 A, uint16 B)
{
	if (TextFont)
	{
		float Width = (float)TextFont->Width[A];
		float NextWidth = (float)TextFont->Width[B];
		return (Width * 0.5f + NextWidth * 0.5f);
	}
	else
	{
		// TODO: Error
		return 0;
	}
}