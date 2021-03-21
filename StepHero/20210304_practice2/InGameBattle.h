#pragma once
#include "TextViewManager.h"
#include "TextView.h"
#include "KeyManager.h"
#include "MonsterTable.h"
#include "GameData.h"

extern TextViewManager gTextViewManager;
extern KeyManager gKeyManager;
extern MonsterTable gMonsterTable;
extern Difficulty difficulty;
extern GameData gGameData;

class InGameBattle : public TextView
{
private:
	enum BattleState
	{
		BS_INIT,
		BS_READY,
		BS_BATTLE,
		BS_BATTLE_RESULT,
		BS_WIN,
		BS_LOSE,
		BS_NONE
	};

	struct BattleLog
	{
		int useItemA;
		int useItemB;
	};

	BattleState state = BattleState::BS_NONE;

	Hero* player = nullptr;
	Monster* monster = nullptr;
	vector<BattleLog> battleLog;
	vector<string> battleItemName;

private:
	InGameBattle();

public:
	InGameBattle(int width, int height);
	~InGameBattle();

	void Init();
	void Update();
	void Render(vector<string>* targetBuffer = nullptr);
	void Release();
};

