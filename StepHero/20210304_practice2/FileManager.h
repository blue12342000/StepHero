#pragma once
#include "stdafx.h"

class FileManager
{
private:

public:

	enum DataType
	{
		POS_X = 0, POS_Y, GOLD, MON_COUNT, MON_POS_X, MON_POS_Y, DIFFICULTY, ROOM_DATA, NONE
	};

	FileManager();
	~FileManager();
	
	string dataList[DataType::NONE];

	void InsertData(DataType dataType, int data);
	void InsertMonsterData(int x, int y);
	void SaveData(string fileName);
	void LoadData(string fileName);

	void DataClear();

	int GetData(DataType dataType, int index = 0);
};

