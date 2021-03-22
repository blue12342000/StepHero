#pragma once
#include "TextRender.h"

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
private:
	vector<ShopItem> items;

public:
	Shop();
	~Shop();

	void Init();
	void Update();
	void Render(TextRender& view, int hilightItemNo = -1);
	void Release();

	// 구매가 가능한가?
	int GetItemGold(int selectItemNo);
	bool IsBuyItem(int selectItemNo);
	const ShopItem& BuyItem(int selectItemNo);
	int GetItemCount();

	void AddShopItem(ShopItem item);
};

