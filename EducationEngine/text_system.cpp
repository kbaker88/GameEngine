#include "text_system.h"

#if DATA_ORIENTED
// TODO: Make this more robust to support unicode
void
Text_BuildFont(char* FontName, Font* FontPtr)
{
	FontPtr->Name = FontName;
	FontPtr->GlyphsCount = 255;
	Texture2D GlyphImage[255];
	Asset_LoadFont(FontName, "c:/Windows/Fonts/arial.ttf\0", GlyphImage);

	for (uint32 Character = 32; Character < 126; Character++)
	{
	Model GlyphRectangle;
		ModelObj_CreateRectangle(&GlyphRectangle,
			(float)GlyphImage[Character].Width,
			(float)GlyphImage[Character].Height);

		RenderObj_CreateRenderObject(
			&FontPtr->Glyph[Character], &GlyphRectangle);

		Render_BuildTexture(&FontPtr->GlyphImageID[Character],
			GlyphImage[Character].Width, GlyphImage[Character].Height,
			GlyphImage[Character].Data);

		FontPtr->Width[Character] = (float)GlyphImage[Character].Width;
#if MEMORY_ON
		// TODO: Make cleanup system
#else
		delete[] GlyphImage[Character].Data;
#endif // MEMORY_ON
	}

}
#else
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
	
#if MEMORY_ON
		// TODO: Make cleanup system
#else
		delete[] GlyphArray[Character].Data;
#endif // MEMORY_ON
	}
}
#endif // DATA_ORIENTED

void 
Text_DeleteFont(Font* FontPtr)
{
#if DATA_ORIENTED

#else
	if (FontPtr)
	{
		for (unsigned int Index = 0; Index < FontPtr->GlyphsCount; Index++)
		{
			FontPtr->Glyph[Index].Delete();
		}
		// NOTE: This only deletes one font for now
#if MEMORY_ON
		// TODO: Make cleanup system
#else
		delete FontPtr;
#endif // MEMORY_ON
	}
	else
	{
		// TODO: Error
	}
#endif // DATA_ORIENTED
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

#if DATA_ORIENTED

#else
			TextObj->Font->Glyph[TextObj->Buffer[i]].Draw();
#endif // DATA_ORIENTED
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

#if DATA_ORIENTED

#else
		Font->Glyph[ConsoleBuffer[i]].Draw();
#endif // DATA_ORIENTED
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

#if DATA_ORIENTED
		Render_BindTexture(Font->GlyphImageID[Text.CharStr[i]]);
		Render_Draw(&Font->Glyph[Text.CharStr[i]]);
		Render_BindTexture(0);
#else
		Font->Glyph[Text.CharStr[i]].Draw();
#endif// DATA_ORIENTED

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
#if DATA_ORIENTED
			float Width = (float)Font->Width[A];
			float NextWidth = (float)Font->Width[B];
			return (Width * 0.5f + NextWidth * 0.5f);
#else
			float Width = (float)Font->Glyph[A].Width;
			float NextWidth = (float)Font->Glyph[B].Width;
			return (Width * 0.5f + NextWidth * 0.5f);
#endif // DATA_ORIENTED
		}
		else if (A)
		{
#if DATA_ORIENTED
			return (float)Font->Width[A];
#else
			return (float)Font->Glyph[A].Width;
#endif // DATA_ORIENTED
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

#if DATA_ORIENTED
	return 0;
#endif // DATA_ORIENTED
}

















