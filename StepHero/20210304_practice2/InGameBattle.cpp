#include "stdafx.h"
#include "InGameBattle.h"


InGameBattle::InGameBattle()
{
}

InGameBattle::InGameBattle(int width, int height)
{
	this->width = width;
	this->height = height;
	view.Init(width, height);
	view.AddLayout(view.TL_TOP, view.RT_STATIC, 0, 8);
	view.AddLayout(view.TL_CONTENT, view.RT_EXIST_DATA, 8, 12);
	view.AddLayout(view.TL_BOTTOM, view.RT_EVERYTIME, 20, 30);

	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      #####  #####  #####  #####      #   #  #####  #####  #####     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      #        #    #      #   #      #   #  #      #   #  #   #     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      #####    #    ###    #####      #####  ###    #####  #   #     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "          #    #    #      #          #   #  #      # #    #   #     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "          #    #    #      #          #   #  #      #  #   #   #     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      #####    #    #####  #          #   #  #####  #   #  #####     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));

	battleLog.reserve(height * 3);
	battleItemName = { "����", "����", " �� " };
}

InGameBattle::~InGameBattle()
{
}

void InGameBattle::Init()
{
	player = gGameData.GetHeroPtr();
	monster = gMonsterTable.GetMonster(gGameData.GetDungeon().GetRoomFieldType(player->GetPosX(), player->GetPosY()));

	state = BattleState::BS_INIT;
}

void InGameBattle::Update()
{
	if (state == BattleState::BS_INIT)
	{
		state = BattleState::BS_READY;
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
	else if (state == BattleState::BS_READY)
	{
		state = BattleState::BS_BATTLE;
	}
	else if (state == BattleState::BS_BATTLE)
	{
		// ��Ʋ
		int playerInput = rand() % 3;
		int monsterInput = rand() % 3;

		while (playerInput == monsterInput)
		{
			battleLog.push_back({ playerInput, monsterInput });
			playerInput = rand() % 3;
			monsterInput = rand() % 3;
		}
		battleLog.push_back({ playerInput, monsterInput });
		state = BattleState::BS_BATTLE_RESULT;
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
	else if (state == BattleState::BS_BATTLE_RESULT)
	{
		// ��� ��� �� ��� �α�
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
	else if (state == BattleState::BS_WIN)
	{
		// ���Ͱ� �׾����� Ȯ��
	}
	else if (state == BattleState::BS_LOSE)
	{
		// �÷��̾ �׾����� Ȯ��
		gKeyManager.Clear();
		gTextViewManager.ChangeView(gTextViewManager.VT_INGAME, gTextViewManager.AT_FADE_OUT_IN, 1000);
	}
}

void InGameBattle::Render(vector<string>* targetBuffer)
{
	if (state == BattleState::BS_INIT || state == BattleState::BS_READY)
	{
		view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, string(80, '-')));
		view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, string(80, '-')));

		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** " + monster->GetName() + "�� �����Ͽ����ϴ�. ***"));
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: < ��� > �ƹ�Ű�� �����ּ���...");
	}
	else if (state == BattleState::BS_BATTLE_RESULT)
	{
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** ����! ����! ��! ***"));
		for (auto it = battleLog.begin(); it != battleLog.end();)
		{
			view.Write(view.TL_BOTTOM, " ");
			string logStr = MakeString(TA_RIGHT, 30, player->GetName() + " [[ " + battleItemName[it->useItemA] + " ]]") + " VS " + MakeString(TA_LEFT, 30, monster->GetName() + " [[ " + battleItemName[it->useItemB] + " ]]");
			view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, logStr));
			if (it->useItemA == it->useItemB)
			{
				// ���º�
				view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** �� �� �� ***"));
			}
			else if (it->useItemA == (it->useItemB + 1) % 3)
			{
				// �¸�
				view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** �� �� ***"));
				state = BattleState::BS_WIN;
			}
			else
			{
				// �й�
				view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** �� �� ***"));
				state = BattleState::BS_LOSE;
			}
			it = battleLog.erase(it);
		}
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: < ��� > �ƹ�Ű�� �����ּ���...");
	}
	// �信�ִ°� ���
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();
}

void InGameBattle::Release()
{
}
