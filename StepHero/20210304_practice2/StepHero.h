#pragma once
#include "TextViewManager.h"
#include "GameTitle.h"
#include "InGameDungeon.h"
#include "InGameBattle.h"
#include "GeneralShop.h"
#include "GameOver.h"
#include "GameEnding.h"
#include "GameEscape.h"

extern TextViewManager gTextViewManager;

class StepHero
{
public:

	enum GameState
	{
		INIT = 0,
		TITLE_INPUT_DIFFI,
		TITLE_INPUT_NAME,
		INGAME,
		INGAME_BATTLE,
		INGAME_SHOP,
		GAMEOVER,
		ENDING
	};

	GameState gameState = GameState::INIT;

	StepHero();
	~StepHero();

	void Init();
	void Update();
	void Render();
	bool IsEnd();
	void Release();
};

