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
	monster[Room::FieldType::empty][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "�δ���");
	monster[Room::FieldType::empty][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "������" );
	monster[Room::FieldType::empty][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "����" );
	monster[Room::FieldType::empty][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "����" );

	monster[Room::FieldType::wood][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "�䳢" );
	monster[Room::FieldType::wood][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "�罿" );
	monster[Room::FieldType::wood][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "����" );
	monster[Room::FieldType::wood][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "�����ΰ�" );

	monster[Room::FieldType::swamp][0] = Monster(1, 10 * strength, 10 * strength, 10 * strength, 20, 30, "�β���" );
	monster[Room::FieldType::swamp][1] = Monster(1, 12 * strength, 20 * strength, 20 * strength, 25, 50, "��" );
	monster[Room::FieldType::swamp][2] = Monster(1, 14 * strength, 30 * strength, 30 * strength, 30, 80, "���ֹ��� ����" );
	monster[Room::FieldType::swamp][3] = Monster(1, 16 * strength, 40 * strength, 40 * strength, 35, 100, "���νĹ�" );
}

Monster* MonsterTable::CreateMonster(int fieldType)
{
	// 0: ��, 1: ��, 2: ��, 3: ��, 4: Ż�ⱸ
	if (fieldType == 3 || fieldType == 4) return nullptr;
	return new Monster(monster[fieldType][rand() % 4]);
}

Monster MonsterTable::GetMonster(Room::FieldType fieldType)
{
	return monster[fieldType][rand() % 4];
}