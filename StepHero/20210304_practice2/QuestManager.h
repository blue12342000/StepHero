#pragma once
#include "Quest.h"

class QuestManager
{
private:
	map<int, shared_ptr<Quest>> questMap;

public:
	QuestManager();
	~QuestManager();

	shared_ptr<Quest> GetQuest(int id);

	void AddQuset(shared_ptr<Quest> quest);
};

