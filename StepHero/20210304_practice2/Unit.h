#pragma once
#include "GameObject.h"

class Unit : public GameObject
{
protected:
	int lvl;
	int maxHP;
	int hp;
	int atk;
	int exp;

	string name;

protected:
	Unit() {}

public:
	Unit(string name, int lvl, int hp, int atk, int exp):lvl(lvl), maxHP(hp), hp(hp), atk(atk), exp(exp), name(name) {}
	virtual ~Unit() {}

	int GetLvl() { return lvl; }
	int GetMaxHP() { return maxHP; }
	int GetHp() { return hp; }
	int GetAtk() { return atk; }
	int GetExp() { return exp; }
	string GetName() { return name; }

	virtual bool HitDamage(int damage) { return false; }
	virtual bool IsAlive() { return hp > 0; }
};

