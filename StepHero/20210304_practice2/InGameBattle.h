#pragma once
#include "TextViewManager.h"
#include "TextView.h"
#include "KeyManager.h"
#include "MonsterTable.h"
#include "GameData.h"
#include "QuestManager.h"

extern TextViewManager gTextViewManager;
extern KeyManager gKeyManager;
extern MonsterTable gMonsterTable;
extern Difficulty difficulty;
extern GameData gGameData;
extern QuestManager gQuestManager;

class InGameBattle : public TextView
{
private:
	enum BattleState
	{
		BS_INIT,
		BS_READY,
		BS_BATTLE_START,
		BS_BATTLE_END,
		BS_BATTLE_RESULT,
		BS_CYCLE_END,
		BS_CHOOSE_SHOP,
		BS_NONE
	};

	enum BattleResult
	{
		BR_PLAYER_WIN,
		BR_ENEMY_WIN,
		BR_NONE
	};

	struct BattleLog
	{
		int useItemA;
		int useItemB;
	};

	BattleState state = BattleState::BS_NONE;
	BattleResult result = BattleResult::BR_NONE;

	bool selectShopEnterYN = true;

	Hero* player = nullptr;
	Monster monster;
	vector<BattleLog> battleLog;
	vector<string> battleItemName;

private:
	InGameBattle();

	void RenderBattleStatus();

public:
	InGameBattle(int width, int height);
	~InGameBattle();

	void Init();
	void Update();
	void Render(vector<string>* targetBuffer = nullptr);
	void Release();
};

