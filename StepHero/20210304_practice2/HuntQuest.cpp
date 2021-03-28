#include "stdafx.h"
#include "HuntQuest.h"


HuntQuest::HuntQuest()
{
}

HuntQuest::HuntQuest(int needCount, int id, string name, string desc, QuestReward completeReward, QuestReward failedReward) :Quest(id, name, desc, completeRewards, failedRewards), needCount(needCount)
{
	type = Quest::QusetType::QT_HUNT;
	currCount = 0;
}

HuntQuest::~HuntQuest()
{
}

void HuntQuest::Progress(void* questParam)
{
	GameObject* object = (GameObject*)questParam;
	if (typeid(Monster) == typeid(*object))
	{
		// ������ Monster���� ������ ������ ���� Class�� ����ȭ �Ŵٸ�
		// ���� ĳ������ ���ʿ� ���� typeid������ � ������ ���� ��Ҵ��� Ȯ���� ������
		++currCount;
	}
}
