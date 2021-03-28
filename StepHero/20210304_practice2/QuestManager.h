#pragma once
#include "Quest.h"

class Hero;
class Monster;
class QuestManager
{
private:
	map<int, Quest*> questMap;
	map<Quest::QusetType, vector<Quest*>> questGroup;

public:
	QuestManager();
	~QuestManager();

	Quest* GetQuest(int id) { return questMap[id]; }

	void AddQuest(Quest* quest);
	void Progress(Quest::QusetType questType, void* questParam);
};

