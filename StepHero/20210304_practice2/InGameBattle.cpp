#include "stdafx.h"
#include "InGameBattle.h"


InGameBattle::InGameBattle()
{
}

void InGameBattle::RenderBattleStatus()
{
	// 배틀의 정보 출력 내부함수
	vector<string> statusLines(10);
	//     ++저주받은 용사++
	// 레벨                  1 LV
	// 공격력                  35
	// 체력             100 / 100
	// [DDDDDDDDDDDDDDDDDDDDDDDDD]
	statusLines[0] = MakeString(TA_CENTER, 27, "++" + player->GetName() + "++");
	statusLines[1] = MakeString(TA_LEFT, 6, "레  벨") + MakeString(TA_RIGHT, 21, to_string(player->GetLvl()) + " LV");
	statusLines[2] = MakeString(TA_LEFT, 6, "공격력") + MakeString(TA_RIGHT, 21, to_string(player->GetAtk()));
	statusLines[3] = MakeString(TA_LEFT, 6, "체  력") + MakeString(TA_RIGHT, 21, string(to_string(player->GetHp()) + " / " + to_string(player->GetMaxHP())));
	statusLines[4] = "["+ MakeString(TA_LEFT, 25, string((int)(ceil(25.0f * player->GetHp() / player->GetMaxHP()) + 0.1f), 'D')) + "]";

	statusLines[5] = MakeString(TA_CENTER, 27, "++" + monster->GetName() + "++");
	statusLines[6] = MakeString(TA_LEFT, 6, "레  벨") + MakeString(TA_RIGHT, 21, to_string(monster->GetLvl()) + " LV");
	statusLines[7] = MakeString(TA_LEFT, 6, "공격력") + MakeString(TA_RIGHT, 21, to_string(monster->GetAtk()));
	statusLines[8] = MakeString(TA_LEFT, 6, "체  력") + MakeString(TA_RIGHT, 21, string(to_string(monster->GetHp()) + " / " + to_string(monster->GetMaxHP())));
	statusLines[9] = "[" + MakeString(TA_RIGHT, 25, string((int)(ceil(25.0f * monster->GetHp() / monster->GetMaxHP()) + 0.1f), 'D')) + "]";

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
	battleItemName = { "가위", "바위", " 보 " };
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
		// 배틀
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
			// 승리
			result = BattleResult::BR_PLAYER_WIN;
			monster->HitDamage(player->GetAtk());
		}
		else
		{
			result = BattleResult::BR_ENEMY_WIN;
			player->HitDamage(monster->GetAtk());
		}
		//battleLog.resize(30, {1,  1});
		battleLog.push_back({ playerInput, monsterInput });
		state = BattleState::BS_BATTLE_END;
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
	else if (state == BattleState::BS_BATTLE_RESULT)
	{
		// 결과 출력 중 결과 로그
		if (view.IsBufferEmpty(view.TL_BOTTOM))
		{
			// 결과창의 데이터 출력이 다 되었는가?
			state = BattleState::BS_CYCLE_END;
		}
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
	else if (state == BattleState::BS_CYCLE_END)
	{
		if (result == BattleResult::BR_PLAYER_WIN)
		{
			// 몬스터가 죽었는지 확인
			if (monster->IsAlive())
			{
				state = BattleState::BS_BATTLE_START;
				gKeyManager.Request(KeyManager::InputType::ANYKEYS);
			}
			else
			{
				// 선택
				state = BattleState::BS_CHOOSE_SHOP;
				gKeyManager.Request(KeyManager::InputType::SELECT);
				//gKeyManager.Clear();
				//gTextViewManager.ChangeView(gTextViewManager.VT_INGAME, gTextViewManager.AT_ZIGZAG_OUT_IN, 1000);
			}
		}
		else if (result == BattleResult::BR_ENEMY_WIN)
		{
			// 플레이어가 죽었는지 확인
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
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** " + monster->GetName() + "와 조우하였습니다. ***"));
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: < 계속 > 아무키나 눌러주세요...");
	}
	else if (state == BattleState::BS_BATTLE_START)
	{
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** 전투 시작!! ***"));
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: < 계속 > 아무키나 눌러주세요...");
	}
	else if (state == BattleState::BS_BATTLE_END)
	{
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** 가위! 바위! 보! ***"));
		for (auto it = battleLog.begin(); it != battleLog.end();)
		{
			view.Write(view.TL_BOTTOM, " ");
			string logStr = MakeString(TA_RIGHT, 30, player->GetName() + " [[ " + battleItemName[it->useItemA] + " ]]") + " VS " + MakeString(TA_LEFT, 30, monster->GetName() + " [[ " + battleItemName[it->useItemB] + " ]]");
			view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, logStr));
			if (it->useItemA == it->useItemB)
			{
				// 무승부
				view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** 무 승 부 ***"));
			}
			else if (it->useItemA == (it->useItemB + 1) % 3)
			{
				// 승리
				view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** 승 리 ***"));
			}
			else
			{
				// 패배
				view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "*** 패 배 ***"));
			}
			it = battleLog.erase(it);
		}
		state = BS_BATTLE_RESULT;
	}
	else if (state == BattleState::BS_CYCLE_END)
	{
		RenderBattleStatus();
		// 팝업창으로 결과를 띄워줌
		// if (result ==)
		view.Write(view.TL_BOTTOM, " 결과 팝업 ");
		view.Write(view.TL_BOTTOM, ":: < 계속 > 아무키나 눌러주세요...");
	}
	else if (state == BattleState::BS_CHOOSE_SHOP)
	{
		// 상점으로 갈지 안갈지 선택하는 화면
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, "상점을 이용하시겠습니까?"));
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
			view.Write(view.TL_BOTTOM, ":: 입력이 잘못되었습니다.");
			view.Write(view.TL_BOTTOM, ":: 한글로 되어있다면 한/영 키로 영어로 바꿔주세요.");
		}
	}

	// 뷰에있는걸 출력
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();

	if (state == BattleState::BS_BATTLE_RESULT)
	{
		// 이곳은 예외적으로 cout 사용?
		// 버퍼공간을 사용안하면서 해야해서 현재 구조상 불가피함
		cout << ":: < 계속 > 아무키나 눌러주세요...";
	}
}

void InGameBattle::Release()
{
}
