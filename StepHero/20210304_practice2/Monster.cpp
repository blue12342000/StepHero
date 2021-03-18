#include "stdafx.h"
#include "Monster.h"


Monster::Monster()
{
}

Monster::Monster(int lvl, int maxHP, int atk, int exp, int rootExp, int rootGold, string name) :Unit(name, lvl, maxHP, atk, exp), rootExp(rootExp), rootGold(rootGold)
{
}


Monster::~Monster()
{
}

bool Monster::HitDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		state = 1;
	}
	return (state == 1);
}