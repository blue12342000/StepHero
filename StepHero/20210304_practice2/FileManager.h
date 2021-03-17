#pragma once

class FileManager
{
public:
	enum DataType
	{
		POS_X = 0, POS_Y, GOLD, MON_COUNT, MON_POS_X, MON_POS_Y, DIFFICULTY, ROOM_DATA, NONE
	};

	FileManager();
	~FileManager();
	
	string dataList[FileManager::DataType::NONE];

	void InsertData(FileManager::DataType dataType, int data);
	void InsertMonsterData(int x, int y);
	void SaveData(string fileName);
	void LoadData(string fileName);

	void DataClear();

	int GetData(FileManager::DataType dataType, int index = 0);
};

