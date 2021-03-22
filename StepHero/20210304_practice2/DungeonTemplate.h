#pragma once
class DungeonTemplate
{
public:
	int rows;
	int cols;

	int** fieldSet = nullptr;

private:

public:
	DungeonTemplate();
	DungeonTemplate(int rows, int cols);
	~DungeonTemplate();

	void Init();
	void Release();
};