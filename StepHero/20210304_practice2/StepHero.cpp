#include "stdafx.h"
#include "StepHero.h"

StepHero::StepHero()
{
}


StepHero::~StepHero()
{
}

void StepHero::Init()
{
#pragma region �������ø�
	gDungeonTemplate[0] = { 10, 10 };
	gDungeonTemplate[0].fieldSet = new int*[gDungeonTemplate[0].rows];
	gDungeonTemplate[0].fieldSet[0] = new int[gDungeonTemplate[0].cols]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	gDungeonTemplate[0].fieldSet[1] = new int[gDungeonTemplate[0].cols]{ 0, 0, 0, 0, 0, 0, 0, 2, 1, 1 };
	gDungeonTemplate[0].fieldSet[2] = new int[gDungeonTemplate[0].cols]{ 0, 0, 0, 0, 0, 3, 2, 2, 2, 1 };
	gDungeonTemplate[0].fieldSet[3] = new int[gDungeonTemplate[0].cols]{ 0, 0, 0, 0, 3, 2, 2, 2, 0, 2 };
	gDungeonTemplate[0].fieldSet[4] = new int[gDungeonTemplate[0].cols]{ 0, 0, 0, 0, 0, 3, 2, 0, 0, 0 };
	gDungeonTemplate[0].fieldSet[5] = new int[gDungeonTemplate[0].cols]{ 0, 0, 3, 0, 0, 2, 2, 4, 0, 0 };
	gDungeonTemplate[0].fieldSet[6] = new int[gDungeonTemplate[0].cols]{ 0, 0, 0, 1, 0, 1, 1, 2, 0, 0 };
	gDungeonTemplate[0].fieldSet[7] = new int[gDungeonTemplate[0].cols]{ 0, 0, 1, 1, 0, 1, 1, 1, 2, 0 };
	gDungeonTemplate[0].fieldSet[8] = new int[gDungeonTemplate[0].cols]{ 0, 0, 0, 0, 1, 0, 0, 1, 1, 2 };
	gDungeonTemplate[0].fieldSet[9] = new int[gDungeonTemplate[0].cols]{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 2 };

	gDungeonTemplate[1] = { 10, 10 };
	gDungeonTemplate[1].fieldSet = new int*[gDungeonTemplate[1].rows];
	gDungeonTemplate[1].fieldSet[0] = new int[gDungeonTemplate[1].cols]{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 };
	gDungeonTemplate[1].fieldSet[1] = new int[gDungeonTemplate[1].cols]{ 0, 0, 0, 0, 1, 1, 1, 1, 0, 0 };
	gDungeonTemplate[1].fieldSet[2] = new int[gDungeonTemplate[1].cols]{ 0, 0, 0, 1, 1, 1, 0, 0, 3, 0 };
	gDungeonTemplate[1].fieldSet[3] = new int[gDungeonTemplate[1].cols]{ 0, 0, 1, 1, 0, 0, 0, 3, 3, 0 };
	gDungeonTemplate[1].fieldSet[4] = new int[gDungeonTemplate[1].cols]{ 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 };
	gDungeonTemplate[1].fieldSet[5] = new int[gDungeonTemplate[1].cols]{ 0, 1, 1, 1, 3, 0, 4, 3, 3, 0 };
	gDungeonTemplate[1].fieldSet[6] = new int[gDungeonTemplate[1].cols]{ 0, 1, 1, 3, 3, 0, 3, 0, 0, 0 };
	gDungeonTemplate[1].fieldSet[7] = new int[gDungeonTemplate[1].cols]{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	gDungeonTemplate[1].fieldSet[8] = new int[gDungeonTemplate[1].cols]{ 0, 0, 0, 3, 3, 0, 3, 0, 0, 0 };
	gDungeonTemplate[1].fieldSet[9] = new int[gDungeonTemplate[1].cols]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	gDungeonTemplate[2] = { 5, 10 };
	gDungeonTemplate[2].fieldSet = new int*[gDungeonTemplate[2].rows];
	gDungeonTemplate[2].fieldSet[0] = new int[gDungeonTemplate[2].cols]{ 0, 0, 1, 1, 0, 0, 2, 2, 0, 0 };
	gDungeonTemplate[2].fieldSet[1] = new int[gDungeonTemplate[2].cols]{ 0, 1, 1, 1, 1, 0, 2, 2, 2, 0 };
	gDungeonTemplate[2].fieldSet[2] = new int[gDungeonTemplate[2].cols]{ 0, 0, 1, 1, 1, 3, 0, 0, 2, 2 };
	gDungeonTemplate[2].fieldSet[3] = new int[gDungeonTemplate[2].cols]{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 2 };
	gDungeonTemplate[2].fieldSet[4] = new int[gDungeonTemplate[2].cols]{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 2 };

	gDungeonTemplate[3] = { 5, 10 };
	gDungeonTemplate[3].fieldSet = new int*[gDungeonTemplate[3].rows];
	gDungeonTemplate[3].fieldSet[0] = new int[gDungeonTemplate[3].cols]{ 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
	gDungeonTemplate[3].fieldSet[1] = new int[gDungeonTemplate[3].cols]{ 0, 0, 1, 1, 1, 0, 3, 0, 0, 0 };
	gDungeonTemplate[3].fieldSet[2] = new int[gDungeonTemplate[3].cols]{ 0, 0, 1, 1, 0, 3, 0, 0, 1, 0 };
	gDungeonTemplate[3].fieldSet[3] = new int[gDungeonTemplate[3].cols]{ 0, 1, 1, 1, 3, 0, 1, 1, 1, 0 };
	gDungeonTemplate[3].fieldSet[4] = new int[gDungeonTemplate[3].cols]{ 0, 0, 0, 1, 0, 1, 1, 1, 0, 0 };

	gDungeonTemplate[4] = { 10, 5 };
	gDungeonTemplate[4].fieldSet = new int*[gDungeonTemplate[4].rows];
	gDungeonTemplate[4].fieldSet[0] = new int[gDungeonTemplate[4].cols]{ 0, 0, 0, 0, 0 };
	gDungeonTemplate[4].fieldSet[1] = new int[gDungeonTemplate[4].cols]{ 0, 1, 1, 1, 1 };
	gDungeonTemplate[4].fieldSet[2] = new int[gDungeonTemplate[4].cols]{ 1, 1, 1, 1, 1 };
	gDungeonTemplate[4].fieldSet[3] = new int[gDungeonTemplate[4].cols]{ 0, 0, 1, 0, 1 };
	gDungeonTemplate[4].fieldSet[4] = new int[gDungeonTemplate[4].cols]{ 0, 0, 1, 0, 1 };
	gDungeonTemplate[4].fieldSet[5] = new int[gDungeonTemplate[4].cols]{ 0, 0, 2, 0, 2 };
	gDungeonTemplate[4].fieldSet[6] = new int[gDungeonTemplate[4].cols]{ 0, 2, 2, 2, 2 };
	gDungeonTemplate[4].fieldSet[7] = new int[gDungeonTemplate[4].cols]{ 0, 2, 2, 2, 2 };
	gDungeonTemplate[4].fieldSet[8] = new int[gDungeonTemplate[4].cols]{ 2, 2, 2, 2, 2 };
	gDungeonTemplate[4].fieldSet[9] = new int[gDungeonTemplate[4].cols]{ 2, 2, 2, 2, 2 };

	gDungeonTemplate[5] = { 10, 5 };
	gDungeonTemplate[5].fieldSet = new int*[gDungeonTemplate[5].rows];
	gDungeonTemplate[5].fieldSet[0] = new int[gDungeonTemplate[5].cols]{ 0, 0, 0, 0, 0 };
	gDungeonTemplate[5].fieldSet[1] = new int[gDungeonTemplate[5].cols]{ 0, 1, 0, 1, 1 };
	gDungeonTemplate[5].fieldSet[2] = new int[gDungeonTemplate[5].cols]{ 0, 1, 1, 1, 1 };
	gDungeonTemplate[5].fieldSet[3] = new int[gDungeonTemplate[5].cols]{ 0, 0, 0, 0, 1 };
	gDungeonTemplate[5].fieldSet[4] = new int[gDungeonTemplate[5].cols]{ 0, 0, 0, 0, 1 };
	gDungeonTemplate[5].fieldSet[5] = new int[gDungeonTemplate[5].cols]{ 0, 0, 1, 0, 0 };
	gDungeonTemplate[5].fieldSet[6] = new int[gDungeonTemplate[5].cols]{ 0, 1, 0, 0, 0 };
	gDungeonTemplate[5].fieldSet[7] = new int[gDungeonTemplate[5].cols]{ 1, 1, 0, 0, 0 };
	gDungeonTemplate[5].fieldSet[8] = new int[gDungeonTemplate[5].cols]{ 0, 1, 0, 0, 0 };
	gDungeonTemplate[5].fieldSet[9] = new int[gDungeonTemplate[5].cols]{ 0, 0, 0, 0, 0 };

	gDungeonTemplate[6] = { 5, 5 };
	gDungeonTemplate[6].fieldSet = new int*[gDungeonTemplate[6].rows];
	gDungeonTemplate[6].fieldSet[0] = new int[gDungeonTemplate[6].cols]{ 0, 3, 0, 3, 0 };
	gDungeonTemplate[6].fieldSet[1] = new int[gDungeonTemplate[6].cols]{ 0, 0, 2, 0, 0 };
	gDungeonTemplate[6].fieldSet[2] = new int[gDungeonTemplate[6].cols]{ 0, 2, 2, 2, 0 };
	gDungeonTemplate[6].fieldSet[3] = new int[gDungeonTemplate[6].cols]{ 0, 0, 2, 0, 0 };
	gDungeonTemplate[6].fieldSet[4] = new int[gDungeonTemplate[6].cols]{ 0, 3, 0, 3, 0 };

	gDungeonTemplate[7] = { 5, 5 };
	gDungeonTemplate[7].fieldSet = new int*[gDungeonTemplate[7].rows];
	gDungeonTemplate[7].fieldSet[0] = new int[gDungeonTemplate[7].cols]{ 0, 0, 3, 3, 0 };
	gDungeonTemplate[7].fieldSet[1] = new int[gDungeonTemplate[7].cols]{ 3, 0, 0, 0, 0 };
	gDungeonTemplate[7].fieldSet[2] = new int[gDungeonTemplate[7].cols]{ 3, 0, 4, 0, 3 };
	gDungeonTemplate[7].fieldSet[3] = new int[gDungeonTemplate[7].cols]{ 0, 0, 0, 0, 3 };
	gDungeonTemplate[7].fieldSet[4] = new int[gDungeonTemplate[7].cols]{ 0, 3, 3, 0, 0 };

	gDungeonTemplate[8] = { 5, 5 };
	gDungeonTemplate[8].fieldSet = new int*[gDungeonTemplate[8].rows];
	gDungeonTemplate[8].fieldSet[0] = new int[gDungeonTemplate[8].cols]{ 0, 2, 0, 3, 0 };
	gDungeonTemplate[8].fieldSet[1] = new int[gDungeonTemplate[8].cols]{ 2, 2, 3, 0, 0 };
	gDungeonTemplate[8].fieldSet[2] = new int[gDungeonTemplate[8].cols]{ 3, 0, 0, 0, 3 };
	gDungeonTemplate[8].fieldSet[3] = new int[gDungeonTemplate[8].cols]{ 0, 0, 3, 1, 1 };
	gDungeonTemplate[8].fieldSet[4] = new int[gDungeonTemplate[8].cols]{ 0, 0, 3, 1, 1 };

	gDungeonTemplate[9] = { 5, 5 };
	gDungeonTemplate[9].fieldSet = new int*[gDungeonTemplate[9].rows];
	gDungeonTemplate[9].fieldSet[0] = new int[gDungeonTemplate[9].cols]{ 0, 0, 1, 0, 0 };
	gDungeonTemplate[9].fieldSet[1] = new int[gDungeonTemplate[9].cols]{ 0, 3, 0, 3, 0 };
	gDungeonTemplate[9].fieldSet[2] = new int[gDungeonTemplate[9].cols]{ 1, 0, 4, 0, 1 };
	gDungeonTemplate[9].fieldSet[3] = new int[gDungeonTemplate[9].cols]{ 0, 3, 0, 3, 0 };
	gDungeonTemplate[9].fieldSet[4] = new int[gDungeonTemplate[9].cols]{ 0, 0, 1, 0, 0 };
#pragma endregion

	// TITLE �۾� == �ѹ��� �ϸ��
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "      ----------------------------------------------------------     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "---------------------------------------------------------------------");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "      #####  #####  #####  #####      #   #  #####  #####  #####     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "      #        #    #      #   #      #   #  #      #   #  #   #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "      #####    #    ###    #####      #####  ###    #####  #   #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "          #    #    #      #          #   #  #      # #    #   #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "          #    #    #      #          #   #  #      #  #   #   #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "      #####    #    #####  #          #   #  #####  #   #  #####     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "---------------------------------------------------------------------");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "     -----------------------------------------------------------     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::TOP, "");

	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::TOP, "---------------------------------------------------------------------");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::TOP, "      #####  #####  #####  #####      #   #  #####  #####  #####     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::TOP, "      #        #    #      #   #      #   #  #      #   #  #   #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::TOP, "      #####    #    ###    #####      #####  ###    #####  #   #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::TOP, "          #    #    #      #          #   #  #      # #    #   #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::TOP, "          #    #    #      #          #   #  #      #  #   #   #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::TOP, "      #####    #    #####  #          #   #  #####  #   #  #####     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::TOP, "---------------------------------------------------------------------");

	gTextRender.AppendBuffer(TextLayout::LayoutKind::SHOP, TextLayout::LayoutPos::TOP, "---------------------------------------------------------------------");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::SHOP, TextLayout::LayoutPos::TOP, " #####  #####  #####  #####  #####  #   #    ##### #   # ##### ##### ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::SHOP, TextLayout::LayoutPos::TOP, " #   #  #   #    #      #    #   #  ##  #    #     #   # #   # #   # ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::SHOP, TextLayout::LayoutPos::TOP, " #####  #   #    #      #    #   #  # # #    ##### ##### #   # ##### ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::SHOP, TextLayout::LayoutPos::TOP, " #      #   #    #      #    #   #  # # #        # #   # #   # #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::SHOP, TextLayout::LayoutPos::TOP, " #      #   #    #      #    #   #  #  ##        # #   # #   # #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::SHOP, TextLayout::LayoutPos::TOP, " #      #####    #    #####  #####  #   #    ##### #   # ##### #     ");
	gTextRender.AppendBuffer(TextLayout::LayoutKind::SHOP, TextLayout::LayoutPos::TOP, "---------------------------------------------------------------------");
}

void StepHero::Update()
{
	switch (gameState)
	{
	case GameState::TITLE_INPUT_DIFFI:
	case GameState::TITLE_INPUT_NAME:
		if (gKeyManager.inputResult == KeyManager::InputResult::SUCCESS)
		{
			if (gameState == GameState::TITLE_INPUT_DIFFI)
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
					gameState = GameState::TITLE_INPUT_NAME;
					gKeyManager.Request(KeyManager::InputType::TEXT);
				}
			}
			else if (gameState == GameState::TITLE_INPUT_NAME)
			{
				gameState = GameState::TIELE_INGAME_LODING;
				
				// ������ ���̵��� ������ �Է������� �� �޾�����
				// ������ ���� �÷��̾�, ����, ���� ����
				player = new Hero(1, difficulty.heroHP[selectDifficulty], 30, 1000, 0, 3.5f, gKeyManager.inputText);
				player->SetPosition(0, 0);

				gMonsterTable.Init(difficulty.monsterStrength[selectDifficulty]);

				dungeon = new Dungeon();
				dungeon->CreateDungeon(difficulty.dungeonSize[selectDifficulty]);
				dungeon->SetMonsters(difficulty.monsterEncounter[selectDifficulty], player);

				dungeon->Render(player, false);
				// �÷��̾� ����
				gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, string(100, '='));
				player->Render(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM);
				// �ý��� �α�
				gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, string(100, '='));
				gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, ":: W, A, S ,D �� �̵����ּ���...");

				gTextRender.ChangeLayout(TextRender::TextChangeAnim::FADE_OUT_IN, TextLayout::LayoutKind::TITLE, TextLayout::LayoutKind::INGAME, 1000);
			}
		}
		if (gameState == GameState::TITLE_INPUT_DIFFI) gKeyManager.Request(KeyManager::InputType::SELECT);
		break;
	case GameState::TIELE_INGAME_LODING:
		gTextRender.Update();
		if (!gTextRender.isAnimationRun)
		{
			gameState = GameState::INGAME;
			gTextRender.Render(TextLayout::LayoutKind::INGAME);
		}
		break;
	case GameState::INGAME:
		if (gKeyManager.IsPressKey('w'))
		{
			if (dungeon->IsPossibleMove(player->GetPosX(), player->GetPosY() - 1))
			{
				player->Move(player->GetPosX(), player->GetPosY() - 1);
			}
		}
		else if (gKeyManager.IsPressKey('a'))
		{
			if (dungeon->IsPossibleMove(player->GetPosX() - 1, player->GetPosY()))
			{
				player->Move(player->GetPosX() - 1, player->GetPosY());
			}
		}
		else if (gKeyManager.IsPressKey('s'))
		{
			if (dungeon->IsPossibleMove(player->GetPosX(), player->GetPosY() + 1))
			{
				player->Move(player->GetPosX(), player->GetPosY() + 1);
			}
		}
		else if (gKeyManager.IsPressKey('d'))
		{
			if (dungeon->IsPossibleMove(player->GetPosX() + 1, player->GetPosY()))
			{
				player->Move(player->GetPosX() + 1, player->GetPosY());
			}
		}
		gKeyManager.Request(KeyManager::InputType::INGAME);
		break;
	case GameState::INGAME_BATTLE:

		break;
	case GameState::INGAME_SHOP:
		break;
	}
}

