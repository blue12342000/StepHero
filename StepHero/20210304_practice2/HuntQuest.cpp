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
		// ������ Monster���� ������ ������ ���� Class�� ����ȭ �Ŵٸ�
		// ���� ĳ������ ���ʿ� ���� typeid������ � ������ ���� ��Ҵ��� Ȯ���� ������
	}
}
