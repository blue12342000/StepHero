#include "stdafx.h"
#include "Unit.h"


Unit::Unit()
{
}

Unit::Unit(string name, int lvl, int hp, int atk, int exp) :name(name), lvl(lvl), maxHP(hp), hp(hp), atk(atk), exp(exp)
{
}

Unit::~Unit()
{
}

int Unit::GetLvl()
{
	return lvl;
}

int Unit::GetMaxHP()
{
	return maxHP;
}

int Unit::GetHp()
{
	return hp;
}

int Unit::GetAtk()
{
	return atk;
}

int Unit::GetExp()
{
	return exp;
}

string Unit::GetName()
{
	return name;
}
