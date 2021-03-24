#pragma once
#include "Monster.h"
#include "Dungeon.h"

class MonsterTable
{
private:
	Monster monster[Room::FieldType::FT_WALL][4];

public:
	MonsterTable();
	~MonsterTable();

	void Init(int strength);

	// 몬스터 생성	
	Monster GetMonster(Room::FieldType fieldType);
};

