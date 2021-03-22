#include "stdafx.h"
#include "Shop.h"

Shop::Shop()
{
}


Shop::~Shop()
{
}

void Shop::Init()
{
}

void Shop::Update()
{
}

void Shop::Render(TextRender& view, int hilightItemNo)
{
	int no = 0;
	string label;
	for (auto item : items)
	{
		string label = MakeString(TA_LEFT, 26, to_string(no + 1) + ". " + item.name + " (" + to_string(item.gold) + " GOLD)") + MakeString(TA_RIGHT, 14, "남은수량 : " + MakeString(TA_RIGHT, 3, to_string(item.qty)));
		if (no == hilightItemNo) label = ">>> " + MakeString(TA_LEFT, 40, label) + " <<< ";
		else label = MakeString(TA_LEFT, 40, label);
		view.Write(view.TL_CONTENT, MakeString(TA_CENTER, view.GetWidth(), label));
		++no;
	}
	label = to_string(no + 1) + ". 나가기";
	if (no == hilightItemNo) label = ">>> " + MakeString(TA_LEFT, 40, label) + " <<< ";
	else label = MakeString(TA_LEFT, 40, label);
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, view.GetWidth(), label));
}

void Shop::Release()
{
}

int Shop::GetItemGold(int selectItemNo)
{
	return items[selectItemNo].gold;
}

bool Shop::IsBuyItem(int selectItemNo)
{
	if (items[selectItemNo].qty > 0)
	{
		return true;
	}
	return false;
}

const ShopItem & Shop::BuyItem(int selectItemNo)
{
	--items[selectItemNo].qty;
	return items[selectItemNo];
}

int Shop::GetItemCount()
{
	return items.size();
}

void Shop::AddShopItem(ShopItem item)
{
	items.push_back(item);
}
