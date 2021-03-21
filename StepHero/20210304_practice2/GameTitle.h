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

class GameTitle : public TextView
{
private:
	enum GameState
	{
		GS_SELECT_DIFFI, GS_INPUT_NAME
	};

	GameState gameState = GameState::GS_SELECT_DIFFI;
	int selectDifficulty = 0;

private:
	GameTitle();

public:
	GameTitle(int width, int height);
	~GameTitle();

	void Init() override;
	void Update() override;
	void Render(vector<string>* targetBuffer = nullptr) override;
	void Release() override;
};

