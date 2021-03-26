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
	// ARGS TYPE : BOOL, INT, FLOAT, 
	AddQuest(new HuntQuest(3, 1, "몹을 3마리 잡아라!", "필드에 있는 몬스터를 해치워라", { [](Target target, TargetArgs args) { ((Hero*)target)->RootExp((int)args[0]); ((Hero*)target)->RootGold((int)args[1]); } }, { [](Target, TargetArgs) {} }));
}

QuestManager::~QuestManager()
{
}

void QuestManager::AddQuest(Quest* quest)
{
	questMap.insert(make_pair(quest->GetId(), quest));
}

void QuestManager::Progress(Quest::QusetType questType, void* target)
{
	if (typeid(target) == typeid(QuestManager))
	{
		int a = 0;
	}

	bool aa = typeid(target) == typeid(QuestManager);
	bool bb = typeid(target).raw_name() == typeid(QuestManager).raw_name();

	int b = 0;
}
