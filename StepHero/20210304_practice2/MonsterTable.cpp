#include "stdafx.h"
#include "MonsterTable.h"

MonsterTable::MonsterTable()
{
}


MonsterTable::~MonsterTable()
{
}

void MonsterTable::Init(int strength)
{
	monster[Room::FieldType::FT_EMPTY][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "두더지");
	monster[Room::FieldType::FT_EMPTY][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "슬라임" );
	monster[Room::FieldType::FT_EMPTY][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "도적" );
	monster[Room::FieldType::FT_EMPTY][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "산적" );

	monster[Room::FieldType::FT_WOOD][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "토끼" );
	monster[Room::FieldType::FT_WOOD][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "사슴" );
	monster[Room::FieldType::FT_WOOD][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "늑대" );
	monster[Room::FieldType::FT_WOOD][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "늑대인간" );

	monster[Room::FieldType::FT_SWAMP][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "두꺼비" );
	monster[Room::FieldType::FT_SWAMP][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "뱀" );
	monster[Room::FieldType::FT_SWAMP][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "저주받은 나무" );
	monster[Room::FieldType::FT_SWAMP][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "식인식물" );
}

Monster MonsterTable::GetMonster(Room::FieldType fieldType)
{
	return monster[fieldType][rand() % 4];
}