#pragma once
#include "TextRender.h"

extern TextRender gTextRender;

class ShopItem
{
public:
	int gold;
	int heal;
	int qty;
	string name;
};

class Shop
{
public:
	int posX;
	int posY;
	string name;
	ShopItem items[5];

	Shop();
	~Shop();

	// 구매가 가능한가?
	bool IsBuyItem(int selectItemNo);
	const ShopItem& BuyItem(int selectItemNo);
};

