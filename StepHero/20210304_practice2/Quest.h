#pragma once

// ����Ʈ�� ����������
// ����Ʈ�� ����/������������ ������ ����Ʈ�� ���� �־�� �ϴ°����� ������ �Ͽ���.
// ������ ������ �ִ� �¶��� �����̶�� ����Ʈ ����� �����ְ� �޴������� ��Ŷó���� �Ͽ�
// �ش� ����Ʈ�� �Լ��� ȣ�� �ϴ� ������ε� ������ �ȴٸ�
// �ش� �ݹ��� ����Ʈ�� �ƴ� �޴����� ���� �־ �Ǵ� �κ��� �ƴұ�? �ϴ� ������ ����ִ�.
// �׷��� �ɰ�� �޴����� ����Ʈ�� ����ŭ�� �ݹ��Լ��� �Լ��� ���� switch case �� �����Ǵ°� �ƴұ�?
// 
class Quest
{
public:
	enum class QusetType
	{
		QT_HUNT, QT_MOVE, QT_NONE
	};

	enum class RewardType
	{
		RT_SUCCESS_GOLD, RT_SUCCESS_EXP, RT_FAILED_DAMAGE
	};

	struct QuestReward
	{
		vector<RewardType> rewards;
		vector<int> args;
	};

protected:
	bool isComplete;

	int id;
	QusetType type = QusetType::QT_NONE;
	string name;
	string desc;

	QuestReward completeReward;
	QuestReward failedReward;

	// ���� ����Ʈ ��������
	shared_ptr<Quest> next = nullptr;
	shared_ptr<Quest> pre = nullptr;

protected:
	Quest();
	Quest(int id, string name, string desc, QuestReward completeReward, QuestReward failedReward);

public:
	~Quest();

	inline int GetId() { return id; }
	inline string GetName() { return name; }
	inline string GetDesc() { return desc; }

	shared_ptr<Quest> GetNextQuest();
	shared_ptr<Quest> GetPreQuset();

	void Complete();
	void Failed();

	virtual void Progress(void* questParam) = 0;
};

