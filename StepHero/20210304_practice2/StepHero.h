#pragma once
#include "Dungeon.h"
#include "MonsterTable.h"
#include "TextRender.h"
#include "KeyManager.h"

extern TextRender gTextRender;
extern MonsterTable gMonsterTable;
extern KeyManager gKeyManager;

struct Difficulty
{
	int dungeonSize[3] = { 10, 15, 20 };
	int heroHP[3] = { 100, 90, 80 };
	float monsterStrength[3] = { 1, 1.5f, 2 };
	int monsterEncounter[3] = { 20, 10, 10 };
	string name[3] = { "EASY", "NORMAL", "HARD" };
};

struct BattleItem
{
	string name;
};

struct BattleInfo
{
	BattleItem item[3];

	void PrintBattleStatus(Hero& hero, Monster& monster)
	{
		string line;
		cout << "---------------------------------------------------------------------" << endl;
		cout << left << setw(38) << ("  [[ " + hero.GetName() + " ]]") << ("[[ " + monster.GetName() + " ]] ") << endl;
		cout << "    레벨\t: " << right << setw(10) << hero.GetLvl() << endl;
		cout << "    공격력\t: " << right << setw(10) << hero.GetAtk() << setw(20) << "공격력\t: " << setw(10) << monster.GetAtk() << endl;
		cout << "    체  력\t: " << right << setw(10) << string(to_string(hero.GetHp()) + " / " + to_string(hero.GetMaxHP())) << setw(20) << " 체  력\t: " << setw(10) << string(to_string(monster.GetHp()) + " / " + to_string(monster.GetMaxHP())) << endl;
		cout << "---------------------------------------------------------------------" << endl;
	}
};

class StepHero
{
public:

	enum GameState
	{
		TITLE_INPUT_DIFFI = 0, TITLE_INPUT_NAME, INGAME, INGAME_BATTLE, INGAME_SHOP, ENDING
	};

	GameState gameState = GameState::TITLE_INPUT_DIFFI;

	int selectDifficulty = 0;
	Difficulty difficulty;

	Hero* player = nullptr;
	Shop* shop = nullptr;
	Dungeon* dungeon = nullptr;

	StepHero();
	~StepHero();

	void Init();
	void Update();
	void Render();
	bool IsEnd();
	void Release();
};

