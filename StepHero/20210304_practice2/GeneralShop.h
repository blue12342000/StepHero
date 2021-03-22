#pragma once
#include "TextViewManager.h"
#include "TextView.h"
#include "KeyManager.h"
#include "MonsterTable.h"
#include "GameData.h"
#include "Shop.h"

extern TextViewManager gTextViewManager;
extern KeyManager gKeyManager;
extern GameData gGameData;

class GeneralShop : public TextView
{
private:
	enum ShopState
	{
		INIT,
		READY,
		BUY_SUCCESS,
		BUY_FAIL_GOLD,
		BUY_FAIL_QTY,
		END
	};

private:
	int selectItemNo = 0;
	ShopState state = ShopState::INIT;
	Hero* player = nullptr;
	Shop shop;

private:
	GeneralShop();

public:
	GeneralShop(int width, int height);
	~GeneralShop();

	void Init();
	void Update();
	void Render(vector<string>* targetBuffer = nullptr);
	void Release();
};

