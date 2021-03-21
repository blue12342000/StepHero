#include "stdafx.h"
#include "GameTitle.h"

GameTitle::GameTitle() { }
GameTitle::GameTitle(int width, int height)
{
	this->width = width;
	this->height = height;
	view.Init(width, height);
	view.AddLayout(view.TL_TOP, view.RT_STATIC, 0, 16);
	view.AddLayout(view.TL_CONTENT, view.RT_EVERYTIME, 16, 13);

	// Ÿ��Ʋ �߰�
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, ""));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, ""));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      ----------------------------------------------------------     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      #####  #####  #####  #####      #   #  #####  #####  #####     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      #        #    #      #   #      #   #  #      #   #  #   #     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      #####    #    ###    #####      #####  ###    #####  #   #     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "          #    #    #      #          #   #  #      # #    #   #     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "          #    #    #      #          #   #  #      #  #   #   #     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "      #####    #    #####  #          #   #  #####  #   #  #####     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "     -----------------------------------------------------------     "));
	//view.Write(view.TL_TOP, MakeString(TA_CENTER, width, ""));
	//view.Write(view.TL_TOP, MakeString(TA_CENTER, width, ""));
}
GameTitle::~GameTitle() { }

void GameTitle::Init()
{
}

void GameTitle::Update()
{
	if (gKeyManager.inputResult == KeyManager::InputResult::SUCCESS)
	{
		if (gameState == GameState::GS_SELECT_DIFFI)
		{
			if (gKeyManager.IsPressKey('w'))
			{
				selectDifficulty = (selectDifficulty + 2) % 3;
			}
			else if (gKeyManager.IsPressKey('s'))
			{
				selectDifficulty = (selectDifficulty + 1) % 3;
			}
			else if (gKeyManager.IsPressKey(13))
			{
				// ENTER
				gameState = GameState::GS_INPUT_NAME;
				gKeyManager.Clear();
				gKeyManager.Request(KeyManager::InputType::TEXT);
			}
		}
		else if (gameState == GameState::GS_INPUT_NAME)
		{
			//gameState = GameState::TITLE_INGAME_LODING;

			// ������ ���̵��� ������ �Է������� �� �޾�����
			// ������ ���� �÷��̾�, ����, ���� ����
			gGameData.SetDifficulty(selectDifficulty);

			Hero* player = new Hero(1, difficulty.heroHP[selectDifficulty], 30, 1000, 0, 3.5f, gKeyManager.inputText);
			player->SetPosition(0, 0);
			
			gMonsterTable.Init(difficulty.monsterStrength[selectDifficulty]);
			
			Dungeon* dungeon = new Dungeon();
			dungeon->CreateDungeon(difficulty.dungeonSize[selectDifficulty]);
			dungeon->SetMonsters(difficulty.monsterEncounter[selectDifficulty], player);
			
			gGameData.SetHero(*player);
			gGameData.SetDungeon(*dungeon);

			gKeyManager.Clear();
			gTextViewManager.ChangeView(gTextViewManager.VT_INGAME, gTextViewManager.AT_FADE_OUT_IN, 1000);
		}
	}
	if (gameState == GameState::GS_SELECT_DIFFI) gKeyManager.Request(KeyManager::InputType::SELECT);

	//view.Update();
}

void GameTitle::Render(vector<string>* targetBuffer)
{
	// �信 ���� ����
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "------"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "----  ----"));
	view.Write(view.TL_CONTENT, "");

	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, difficulty.name[selectDifficulty]));

	if (gameState == GameState::GS_SELECT_DIFFI)
	{
		view.Write(view.TL_CONTENT, "");
		view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "CHANGE :: W,S  SELECT :: ENTER"));
	}

	view.Write(view.TL_CONTENT, "");
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "----  ----"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "------"));

	if (gameState == GameState::GS_INPUT_NAME)
	{
		view.Write(view.TL_CONTENT, "");
		view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "������ �̸��� �Է����ּ���"));
	}
	else if (gKeyManager.inputResult == KeyManager::InputResult::FAIL)
	{
		view.Write(view.TL_CONTENT, "");
		view.Write(view.TL_CONTENT, ":: [ ERROR ]");
		view.Write(view.TL_CONTENT, ":: �Է��� �߸��Ǿ����ϴ�.");
		view.Write(view.TL_CONTENT, ":: �ѱ۷� �Ǿ��ִٸ� ��/�� Ű�� ����� �ٲ��ּ���.");
	}

	// �信�ִ°� ���
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();
}

void GameTitle::Release()
{
	view.Release();
}
