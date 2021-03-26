#pragma once
#include "Quest.h"

class Hero;
class Monster;
class QuestManager
{
private:
	map<int, shared_ptr<Quest>> questMap;

public:
	QuestManager();
	~QuestManager();

	shared_ptr<Quest> GetQuest(int id) { return questMap[id]; }

	void AddQuest(Quest* quest);
	void Progress(Quest::QusetType questType, void* target);
};

