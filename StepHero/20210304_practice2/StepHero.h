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
	bool isBattleEnd = false;
	bool isWinnerA = false;
	Unit* unitA = nullptr;
	Unit* unitB = nullptr;

	int frontLog = 0;
	int rearLog = 0;
	BattleLog battleLog[100];
	BattleItem item[3];

	BattleInfo() {}
	BattleInfo(Unit& unitA, Unit& unitB) :unitA(&unitA), unitB(&unitB)
	{
		isBattleEnd = false;
		item[0] = { 0, "가위" };
		item[1] = { 1, "바위" };
		item[2] = { 2, " 보 " };
	}

	void PushBack(int itemA, int itemB)
	{
		// 마지막칸은 사용하지않고 최대값인지만 확인 다차있으면 넣지 않음
		if ((rearLog + 1) % 100 != frontLog)
			battleLog[rearLog++] = { itemA, itemB };
	}

	void Update()
	{
		if (!isBattleEnd)
		{
			// 배틀
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
				// 패배
				isBattleEnd = true;
				isWinnerA = false;

				if (unitA->HitDamage(unitB->GetAtk()))
				{
					// 몬스터에게 죽었다.
					// GAME_OVER로 이동
				}

				Monster* monster = (Monster*)unitB;
				gTextRender.InsertMessage(string(34, '='));
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("패 배", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString(to_string(monster->GetAtk()) + " 의 데미지를 받았습니다", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
				gTextRender.InsertMessage(string(34, '='));
			}
			else
			{
				// 승리
				isWinnerA = true;

				if (unitB->HitDamage(unitA->GetAtk()))
				{
					// 몬스터를 죽였다.
					Monster* monster = (Monster*)unitB;
					gTextRender.InsertMessage(string(34, '='));
					gTextRender.InsertMessage(":: " + gTextRender.MakeString("승 리", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString(to_string(monster->rootGold) + " 골드 획득", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString(to_string(monster->rootExp) + " 경험치 획득", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(":: " + gTextRender.MakeString("", 28, TextBuffer::TextAlign::CENTER) + " ::");
					gTextRender.InsertMessage(string(34, '='));
				}
			}
		}
		else
		{
			// 배틀이 끝난 후 다음 프레임에 실행
		}
	}

	void Render()
	{
		// 유닛상태 갱신
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, string(100, '-'));
		RenderBattleStatus();
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, string(100, '-'));

		// 배틀로그
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
		unitStrA = gTextRender.MakeString("레  벨 :" + gTextRender.MakeString(to_string(unitA->GetLvl()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		unitStrB = gTextRender.MakeString("레  벨 :" + gTextRender.MakeString(to_string(unitB->GetLvl()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, unitStrA + string(10, ' ') + unitStrB);

		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, "");
		unitStrA = gTextRender.MakeString("공격력 :" + gTextRender.MakeString(to_string(unitA->GetAtk()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		unitStrB = gTextRender.MakeString("공격력 :" + gTextRender.MakeString(to_string(unitB->GetAtk()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, unitStrA + string(10, ' ') + unitStrB);

		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, "");
		unitStrA = gTextRender.MakeString("체  력 :" + gTextRender.MakeString(to_string(unitA->GetHp()) + " / " + to_string(unitA->GetMaxHP()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		unitStrB = gTextRender.MakeString("체  력 :" + gTextRender.MakeString(to_string(unitB->GetHp()) + " / " + to_string(unitB->GetMaxHP()), 18, TextBuffer::TextAlign::RIGHT), 26, TextBuffer::TextAlign::LEFT);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, unitStrA + string(10, ' ') + unitStrB);
		gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::CONTENT, "");
	}

	void RenderBattleLog()
	{
		if (frontLog != rearLog) gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "*** 가위! 바위! 보! ***");
		while (frontLog != rearLog)
		{
			gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "");
			string unitStrA = gTextRender.MakeString(unitA->GetName() + " [[ " + item[battleLog[frontLog].useItemA].name + " ]]", 30, TextBuffer::TextAlign::RIGHT);
			string unitStrB = gTextRender.MakeString("[[ " + item[battleLog[frontLog].useItemB].name + " ]] " + unitB->GetName(), 30, TextBuffer::TextAlign::LEFT);
			gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, unitStrA + " VS " + unitStrB);
			if (item[battleLog[frontLog].useItemA].power == item[battleLog[frontLog].useItemB].power)
			{
				// 무승부
				gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "*** 무 승 부 ***");
			}
			else if (item[battleLog[frontLog].useItemA].power == (item[battleLog[frontLog].useItemB].power + 1) % 3 )
			{
				// 승리
				gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "*** 승 리 ***");
			}
			else
			{
				// 패배
				gTextRender.AppendBuffer(TextLayout::LayoutKind::BATTLE, TextLayout::LayoutPos::BOTTOM, "*** 패 배 ***");
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
		TITLE_LODING = 0, TITLE_INPUT_DIFFI, TITLE_INPUT_NAME, TITLE_INGAME_LODING, INGAME, INGAME_BATTLE_LODING, INGAME_BATTLE, INGAME_SHOP, ENDING
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

