#pragma once
#include "Dungeon.h"
#include "MonsterTable.h"
#include "KeyManager.h"
#include "TextViewManager.h"
#include "GameTitle.h"
#include "InGameDungeon.h"
#include "InGameBattle.h"

extern MonsterTable gMonsterTable;
extern KeyManager gKeyManager;
extern TextViewManager gTextViewManager;

class StepHero
{
public:

	enum GameState
	{
		TITLE_LODING = 0,
		TITLE_INPUT_DIFFI,
		TITLE_INPUT_NAME,
		TITLE_INGAME_LODING,
		INGAME,
		INGAME_BATTLE_LODING,
		INGAME_BATTLE,
		BATTLE_INGAME_LODING,
		INGAME_SHOP,
		GAMEOVER,
		ENDING
	};

	GameState gameState = GameState::TITLE_LODING;

	StepHero();
	~StepHero();

	void Init();
	void Update();
	void Render();
	bool IsEnd();
	void Release();
};

