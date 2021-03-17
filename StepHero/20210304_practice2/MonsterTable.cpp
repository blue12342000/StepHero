#include "stdafx.h"
#include "Dungeon.h"
#include "MonsterTable.h"

MonsterTable::MonsterTable()
{
}


MonsterTable::~MonsterTable()
{
}

void MonsterTable::Init(int strength)
{
	monster[Room::FieldType::empty][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "µÎ´õÁö");
	monster[Room::FieldType::empty][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "½½¶óÀÓ" );
	monster[Room::FieldType::empty][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "µµÀû" );
	monster[Room::FieldType::empty][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "»êÀû" );

	monster[Room::FieldType::wood][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "Åä³¢" );
	monster[Room::FieldType::wood][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "»ç½¿" );
	monster[Room::FieldType::wood][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "´Á´ë" );
	monster[Room::FieldType::wood][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "´Á´ëÀÎ°£" );

	monster[Room::FieldType::swamp][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "µÎ²¨ºñ" );
	monster[Room::FieldType::swamp][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "¹ì" );
	monster[Room::FieldType::swamp][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "ÀúÁÖ¹ŞÀº ³ª¹«" );
	monster[Room::FieldType::swamp][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "½ÄÀÎ½Ä¹°" );
}

Monster* MonsterTable::CreateMonster(int fieldType)
{
	// 0: ¶¥, 1: ½£, 2: ´Ë, 3: º®, 4: Å»Ãâ±¸
	if (fieldType == 3 || fieldType == 4) return nullptr;
	return new Monster(monster[fieldType][rand() % 4]);
}

Monster MonsterTable::GetMonster(Room::FieldType fieldType)
{
	return monster[fieldType][rand() % 4];
}