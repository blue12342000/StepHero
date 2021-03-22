#pragma once
#include "TextRender.h"
#include "Unit.h"

extern TextRender gTextRender;

class Monster : public Unit
{
private:
	enum MonsterState
	{
		ALIVE,
		DEAD
	};

public:
	int rootExp;
	int rootGold;

	// 0: LIVE, 1: DIE
	int state = 0;

	// 추후 사용
	//Room* loc = nullptr;

	Monster();
	Monster(int lvl, int maxHP, int atk, int exp, int rootExp, int rootGold, string name);
	~Monster();

	bool HitDamage(int damage);
};

