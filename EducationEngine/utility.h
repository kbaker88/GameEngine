#ifndef UTILITY_H
#define UTILITY_H	

#include "platform_layer.h"
#include "linear_algebra.h"

struct string // string inside of a string gives an error
{
	string()
	{
		CharStr = new char[1];
		CharStr[0] = '\0';
	}

	string(char* Text)
	{
		Size = 0;
		char* EndText = Text;
		if (EndText)
		{
			while (*EndText)
			{
				Size++;
				++EndText;
			}
		}
		
		CharStr = new char[Size + 1];

		for (unsigned int i = 0; i < Size; i++)
		{
			CharStr[i] = Text[i];
		}
		CharStr[Size] = '\0';
	}

	string(char* Text, unsigned int NumberOfChars)
	{
		Size = NumberOfChars;
		CharStr = new char[Size + 1];

		for (unsigned int i = 0; i < Size; i++)
		{
			CharStr[i] = Text[i];
		}
		CharStr[Size] = '\0';
	}

	~string()
	{
		if (*CharStr)
		{
			delete[] CharStr;
			CharStr = 0;
		}
	}

	string& operator=(string& B)
	{
		char *DeletePtr = CharStr;
		Size = B.Size;
		char *TempPtr = new char[Size + 1];

		for (unsigned int i = 0; i < Size; i++)
		{
			TempPtr[i] = B.CharStr[i];
		}
		TempPtr[Size] = '\0';

		CharStr = TempPtr;
		delete[] DeletePtr;

		return *this;
	}

	string& operator+(string& B)
	{
		Size = Size + B.Size;
		char *TempPtr = new char[Size + 1];
		char *DeletePtr = CharStr;
		for (unsigned int i = 0; i < Size - B.Size; i++)
		{
			TempPtr[i] = CharStr[i];
		}

		for (unsigned int i = (Size - B.Size); i < Size; i++)
		{
			TempPtr[i] = B.CharStr[i - (Size - B.Size)];
		}
		TempPtr[Size] = '\0';

		CharStr = TempPtr;
		delete[] DeletePtr;

		return *this;
	}

	char* CharStr;
	unsigned int Size;
};

int8 CompareStrings(char *StrA, char* StrB);
int32 FindString(char* Str, char* Data);
int32 FindString(char* Str, uint32 StartPos, char* Data);
uint32 StringLength(char* Str);
char* AppendString(char* StrA, char* StrB);

inline int32 DeleteString(char* Str)
{
	if (Str)
	{
		delete[] Str;
		return 1;
	}
	return 0;
}


// TEMPLATES, REMOVE LATER

template <typename Type> 
struct LinkedListNode
{
	LinkedListNode *NextNode;
	Type Data;
};

template <class Type>
class LinkedList
{
public:
	LinkedList();
	~LinkedList();

	void Add(Type value);
	void Delete(Type value);
	Type Get();
	void AdvanceItr();
	void ResetItr();
	bool End();
	bool Find(Type value);
	void ClearLinkedList();

	LinkedListNode<Type> *StartNode;
	LinkedListNode<Type> *Itr;

};

template <class Type>
LinkedList<Type>::LinkedList()
{
	Itr = StartNode;
}

template <class Type>
LinkedList<Type>::~LinkedList()
{

}

template <class Type>
void LinkedList<Type>::Add(Type value)
{
	LinkedListNode<Type>* Temporary;

	if (!StartNode)
	{
		StartNode = new LinkedListNode<Type>;
		StartNode->Data = value;
		StartNode->NextNode = 0;
		Itr = StartNode;
	}
	else
	{
		Temporary = StartNode;
		while (Temporary->NextNode)
		{
			Temporary = Temporary->NextNode;
		}

		Temporary->NextNode = new LinkedListNode<Type>;
		Temporary->NextNode->Data = value;
		Temporary->NextNode->NextNode = 0;
	}
}

template <class Type>
void LinkedList<Type>::Delete(Type value)
{
	LinkedListNode<Type> *TempNode = StartNode;
	LinkedListNode<Type> *PrevNode = 0;

	while (TempNode)
	{
		if (TempNode->Data == value)
		{
			if (PrevNode)
			{
				PrevNode->NextNode = TempNode->NextNode;
				TempNode->NextNode = 0;
				delete[] TempNode;
			}
			else
			{
				delete[] TempNode;
				TempNode->NextNode = 0;
			}
			break;
		}
		else
		{
			PrevNode = TempNode;
			TempNode = TempNode->NextNode;
		}
	}
}

template <class Type>
bool LinkedList<Type>::Find(Type value)
{
	LinkedListNode<Type> *TempNode = StartNode;

	if (TempNode)
	{
		while (TempNode)
		{
			if (TempNode->Data == value)
			{
				return 1;
			}
			else
			{
				TempNode = TempNode->NextNode;
			}
		}
		return 0;
	}
	else
	{
		return 0;
	}
}

template <class Type>
Type LinkedList<Type>::Get()
{
	return Itr->Data;
}

template <class Type>
void LinkedList<Type>::AdvanceItr()
{
	if (Itr->NextNode)
	{
		Itr = Itr->NextNode;
	}
	else
	{
		Itr = 0;
	}
}

template <class Type>
bool LinkedList<Type>::End()
{
	if (Itr)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

template <class Type>
void LinkedList<Type>::ResetItr()
{
	Itr = StartNode;
}

template <class Type>
void LinkedList<Type>::ClearLinkedList()
{
	LinkedListNode<Type> *Temp = StartNode;
	LinkedListNode<Type> *NextNode = 0;

	while (Temp)
	{
		NextNode = Temp->NextNode;
		delete[] Temp;
		Temp = NextNode;
	}

	StartNode = 0;
}

#endif