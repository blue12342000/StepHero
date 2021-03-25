#include "stdafx.h"
#include "Quest.h"

Quest::Quest()
{
}

Quest::Quest(int id, string name, string desc, Command complete, Command failed):id(id),name(name),desc(desc), completeCmd(complete), failedCmd(failed)
{
}

Quest::~Quest()
{
}

int Quest::GetId()
{
	return id;
}

string Quest::GetName()
{
	return name;
}

string Quest::GetDesc()
{
	return desc;
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
	completeCmd.Excute();
}

void Quest::Failed()
{
	failedCmd.Excute();
}
