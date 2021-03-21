#include "stdafx.h"
#include "InGameDungeon.h"


InGameDungeon::InGameDungeon()
{
}

InGameDungeon::InGameDungeon(int width, int height)
{
	this->width = width;
	this->height = height;
	view.Init(width, height);
	view.AddLayout(view.TL_TOP, view.RT_STATIC, 0, 8);
	view.AddLayout(view.TL_CONTENT, view.RT_EVERYTIME, 8, 22);
	view.AddLayout(view.TL_BOTTOM, view.RT_EVERYTIME, 30, 21);

	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "####       #      #####    #####    #        #####"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#   #     # #       #        #      #        #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "####     #   #      #        #      #        ###  "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#   #    #####      #        #      #        #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#   #    #   #      #        #      #        #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "####     #   #      #        #      #####    #####"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
}

InGameDungeon::~InGameDungeon()
{
}

void InGameDungeon::Init()
{
	player = gGameData.GetHeroPtr();
	dungeon = gGameData.GetDungeonPtr();
}

void InGameDungeon::Update()
{
	if (gKeyManager.IsPressKey('w'))
	{
		if (dungeon->IsPossibleMove(player->GetPosX(), player->GetPosY() - 1))
		{
			player->SetState(Hero::HeroState::IDLE);
			player->Move(player->GetPosX(), player->GetPosY() - 1);
		}
	}
	else if (gKeyManager.IsPressKey('a'))
	{
		if (dungeon->IsPossibleMove(player->GetPosX() - 1, player->GetPosY()))
		{
			player->SetState(Hero::HeroState::IDLE);
			player->Move(player->GetPosX() - 1, player->GetPosY());
		}
	}
	else if (gKeyManager.IsPressKey('s'))
	{
		if (dungeon->IsPossibleMove(player->GetPosX(), player->GetPosY() + 1))
		{
			player->SetState(Hero::HeroState::IDLE);
			player->Move(player->GetPosX(), player->GetPosY() + 1);
		}
	}
	else if (gKeyManager.IsPressKey('d'))
	{
		if (dungeon->IsPossibleMove(player->GetPosX() + 1, player->GetPosY()))
		{
			player->SetState(Hero::HeroState::IDLE);
			player->Move(player->GetPosX() + 1, player->GetPosY());
		}
	}
	
	if (player->GetState() == Hero::HeroState::IDLE && dungeon->IsInMonster(player->GetPosX(), player->GetPosY()))
	{
		player->SetState(Hero::HeroState::BATTLE_BEGIN);
	
		// ���Ϳ� ����
		//gameState = GameState::INGAME_BATTLE_LODING;
		//gKeyManager.Clear();
		//
		//// ��Ʋ���� ���Է�
		//Monster* monster = gMonsterTable.GetMonster(dungeon->GetRoomFieldType(player->GetPosX(), player->GetPosY()));
		//battleInfo = BattleInfo(*player, *monster);
		//
		//battleInfo.Render();
		//gTextRender.ChangeLayout(TextRender::TextChangeAnim::ZIGZAG_OUT_IN, TextLayout::LayoutKind::INGAME, TextLayout::LayoutKind::BATTLE, 1000);
		gKeyManager.Clear();
		gTextViewManager.ChangeView(gTextViewManager.VT_BATTLE, gTextViewManager.AT_ZIGZAG_OUT_IN, 1000);
	}
	else
	{
		gKeyManager.Request(KeyManager::InputType::INGAME);
	}
}

void InGameDungeon::Render(vector<string>* targetBuffer)
{
	dungeon->Render(view, player);
	
	// �÷��̾� ����
	view.Write(view.TL_BOTTOM, string(width, '='));
	player->Render(view);

	// �ý��� �α�
	view.Write(view.TL_BOTTOM, string(width, '='));
	view.Write(view.TL_BOTTOM, ":: W, A, S ,D �� �̵����ּ���...");
	if (gKeyManager.inputResult == KeyManager::InputResult::FAIL)
	{
		view.Write(view.TL_BOTTOM, "");
		view.Write(view.TL_BOTTOM, ":: [ ERROR ]");
		view.Write(view.TL_BOTTOM, ":: �Է��� �߸��Ǿ����ϴ�.");
		view.Write(view.TL_BOTTOM, ":: �ѱ۷� �Ǿ��ִٸ� ��/�� Ű�� ����� �ٲ��ּ���.");
	}

	// �信�ִ°� ���
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();
}

void InGameDungeon::Release()
{
}