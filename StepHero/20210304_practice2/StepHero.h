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
	StepHero();
	~StepHero();

	void Init();
	void Update();
	void Render();
	bool IsEnd();
	void Release();
};

