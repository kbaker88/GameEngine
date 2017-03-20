#include "utility.h"

int8 CompareStrings(char *StrA, char* StrB)
{
	if (StrA)
	{
		if (!StrB)
		{
			return 0;
		}
		while (*StrA)
		{
			if (StrB)
			{
				if (*StrA == *StrB)
				{
					++StrA;
					++StrB;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
	}
	else if (StrB)
	{
		return 0;
	}
	else
	{
		return 1;
	}

	if (*StrB)
	{
		return 0;
	}

	return 1;
}

int32 FindString(char* Str, char* Data)
{
	char* StrItr = Str;
	char* DataItr = Data;
	int32 StrPos = -1;

	if (Data)
	{
		if (Str)
		{
			while (*DataItr)
			{
				if (*StrItr == *DataItr)
				{
					++StrItr;
				}
				else if (!*StrItr)
				{
					++StrPos;
					return StrPos;
				}
				else
				{
					StrItr = Str;
					++StrPos;
				}
				++DataItr;
			}
			return -1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int32 FindString(char* Str, uint32 StartPos, char* Data)
{
	char* StrItr = Str;
	char* DataItr = Data;

	for (uint32 i = 0; i <= StartPos; i++)
	{
		++DataItr;
	}
	int32 StrPos = StartPos;

	if (Data)
	{
		if (Str)
		{
			while (*DataItr)
			{
				if (*StrItr == *DataItr)
				{
					++StrItr;
				}
				else if (!*StrItr)
				{
					return StrPos;
				}
				else
				{
					StrItr = Str;
					++StrPos;
				}
				++DataItr;
			}
			return -1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

uint32 StringLength(char* Str)
{
	uint32 Length = 0;

	if (Str)
	{
		while (*Str)
		{
			Length++;
			++Str;
		}
		return Length;
	}
	else
	{
		return Length;
	}
}

char* AppendString(char* StrA, char* StrB)
{
	int LengthA = 0;
	int LengthB = 0;
	LengthA += StringLength(StrA);
	LengthB += StringLength(StrB);

	char* Result = new char[LengthA + LengthB];

	for (int i = 0; i < LengthA; i++)
	{
		Result[i] = StrA[i];
	}

	for (int i = LengthA; i < (LengthA + LengthB); i++)
	{
		Result[i] = StrB[i - LengthA];
	}

	return (Result);
}
