#include "stdafx.h"
#include "Hero.h"


Hero::Hero()
{
}

Hero::Hero(int lvl, int maxHP, int atk, int gold, int exp, float sight, string name) :Unit(name, lvl, maxHP, atk, exp), sight(sight)
{
}


Hero::~Hero()
{
}

void Hero::SetPosition(int x, int y)
{
	posX = x;
	posY = y;
}

int Hero::GetPosX()
{
	return posX;
}

int Hero::GetPosY()
{
	return posY;
}

int Hero::GetGold()
{
	return gold;
}

float Hero::GetSight()
{
	return sight;
}

int Hero::RootExp(int rootExp)
{
	int lvlUp = 0;
	while (lvl < G_MAX_LEVEL && exp >= G_LVL_EXP_TABLE[lvl][1])
	{
		exp -= G_LVL_EXP_TABLE[lvl][1];
		++lvl;
		++lvlUp;

		// �������� Ǯ�� ȸ��
		hp = maxHP;
	}
	return lvlUp;
}

int Hero::RootGold(int rootGold)
{
	gold += rootGold;
	return gold;
}

int Hero::HitDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		state = HeroState::DIE;
	}

	return state;
}

bool Hero::IsPossibleUseGold(int needGold)
{
	if (gold >= needGold) return true;
	return false;
}

void Hero::UseItem(const ShopItem& item)
{
	hp += item.heal;
	if (hp > maxHP) hp = maxHP;
}

void Hero::UseGold(int useGold)
{
	gold -= useGold;
}

void Hero::ResetLightMap()
{
	for (int i = 0; i < lRows; ++i)
	{
		memset(lightMap[i], 0, sizeof(int) * lCols);
	}
}

void Hero::CreateLightMap(int rows, int cols)
{
	lRows = rows;
	lCols = cols;

	lightMap = new int*[rows];
	for (int i = 0; i < rows; ++i)
	{
		lightMap[i] = new int[cols] {0, };
	}
}

void Hero::PrintHeroStatus()
{
	cout << "---------------------------------------------------------------------" << endl;
	cout << "     [[ " << name << " ]]" << endl;
	cout << "     ����\t: " << right << setw(10) << lvl << endl;
	cout << "     ���ݷ�\t: " << right << setw(10) << atk << endl;
	cout << "     ü��\t: " << right << setw(10) << string(to_string(hp) + " / " + to_string(maxHP)) << endl;
	cout << "     ����ġ\t: " << right << setw(10) << exp << endl;
	cout << "     ������\t: " << right << setw(10) << gold << endl;
	cout << "---------------------------------------------------------------------" << endl;
}

void Hero::Move(int x, int y)
{
	posX = x;
	posY = y;
}

void Hero::Render(TextLayout::LayoutKind layoutKind, TextLayout::LayoutPos layoutPos)
{
	gTextRender.AppendBuffer(layoutKind, layoutPos, gTextRender.MakeString("[[ " + name + " ]]", 23, TextBuffer::TextAlign::CENTER, 5));
	gTextRender.AppendBuffer(layoutKind, layoutPos, string(5, ' ') + "��  �� :" + gTextRender.MakeString(to_string(lvl), 15, TextBuffer::TextAlign::RIGHT));
	gTextRender.AppendBuffer(layoutKind, layoutPos, string(5, ' ') + "���ݷ� :" + gTextRender.MakeString(to_string(atk), 15, TextBuffer::TextAlign::RIGHT));
	gTextRender.AppendBuffer(layoutKind, layoutPos, string(5, ' ') + "ü  �� :" + gTextRender.MakeString(string(to_string(hp) + " / " + to_string(maxHP)), 15, TextBuffer::TextAlign::RIGHT));
	gTextRender.AppendBuffer(layoutKind, layoutPos, string(5, ' ') + "����ġ :" + gTextRender.MakeString(to_string(exp), 15, TextBuffer::TextAlign::RIGHT));
	gTextRender.AppendBuffer(layoutKind, layoutPos, string(5, ' ') + "������ :" + gTextRender.MakeString(to_string(gold), 15, TextBuffer::TextAlign::RIGHT));
}

void Hero::Release()
{
	for (int r = 0; r < lRows; ++r)
	{
		delete[] lightMap[r];
	}
	delete[] lightMap;
}