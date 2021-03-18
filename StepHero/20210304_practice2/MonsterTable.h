#pragma once
#include "Monster.h"
#include "Dungeon.h"

class MonsterTable
{
public:
	Monster monster[Room::FieldType::wall][4];
	
	MonsterTable();
	~MonsterTable();

	void Init(int strength);

	// 몬스터 생성	
	Monster* CreateMonster(int fieldType);
	Monster* GetMonster(Room::FieldType fieldType);
};

