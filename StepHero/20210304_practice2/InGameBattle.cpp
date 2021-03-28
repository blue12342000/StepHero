#include "stdafx.h"
#include "InGameBattle.h"


InGameBattle::InGameBattle()
{
}

void InGameBattle::RenderBattleStatus()
{
	// ��Ʋ�� ���� ��� �����Լ�
	vector<string> statusLines(10);
	//     ++���ֹ��� ���++
	// ����                  1 LV
	// ���ݷ�                  35
	// ü��             100 / 100
	// [DDDDDDDDDDDDDDDDDDDDDDDDD]
	statusLines[0] = MakeString(TA_CENTER, 27, "++" + player->GetName() + "++");
	statusLines[1] = MakeString(TA_LEFT, 6, "��  ��") + MakeString(TA_RIGHT, 21, to_string(player->GetLvl()) + " LV");
	statusLines[2] = MakeString(TA_LEFT, 6, "���ݷ�") + MakeString(TA_RIGHT, 21, to_string(player->GetAtk()));
	statusLines[3] = MakeString(TA_LEFT, 6, "ü  ��") + MakeString(TA_RIGHT, 21, string(to_string(player->GetHp()) + " / " + to_string(player->GetMaxHP())));
	statusLines[4] = "["+ MakeString(TA_LEFT, 25, string((int)(ceil(25.0f * player->GetHp() / player->GetMaxHP()) + 0.1f), 'D')) + "]";

	statusLines[5] = MakeString(TA_CENTER, 27, "++" + monster.GetName() + "++");
	statusLines[6] = MakeString(TA_LEFT, 6, "��  ��") + MakeString(TA_RIGHT, 21, to_string(monster.GetLvl()) + " LV");
	statusLines[7] = MakeString(TA_LEFT, 6, "���ݷ�") + MakeString(TA_RIGHT, 21, to_string(monster.GetAtk()));
	statusLines[8] = MakeString(TA_LEFT, 6, "ü  ��") + MakeString(TA_RIGHT, 21, string(to_string(monster.GetHp()) + " / " + to_string(monster.GetMaxHP())));
	statusLines[9] = "[" + MakeString(TA_RIGHT, 25, string((int)(ceil(25.0f * monster.GetHp() / monster.GetMaxHP()) + 0.1f), 'D')) + "]";

	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width,                  "                                     "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, statusLines[0] + "                                     " + statusLines[5]));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width,                  "          ##   ##    ######          "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, statusLines[1] + "          ##   ##   ##               " + statusLines[6]));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width,                  "          ##   ##   ######           "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, statusLines[2] + "          ##   ##       ###          " + statusLines[7]));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width,                  "           ## ##         ##          "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, statusLines[3] + "            ###    #######           " + statusLines[8]));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width,                  "                                     "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, statusLines[4] + "                                     " + statusLines[9]));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
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
	//monster = gMonsterTable.GetMonster(Room::FieldType::wood);

	state = BattleState::BS_INIT;
}

