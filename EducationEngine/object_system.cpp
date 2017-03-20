#include "object_system.h"

ObjectList Objects;
uint32 ObjectID;

void Object_ResetObjectCounter()
{
	ObjectID = 0;
}

uint32 Object_Load(Object* NewObject)
{
	NewObject->Init();
	NewObject->ID = ObjectID;
	ObjectID++;
	Objects.Add(NewObject);

	return NewObject->ID;
}

uint32 Object_Load(Object* NewObject, uint32 Width, uint32 Depth)
{
	NewObject->Init(Width, Depth);
	NewObject->ID = ObjectID;
	ObjectID++;
	Objects.Add(NewObject);
	return NewObject->ID;
}

uint32 Object_Load(Object* NewObject, float Width, float Height, float Depth)
{
	NewObject->Init(Width, Height, Depth);
	NewObject->ID = ObjectID;
	ObjectID++;
	Objects.Add(NewObject);

	return NewObject->ID;
}

void Object_SetTexture(uint32 ObjectID, TextureStorage* Texture)
{
	Object_Find(ObjectID)->InputTexture(Texture);
}

Object* Object_Find(uint32 ObjectID)
{
	return Objects.Find(ObjectID);
}

void Object_Delete(uint32 ID)
{
	Objects.Delete(ID);
}

void Object_ClearAll()
{
	Objects.Clear();
}

ObjectList::ObjectList()
{
	FirstObject = 0;
}

ObjectList::~ObjectList()
{

}

void ObjectList::Add(Object *ObjectPtr)
{
	ObjectListNode* Temporary;

	if (!FirstObject)
	{
		FirstObject = new ObjectListNode;
		FirstObject->ObjectPtr = ObjectPtr;
		FirstObject->NextObject = 0;
	}
	else
	{
		Temporary = FirstObject;
		while (Temporary->NextObject)
		{
			Temporary = Temporary->NextObject;
		}

		Temporary->NextObject = new ObjectListNode;
		Temporary->NextObject->ObjectPtr = ObjectPtr;
		Temporary->NextObject->NextObject = 0;
	}
}

void ObjectList::Delete(uint32 ID)
{
	ObjectListNode *TempNode = FirstObject;
	ObjectListNode *PrevNode = 0;

	while (TempNode)
	{
		if (TempNode->ObjectPtr->ID == ID)
		{
			if (PrevNode)
			{
				PrevNode->NextObject = TempNode->NextObject;
				TempNode->NextObject = 0;
				TempNode->ObjectPtr->Delete();
				TempNode->ObjectPtr->~Object();
				delete TempNode;
			}
			else
			{
				TempNode->NextObject = 0;
				TempNode->ObjectPtr->Delete();
				TempNode->ObjectPtr->~Object();
				delete TempNode;
			}
			break;
		}
		else
		{
			PrevNode = TempNode;
			TempNode = TempNode->NextObject;
		}
	}
}

Object* ObjectList::Find(uint32 ID)
{
	ObjectListNode *TempNode = FirstObject;

	while (TempNode)
	{
		if (TempNode->ObjectPtr->ID == ID)
		{
			return TempNode->ObjectPtr;
			break;
		}
		else
		{
			TempNode = TempNode->NextObject;
		}
	}
	return 0;
}

void ObjectList::Clear()
{
	ObjectListNode *Temp = FirstObject;
	ObjectListNode *NextNode = 0;

	while (Temp)
	{
		NextNode = Temp->NextObject;
		Temp->NextObject = 0;
		Temp->ObjectPtr->Delete();
		Temp->ObjectPtr->~Object();
		delete Temp;
		Temp = NextNode;
	}

	FirstObject = 0;
}