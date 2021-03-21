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

class InGameDungeon : public TextView
{
private:
	Hero* player;
	Dungeon* dungeon;

private:
	InGameDungeon();

public:
	InGameDungeon(int width, int height);
	~InGameDungeon();

	void Init() override;
	void Update() override;
	void Render(vector<string>* targetBuffer = nullptr) override;
	void Release() override;
};

