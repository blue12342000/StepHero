#pragma once
#include "Hero.h"
#include "Dungeon.h"

class GameData
{
private:
	// 선택한 난이도
	int difficalty = 0;
	Hero* hero = nullptr;
	Dungeon* dungeon = nullptr;

public:
	void SetHero(Hero& hero) { this->hero = &hero; }
	Hero& GetHero() { return *hero; }
	Hero* GetHeroPtr() { return hero; }

	void SetDungeon(Dungeon& dungeon) { this->dungeon = &dungeon; }
	Dungeon& GetDungeon() { return *dungeon; }
	Dungeon* GetDungeonPtr() { return dungeon; }

	void SetDifficulty(int difficulty) { this->difficalty; }
	int GetDifficulty() { return difficalty; }

	void Save() {}
	void Load() {}
};

