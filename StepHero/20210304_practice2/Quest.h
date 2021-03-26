#pragma once

// 퀘스트를 생각했을때
// 퀘스트의 성공/실패했을때의 동작을 퀘스트가 갖고 있어야 하는것으로 생각을 하였다.
// 하지만 서버가 있는 온라인 게임이라면 퀘스트 결과를 보내주고 메니저에서 패킷처리를 하여
// 해당 퀘스트의 함수를 호출 하는 방식으로도 진행이 된다면
// 해당 콜백을 퀘스트가 아닌 메니저가 갖고 있어도 되는 부분이 아닐까? 하는 생각이 들고있다.
// 그렇게 될경우 메니저는 퀘스트의 수만큼의 콜백함수와 함수에 따른 switch case 가 생성되는게 아닐까?
// 
class Quest
{
public:
	enum class QusetType
	{
		QT_HUNT, QT_MOVE, QT_NONE
	};

protected:
	int id;
	QusetType type = QusetType::QT_NONE;
	string name;
	string desc;

	Command completeCmd;
	Command failedCmd;

	// 연계 퀘스트 있을때만
	shared_ptr<Quest> next = nullptr;
	shared_ptr<Quest> pre = nullptr;

protected:
	Quest();
	Quest(int id, string name, string desc, Command complete, Command failed);

public:
	~Quest();

	inline int GetId() { return id; }
	inline string GetName() { return name; }
	inline string GetDesc() { return desc; }

	shared_ptr<Quest> GetNextQuest();
	shared_ptr<Quest> GetPreQuset();

	void Complete();
	void Failed();

	virtual void Progress(Target target) = 0;
};

