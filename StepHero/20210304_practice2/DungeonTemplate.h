#pragma once
class DungeonTemplate
{
public:
	int rows;
	int cols;

	int** fieldSet = nullptr;

	DungeonTemplate();
	DungeonTemplate(int rows, int cols):rows(rows), cols(cols) {};
	~DungeonTemplate();

	void Release();
};