#pragma once
#include "Hero.h"

class PlayerManager
{
private:
	Hero* hero = nullptr;

public:
	Hero& GetHero();
	Hero* GetHeroPtr();

	void Save();
	void Load();
};

