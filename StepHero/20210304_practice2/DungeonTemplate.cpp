#include "stdafx.h"
#include "DungeonTemplate.h"

DungeonTemplate::DungeonTemplate()
{
}

DungeonTemplate::DungeonTemplate(int rows, int cols) :rows(rows), cols(cols)
{
}

DungeonTemplate::~DungeonTemplate()
{
}

void DungeonTemplate::Init()
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