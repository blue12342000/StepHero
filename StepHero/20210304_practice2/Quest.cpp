#include "stdafx.h"
#include "Quest.h"

Quest::Quest()
{
}

Quest::Quest(int id, string name, string desc, QuestReward completeReward, QuestReward failedReward):id(id),name(name),desc(desc), completeReward(completeReward), failedReward(failedReward), isComplete(false)
{
}

Quest::~Quest()
{
}

shared_ptr<Quest> Quest::GetNextQuest()
{
	return next;
}

shared_ptr<Quest> Quest::GetPreQuset()
{
	return pre;
}

void Quest::Complete()
{
	//completeCmd.Excute();
}

void Quest::Failed()
{
	//failedCmd.Excute();
}
