#include "stdafx.h"
#include "QuestManager.h"


QuestManager::QuestManager()
{
}

QuestManager::~QuestManager()
{
}

void QuestManager::AddQuset(shared_ptr<Quest> quest)
{
	questMap.insert(make_pair(quest->GetId(), quest));
}
