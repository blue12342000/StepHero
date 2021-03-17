#pragma once
class Unit
{
protected:
	int lvl;
	int maxHP;
	int hp;
	int atk;
	int exp;

	string name;
public:
	Unit();
	Unit(string name, int lvl, int hp, int atk, int exp);
	~Unit();

	int GetLvl();
	int GetMaxHP();
	int GetHp();
	int GetAtk();
	int GetExp();
	string GetName();

	virtual int HitDamage(int damage) = 0;
};

