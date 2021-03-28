#include "stdafx.h"
#include "QuestManager.h"
#include "HuntQuest.h"
#include "Hero.h"
#include "Monster.h"

QuestManager::QuestManager()
{
	// ID, NAME, DESC
	// HERO : 플레이어블 캐릭터의 번호
	// MONSTER : 몬스터의 번호 | 0x10
	// ARGS TYPE : BOOL, INT, FLOAT
	//
	HuntQuest* huntQuest = new HuntQuest(1, 1, "몹을 1마리 잡아라!", "필드에 있는 몬스터를 해치워라", {{ Quest::RewardType::RT_SUCCESS_GOLD, Quest::RewardType::RT_SUCCESS_EXP }, {100, 20}}, {});
	AddQuest(huntQuest);
}

QuestManager::~QuestManager()
{
}

void QuestManager::AddQuest(Quest* quest)
{
	questMap.insert(make_pair(quest->GetId(), quest));
}

void QuestManager::Progress(Quest::QusetType questType, void* questParam)
{
	vector<Quest*>& group = questGroup[questType];
	for (auto it = group.begin(); it != group.end(); ++it)
	{
		(*it)->Progress(questParam);
	}
}