void StepHero::Render()
{
	switch (gameState)
	{
	case GameState::TITLE_INPUT_DIFFI:
	case GameState::TITLE_INPUT_NAME:

		gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "                                ------                               ");
		gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "                              ----  ----                             ");
		gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "");

		gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, difficulty.name[selectDifficulty]);

		if (gameState == GameState::TITLE_INPUT_DIFFI)
		{
			gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "");
			gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "CHANGE :: W,S  SELECT :: ENTER");
		}

		gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "");
		gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "                              ----  ----                             ");
		gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "                                ------                               ");

		if (gameState == GameState::TITLE_INPUT_NAME)
		{
			gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "");
			gTextRender.AppendBuffer(TextLayout::LayoutKind::TITLE, TextLayout::LayoutPos::CONTENT, "������ �̸��� �Է����ּ���");
		}

		gTextRender.Refresh(TextLayout::LayoutKind::TITLE);
		gTextRender.Render(TextLayout::LayoutKind::TITLE);
		break;
	case GameState::TIELE_INGAME_LODING:
		gTextRender.Render();
		break;
	case GameState::INGAME:
		dungeon->Render(player, false);

		// �÷��̾� ����
		gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, string(100, '='));
		player->Render(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM);
		
		// �ý��� �α�
		gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, string(100, '='));
		gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, ":: W, A, S ,D �� �̵����ּ���...");
		if (gKeyManager.inputResult == KeyManager::InputResult::FAIL)
		{
			gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, "");
			gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, ":: [ ERROR ]");
			gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, ":: �Է��� �߸��Ǿ����ϴ�.");
			gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::BOTTOM, ":: �ѱ۷� �Ǿ��ִٸ� ��/�� Ű�� ����� �ٲ��ּ���.");

		}

		gTextRender.Refresh(TextLayout::LayoutKind::INGAME);
		gTextRender.Render(TextLayout::LayoutKind::INGAME);
		break;
	case GameState::INGAME_BATTLE:

		break;
	case GameState::INGAME_SHOP:
		break;
	}
}

bool StepHero::IsEnd()
{
	return (gameState == GameState::ENDING);
}

void StepHero::Release()
{
	if (dungeon) dungeon->Release();
	delete dungeon;
	//if (shop) shop->Release();
	if (player) player->Release();
	delete player;
}
