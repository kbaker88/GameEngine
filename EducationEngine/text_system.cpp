#include "text_system.h"

void 
Text_BuildFont(char* FontName, Texture2D* GlyphArray, Font* FontPtr)
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

void 
Text_DeleteFont(Font* FontPtr)
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

void
Text_CreateObj(Text_Object* TextObj, float Scale,
	v3 *Position, uint32 MaxLength, Font* Font)
{
	TextObj->Buffer = new uint16[MaxLength];
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

			TextObj->Font->Glyph[TextObj->Buffer[i]].Draw();

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
	m4 ModelMatrix = Math_ScaleMatrix(Math_IdentityMatrix(), v3(Scale, Scale, 1.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, *Position);

	float Width = 0;
	for (uint32 i = 0; i < BufferLength; i++)
	{
		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Font->Glyph[ConsoleBuffer[i]].Draw();
		Width = Text_SpacingWidth(Font, ConsoleBuffer[i], ConsoleBuffer[i + 1]);

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3(Width * Scale, 0.0f, 0.0f));
	}
}

void 
Text_DrawCharLine(string &Text, v3 &Position, float Scale,
	uint32 ShaderID, Font *Font)
{
	m4 ModelMatrix = Math_ScaleMatrix(Math_IdentityMatrix(), v3(Scale, Scale, 1.0f));
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);

	float Width = 0;
	for (uint32 i = 0; i < Text.Size; i++)
	{
		Render_UpdateShaderVariable(ShaderID, 44,
			&ModelMatrix.Rc[0][0], 1, 0);

		Font->Glyph[Text.CharStr[i]].Draw();

		Width = Text_SpacingWidth(Font, Text.CharStr[i], Text.CharStr[i + 1]);

		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3(Width * Scale, 0.0f, 0.0f) );
	}
}

float 
Text_SpacingWidth(Font* Font, uint16 A, uint16 B)
{
	if (Font)
	{
		if (A && (B < 256) && B)
		{
			float Width = (float)Font->Glyph[A].Width;
			float NextWidth = (float)Font->Glyph[B].Width;
			return (Width * 0.5f + NextWidth * 0.5f);
		}
		else if (A)
		{
			return (float)Font->Glyph[A].Width;
		}
		else
		{
			// TODO: Error
			return 0;
		}
	}
	else
	{
		// TODO: Error
		return 0;
	}
}

















