#pragma once
#include "Dungeon.h"
#include "MonsterTable.h"
#include "TextRender.h"
#include "KeyManager.h"

extern TextRender gTextRender;
extern MonsterTable gMonsterTable;
extern KeyManager gKeyManager;

struct Difficulty
{
	int dungeonSize[3] = { 10, 15, 20 };
	int heroHP[3] = { 100, 90, 80 };
	float monsterStrength[3] = { 1, 1.5f, 2 };
	int monsterEncounter[3] = { 20, 10, 10 };
	string name[3] = { "EASY", "NORMAL", "HARD" };
};

struct BattleLog
{
	int useItemA;
	int useItemB;
};

struct BattleItem
{
	int power;
	string name;
};

struct BattleInfo
{
	enum BattleState
	{
		READY, BATTLE, WIN, LOSE
	};

	BattleState state = BattleState::READY;

	Unit* unitA = nullptr;
	Unit* unitB = nullptr;

	int frontLog = 0;
	int rearLog = 0;
	BattleLog battleLog[100];
	BattleItem item[3];

	BattleInfo() {}
	BattleInfo(Unit& unitA, Unit& unitB) :unitA(&unitA), unitB(&unitB)
	{
		state = BattleState::READY;
		item[0] = { 0, "����" };
		item[1] = { 1, "����" };
		item[2] = { 2, " �� " };
	}

	void PushBack(int itemA, int itemB)
	{
		// ������ĭ�� ��������ʰ� �ִ밪������ Ȯ�� ���������� ���� ����
		if ((rearLog + 1) % 100 != frontLog)
			battleLog[rearLog++] = { itemA, itemB };
	}

	void Update()
	{
		if (state == BattleState::READY)
		{
			state = BattleState::BATTLE;
		}
		else if (state == BattleState::BATTLE)
		{
			// ��Ʋ
			int playerInput = rand() % 3;
			int monsterInput = rand() % 3;
				
			while (playerInput == monsterInput)
			{
				PushBack(playerInput, monsterInput);
				playerInput = rand() % 3;
				monsterInput = rand() % 3;
			}
			PushBack(playerInput, monsterInput);
			if (playerInput != (monsterInput + 1) % 3)
			{
				// �й�
				if (unitA->HitDamage(unitB->GetAtk()))
				{
					// ���Ϳ��� �׾���.
					// GAME_OVER�� �̵�

					state = BattleState::LOSE;
				}

				Monster* monster = (Monster*)unitB;
				gTextRender.InsertMessage(string(34, '='));
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("�� ��", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString(to_string(monster->GetAtk()) + " �� �������� �޾ҽ��ϴ�", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(string(34, '='));
			}
			else
			{
				// �¸�
				if (unitB->HitDamage(unitA->GetAtk()))
				{
					state = BattleState::WIN;

					// ���͸� �׿���.
					Monster* monster = (Monster*)unitB;
					gTextRender.InsertMessage(string(34, '='));
					gTextRender.InsertMessage(":: " + gTextRender.MakeString("�� ��", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString(to_string(monster->rootGold) + " ��� ȹ��", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString(to_string(monster->rootExp) + " ����ġ ȹ��", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(string(34, '='));
				}
			}
		}
	}

	void Render()
	{
		// ���ֻ��� ����
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, string(100, '-'));
		RenderBattleStatus();
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, string(100, '-'));

		// ��Ʋ�α�
		RenderBattleLog();
	}

	void RenderBattleStatus()
	{
		string unitStrA, unitStrB;

		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, "");
		unitStrA = gTextRender.MakeString(unitA->GetName(), 26, TextBuffer::TextAlign::CENTER);
		unitStrB = gTextRender.MakeString(unitB->GetName(), 26, TextBuffer::TextAlign::CENTER);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, unitStrA + string(10, ' ') + unitStrB);

		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, "");
		unitStrA = gTextRender.MakeString("��  �� :" + gTextRender.MakeString(to_string(unitA->GetLvl()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		unitStrB = gTextRender.MakeString("��  �� :" + gTextRender.MakeString(to_string(unitB->GetLvl()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, unitStrA + string(10, ' ') + unitStrB);

		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, "");
		unitStrA = gTextRender.MakeString("���ݷ� :" + gTextRender.MakeString(to_string(unitA->GetAtk()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		unitStrB = gTextRender.MakeString("���ݷ� :" + gTextRender.MakeString(to_string(unitB->GetAtk()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, unitStrA + string(10, ' ') + unitStrB);

		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, "");
		unitStrA = gTextRender.MakeString("ü  �� :" + gTextRender.MakeString(to_string(unitA->GetHp()) + " / " + to_string(unitA->GetMaxHP()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		unitStrB = gTextRender.MakeString("ü  �� :" + gTextRender.MakeString(to_string(unitB->GetHp()) + " / " + to_string(unitB->GetMaxHP()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, unitStrA + string(10, ' ') + unitStrB);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, "");
	}

	void RenderBattleLog()
	{
		if (frontLog != rearLog) gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "*** ����! ����! ��! ***");
		while (frontLog != rearLog)
		{
			gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "");
			string unitStrA = gTextRender.MakeString(unitA->GetName() + " [[ " + item[battleLog[frontLog].useItemA].name + " ]]", 30, TextBuffer::TextAlign::RIGHT);
			string unitStrB = gTextRender.MakeString("[[ " + item[battleLog[frontLog].useItemB].name + " ]] " + unitB->GetName(), 30, TextBuffer::TextAlign::LEFT);
			gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, unitStrA + " VS " + unitStrB);
			if (item[battleLog[frontLog].useItemA].power == item[battleLog[frontLog].useItemB].power)
			{
				// ���º�
				gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "*** �� �� �� ***");
			}
			else if (item[battleLog[frontLog].useItemA].power == (item[battleLog[frontLog].useItemB].power + 1) % 3 )
			{
				// �¸�
				gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "*** �� �� ***");
			}
			else
			{
				// �й�
				gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "*** �� �� ***");
			}
			frontLog = (frontLog + 1) % 100;
			gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "");
		}
	}
};

class StepHero
{
public:

	enum GameState
	{
		TITLE_LODING = 0,
		TITLE_INPUT_DIFFI,
		TITLE_INPUT_NAME,
		TITLE_INGAME_LODING,
		INGAME,
		INGAME_BATTLE_LODING,
		INGAME_BATTLE,
		BATTLE_INGAME_LODING,
		INGAME_SHOP,
		GAMEOVER,
		ENDING
	};

	GameState gameState = GameState::TITLE_LODING;

	int selectDifficulty = 0;
	Difficulty difficulty;

	Hero* player = nullptr;
	Shop* shop = nullptr;
	Dungeon* dungeon = nullptr;
	BattleInfo battleInfo;

	StepHero();
	~StepHero();

	void Init();
	void Update();
	void Render();
	bool IsEnd();
	void Release();
};

