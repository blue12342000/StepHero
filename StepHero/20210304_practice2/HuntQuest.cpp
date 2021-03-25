#include "stdafx.h"
#include "HuntQuest.h"


HuntQuest::HuntQuest()
{
}

HuntQuest::HuntQuest(int id, string name, string desc, Command complete, Command failed) :HuntQuest(id, name, desc, complete, failed)
{
	type = Quest::QusetType::QT_HUNT;
}

HuntQuest::~HuntQuest()
{
}

void HuntQuest::Progress(Target target)
{
	if (typeid(Monster) == typeid(target))
	{
		// 지금은 Monster지만 몬스터의 종류에 따른 Class로 세분화 신다면
		// 굳이 캐스팅을 할필요 없이 typeid만으로 어떤 종류의 몹을 잡았는지 확인이 가능함
	}
}
