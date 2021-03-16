#include "stdafx.h"
#include "FileManager.h"

FileManager::FileManager()
{	//
	//dataList[DataType::POS_X] = "2";
	//dataList[DataType::POS_Y] = "2";
	//dataList[DataType::GOLD] = "123123";
	//dataList[DataType::MON_COUNT] = "4";
	//dataList[DataType::MON_POS_X] = "1,2,3,4,";
	//dataList[DataType::MON_POS_Y] = "5,6,7,8,";
	//dataList[DataType::DIFFICULTY] = "2";
	//dataList[DataType::ROOM_DATA] = "1,2,3,4,5,6,7,8,9,0,";
	//
	//SaveData("SaveData.txt");
	//LoadData("SaveData.txt");
}

FileManager::~FileManager()
{
}

void FileManager::InsertData(DataType dataType, int data)
{
	//Monster�� ���⼭ ó������ �ʴ´�.
	switch (dataType)
	{
	case FileManager::POS_X:
	case FileManager::POS_Y:
	case FileManager::GOLD:
	case FileManager::DIFFICULTY:
		dataList[dataType] = data;
		break;
	case FileManager::ROOM_DATA:
		dataList[dataType] += to_string(data) + ",";
		break;
	}
}

void FileManager::InsertMonsterData(int x, int y)
{
	if (dataList[DataType::MON_COUNT].length() < 1) dataList[DataType::MON_COUNT] = 1;
	else dataList[DataType::MON_COUNT] = stoi(dataList[DataType::MON_COUNT]) + 1;
	dataList[DataType::MON_POS_X] += to_string(x) + ",";
	dataList[DataType::MON_POS_Y] += to_string(y) + ",";
}

void FileManager::SaveData(string fileName)
{
	ofstream fout(fileName);
	for (auto str : dataList)
	{
		fout << str << "\n";
	}
	fout.close();
}

void FileManager::LoadData(string fileName)
{
	DataClear();
	ifstream fin(fileName);
	for (auto& str : dataList)
	{
		fin >> str;
	}
	fin.close();
}

void FileManager::DataClear()
{
	for (string& str : dataList)
	{
		str = "";
	}
}

int FileManager::GetData(DataType dataType, int index)
{
	int data = 0, offset = -1;
	string dataLine;
	switch (dataType)
	{
	case FileManager::POS_X:
	case FileManager::POS_Y:
	case FileManager::GOLD:
	case FileManager::MON_COUNT:
	case FileManager::DIFFICULTY:
		data = stoi(dataList[dataType]);
		break;
	case FileManager::MON_POS_X:
		// x1,x2
		dataLine = dataList[DataType::MON_POS_X];
		//dataLine = "0,1,2,3,4,5,6,7,8,9,10,";
		for (int i = 0; i < index; ++i)
		{
			offset = dataLine.find(',', offset+1);
		}
		data = stoi(dataLine.substr(offset + 1, dataLine.find(',', offset+1) - offset - 1));
		break;
	case FileManager::MON_POS_Y:
		dataLine = dataList[DataType::MON_POS_Y];
		//dataLine = "0,1,2,3,4,5,6,7,8,9,10,";
		for (int i = 0; i < index; ++i)
		{
			offset = dataLine.find(',', offset + 1);
		}
		data = stoi(dataLine.substr(offset + 1, dataLine.find(',', offset + 1) - offset - 1));
		break;
	case FileManager::ROOM_DATA:
		dataLine = dataList[DataType::ROOM_DATA];
		//dataLine = "0,1,2,3,4,5,6,7,8,9,10,";
		for (int i = 0; i < index; ++i)
		{
			offset = dataLine.find(',', offset + 1);
		}
		data = stoi(dataLine.substr(offset + 1, dataLine.find(',', offset + 1) - offset - 1));
		break;
	}
	return data;
}
