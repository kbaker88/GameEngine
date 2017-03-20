#include "structure_system.h"

DownwardList::DownwardList()
{

}

DownwardList::~DownwardList()
{

}

void DownwardList::Push(ObjectMap* NewObj)
{

	if (FirstNode.Object == 0)
	{
		FirstNode.Object = NewObj;
	}
	else
	{
		if (FirstNode.NextNode->Object == 0)
		{

		}
		else
		{
			if (FirstNode.NextNode->NextNode->Object == 0)
			{

			}
		}
	}
}

void DownwardList::Pop()
{

}

void DownwardList::Delete(uint32 Position)
{

}

ObjectMap* At(uint32 Position)
{

	return 0;
}