void InGameBattle::Update()
{
	if (state == BattleState::BS_INIT)
	{
		state = BattleState::BS_READY;
	}
	else if (state == BattleState::BS_READY)
	{
		state = BattleState::BS_BATTLE_START;
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
	else if (state == BattleState::BS_BATTLE_START)
	{
		result = BattleResult::BR_NONE;

		// ��Ʋ
		int playerInput = rand() % 3;
		int monsterInput = rand() % 3;

		while (playerInput == monsterInput)
		{
			battleLog.push_back({ playerInput, monsterInput });
			playerInput = rand() % 3;
			monsterInput = rand() % 3;
		}

		if (playerInput == (monsterInput + 1) % 3)
		{
			// �¸�
			result = BattleResult::BR_PLAYER_WIN;
			monster.HitDamage(player->GetAtk());
		}
		else
		{
			result = BattleResult::BR_ENEMY_WIN;
			player->HitDamage(monster.GetAtk());
		}
		//battleLog.resize(30, {1,  1});
		battleLog.push_back({ playerInput, monsterInput });
		state = BattleState::BS_BATTLE_END;
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
	else if (state == BattleState::BS_BATTLE_RESULT)
	{
		// ��� ��� �� ��� �α�
		if (view.IsBufferEmpty(view.TL_BOTTOM))
		{
			// ���â�� ������ ����� �� �Ǿ��°�?
			state = BattleState::BS_CYCLE_END;
		}
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
	else if (state == BattleState::BS_CYCLE_END)
	{
		if (result == BattleResult::BR_PLAYER_WIN)
		{
			// ���Ͱ� �׾����� Ȯ��
			if (monster.IsAlive())
			{
				state = BattleState::BS_BATTLE_START;
				gKeyManager.Request(KeyManager::InputType::ANYKEYS);
			}
			else
			{
				// ����
				state = BattleState::BS_CHOOSE_SHOP;
				gKeyManager.Request(KeyManager::InputType::SELECT);

				gQuestManager.Progress(Quest::QusetType::QT_HUNT, &monster);

				//gKeyManager.Clear();
				//gTextViewManager.ChangeView(gTextViewManager.VT_INGAME, gTextViewManager.AT_ZIGZAG_OUT_IN, 1000);
			}
		}
		else if (result == BattleResult::BR_ENEMY_WIN)
		{
			// �÷��̾ �׾����� Ȯ��
			if (player->IsAlive())
			{
				state = BattleState::BS_BATTLE_START;
				gKeyManager.Request(KeyManager::InputType::ANYKEYS);
			}
			else
			{
				gKeyManager.Clear();
				gTextViewManager.ChangeView(gTextViewManager.VT_GAMEOVER, gTextViewManager.AT_FADE_OUT_IN, 4000);
			}
		}
	}
	else if (state == BattleState::BS_CHOOSE_SHOP)
	{
		if (gKeyManager.IsPressKey('w') || gKeyManager.IsPressKey('s'))
		{
			selectShopEnterYN = !selectShopEnterYN;
			gKeyManager.Request(KeyManager::InputType::SELECT);
		}
		else if (gKeyManager.IsPressKey(13))
		{
			// ENTER
			if (selectShopEnterYN)
			{
				gKeyManager.Clear();
				gTextViewManager.ChangeView(gTextViewManager.VT_SHOP, gTextViewManager.AT_FADE_OUT_IN, 500);
			}
			else
			{
				gKeyManager.Clear();
				gTextViewManager.ChangeView(gTextViewManager.VT_INGAME, gTextViewManager.AT_ZIGZAG_OUT_IN, 500);
			}
		}
	}
}

void InGameBattle::Render(vector<string>* targetBuffer)
{
	if (state == BattleState::BS_INIT || state == BattleState::BS_READY)
	{
		RenderBattleStatus();
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** " + monster.GetName() + "�� �����Ͽ����ϴ�. ***"));
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: < ��� > �ƹ�Ű�� �����ּ���...");
	}
	else if (state == BattleState::BS_BATTLE_START)
	{
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** ���� ����!! ***"));
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: < ��� > �ƹ�Ű�� �����ּ���...");
	}
	else if (state == BattleState::BS_BATTLE_END)
	{
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** ����! ����! ��! ***"));
		for (auto it = battleLog.begin(); it != battleLog.end();)
		{
			view.Write(view.TL_BOTTOM, " ");
			string logStr = MakeString(TA_RIGHT, 30, player->GetName() + " [[ " + battleItemName[it->useItemA] + " ]]") + " VS " + MakeString(TA_LEFT, 30, monster.GetName() + " [[ " + battleItemName[it->useItemB] + " ]]");
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
			}
			else
			{
				// �й�
				view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** �� �� ***"));
			}
			it = battleLog.erase(it);
		}
		state = BS_BATTLE_RESULT;
	}
	else if (state == BattleState::BS_CYCLE_END)
	{
		RenderBattleStatus();
		// �˾�â���� ����� �����
		if (result == BattleResult::BR_PLAYER_WIN)
		{
			// �÷��̾� �¸� �˾�
			popup.Write("");
			popup.Write("= �� �� =");
			popup.Write("");
			popup.Write("�¸� �Ͽ����ϴ�");
			popup.Write("");
			popup.Write("������ " + to_string(player->GetAtk()) + "���ظ� �־����ϴ�");
		}
		else if (result == BattleResult::BR_ENEMY_WIN)
		{
			// �÷��̾� �й� �˾�
			popup.Write("");
			popup.Write("= �� �� =");
			popup.Write("");
			popup.Write("�й� �Ͽ����ϴ�");
			popup.Write("");
			popup.Write(to_string(monster.GetAtk()) + "���ظ� �޾ҽ��ϴ�");
		}
	}
	else if (state == BattleState::BS_CHOOSE_SHOP)
	{
		// �������� ���� �Ȱ��� �����ϴ� ȭ��
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "������ �̿��Ͻðڽ��ϱ�?"));
		view.Write(view.TL_BOTTOM, " ");

		string strYes = MakeString(TA_CENTER, 6, "YES");
		string strNo = MakeString(TA_CENTER, 6, "NO");
		if (selectShopEnterYN) strYes = "<< " + strYes + " >>";
		else  strNo = "<< " + strNo + " >>";

		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, MakeString(TA_CENTER, 12, strYes) + MakeString(TA_CENTER, 12, strNo)));
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "CHANGE :: W,S  SELECT :: ENTER"));

		if (gKeyManager.inputResult == KeyManager::InputResult::FAIL)
		{
			view.Write(view.TL_BOTTOM, " ");
			view.Write(view.TL_BOTTOM, ":: [ ERROR ]");
			view.Write(view.TL_BOTTOM, ":: �Է��� �߸��Ǿ����ϴ�.");
			view.Write(view.TL_BOTTOM, ":: �ѱ۷� �Ǿ��ִٸ� ��/�� Ű�� ����� �ٲ��ּ���.");
		}
	}

	view.Refresh();
	if (state == BattleState::BS_CYCLE_END && (result == BattleResult::BR_PLAYER_WIN || result == BattleResult::BR_ENEMY_WIN))
	{
		popup.Show(view, view.GetWidth() / 2 - 20, view.GetHeight() / 2 - 7, 40, 10);
	}
	// �信�ִ°� ���
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();

	if (state == BattleState::BS_BATTLE_RESULT
		|| (state == BattleState::BS_CYCLE_END && (result == BattleResult::BR_PLAYER_WIN || result == BattleResult::BR_ENEMY_WIN)))
	{
		// �̰��� ���������� cout ���?
		// ���۰����� �����ϸ鼭 �ؾ��ؼ� ���� ������ �Ұ�����
		cout << ":: < ��� > �ƹ�Ű�� �����ּ���...";
	}
}

void InGameBattle::Release()
{
}
