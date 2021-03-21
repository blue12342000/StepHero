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

Hero::HeroState Hero::GetState()
{
	return state;
}

void Hero::SetState(Hero::HeroState state)
{
	this->state = state;
}

int Hero::RootExp(int rootExp)
{
	int lvlUp = 0;
	while (lvl < G_MAX_LEVEL && exp >= G_LVL_EXP_TABLE[lvl][1])
	{
		exp -= G_LVL_EXP_TABLE[lvl][1];
		++lvl;
		++lvlUp;

		// 레벨업시 풀피 회복
		hp = maxHP;
	}
	return lvlUp;
}

int Hero::RootGold(int rootGold)
{
	gold += rootGold;
	return gold;
}

bool Hero::HitDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		state = HeroState::DIE;
	}

	return state == HeroState::DIE;
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

void Hero::Move(int x, int y)
{
	posX = x;
	posY = y;
}

void Hero::Render(TextRender& view)
{
	view.Write(view.TL_BOTTOM, string("     " + MakeString(TA_CENTER, 23, "[[ " + name + " ]]")));
	view.Write(view.TL_BOTTOM, string("     레  벨 :" + MakeString(TA_RIGHT, 15, to_string(lvl))));
	view.Write(view.TL_BOTTOM, string("     공격력 :" + MakeString(TA_RIGHT, 15, to_string(atk))));
	view.Write(view.TL_BOTTOM, string("     체  력 :" + MakeString(TA_RIGHT, 15, string(to_string(hp) + " / " + to_string(maxHP)))));
	view.Write(view.TL_BOTTOM, string("     경험치 :" + MakeString(TA_RIGHT, 15, to_string(exp))));
	view.Write(view.TL_BOTTOM, string("     소지금 :" + MakeString(TA_RIGHT, 15, to_string(gold))));
}

void Hero::Release()
{
	for (int r = 0; r < lRows; ++r)
	{
		delete[] lightMap[r];
	}
	delete[] lightMap;
}