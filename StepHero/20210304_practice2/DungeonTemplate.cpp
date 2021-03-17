#include "stdafx.h"
#include "DungeonTemplate.h"


DungeonTemplate::DungeonTemplate()
{
}


DungeonTemplate::~DungeonTemplate()
{
}

void DungeonTemplate::Release()
{
	for (int i = 0; i < rows; ++i)
	{
		delete[] fieldSet[i];
	}
	delete[] fieldSet;
}