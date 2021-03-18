#pragma once
#include "Unit.h"
#include "TextRender.h"
#include "Shop.h"

extern TextRender gTextRender;

extern int G_MAX_LEVEL;
extern int G_LVL_EXP_TABLE[15][2];

class Hero  :  public Unit
{
public:
	enum HeroState { IDLE = 0, BATTLE, SHOP, DIE };

private:
	int posX;
	int posY;

	int gold;
	float sight;

public:

	int lRows;
	int lCols;
	int** lightMap = nullptr;

private:
	HeroState state = HeroState::IDLE;

public:
	Hero();
	Hero(int lvl, int maxHP, int atk, int gold, int exp, float sight, string name);
	~Hero();

	void SetPosition(int x, int y);
	int GetPosX();
	int GetPosY();
	int GetGold();
	float GetSight();

	int RootExp(int rootExp);

	int RootGold(int rootGold);

	int HitDamage(int damage);

	bool IsPossibleUseGold(int needGold);

	void UseItem(const ShopItem& item);

	void UseGold(int useGold);

	void ResetLightMap();

	void CreateLightMap(int rows, int cols);

	void PrintHeroStatus();

	void Move(int x, int y);

	void Render(TextLayout::LayoutKind layoutKind, TextLayout::LayoutPos layoutPos);
	void Release();
};

