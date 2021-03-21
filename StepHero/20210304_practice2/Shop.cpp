#include "stdafx.h"
#include "Shop.h"

Shop::Shop()
{
}


Shop::~Shop()
{
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