#pragma once
#include "Quest.h"
#include "Monster.h"

class HuntQuest : public Quest
{
private:
	int needCount;
	int currCount;

private:
	HuntQuest();

public:
	HuntQuest(int needCount, int id, string name, string desc, QuestReward completeReward, QuestReward failedReward);
	~HuntQuest();

	void Progress(void* questParam) override;
};

