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
	monster[Room::FieldType::FT_EMPTY][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "�δ���");
	monster[Room::FieldType::FT_EMPTY][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "������" );
	monster[Room::FieldType::FT_EMPTY][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "����" );
	monster[Room::FieldType::FT_EMPTY][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "����" );

	monster[Room::FieldType::FT_WOOD][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "�䳢" );
	monster[Room::FieldType::FT_WOOD][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "�罿" );
	monster[Room::FieldType::FT_WOOD][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "����" );
	monster[Room::FieldType::FT_WOOD][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "�����ΰ�" );

	monster[Room::FieldType::FT_SWAMP][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "�β���" );
	monster[Room::FieldType::FT_SWAMP][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "��" );
	monster[Room::FieldType::FT_SWAMP][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "���ֹ��� ����" );
	monster[Room::FieldType::FT_SWAMP][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "���νĹ�" );
}

Monster MonsterTable::GetMonster(Room::FieldType fieldType)
{
	return monster[fieldType][rand() % 4];
}