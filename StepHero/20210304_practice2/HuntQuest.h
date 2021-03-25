#pragma once
#include "Quest.h"
#include "Monster.h"

class HuntQuest : public Quest
{
private:

private:
	HuntQuest();

public:
	HuntQuest(int id, string name, string desc, Command complete, Command failed);
	~HuntQuest();

	void Progress(Target target) override;
};

