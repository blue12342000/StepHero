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
#pragma region ´øÀüÅÛÇÃ¸´
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

	gTextViewManager.Init(100, 50);
	gTextViewManager.AddTextView(gTextViewManager.VT_TITLE, new GameTitle(100, 50));
	gTextViewManager.AddTextView(gTextViewManager.VT_INGAME, new InGameDungeon(100, 50));
	gTextViewManager.AddTextView(gTextViewManager.VT_BATTLE, new InGameBattle(100, 50));
	gTextViewManager.AddTextView(gTextViewManager.VT_SHOP, new GeneralShop(100, 50));
	gTextViewManager.AddTextView(gTextViewManager.VT_GAMEOVER, new GameOver(100, 50));
	gTextViewManager.AddTextView(gTextViewManager.VT_ENDING, new GameEnding(100, 50));
	gTextViewManager.AddTextView(gTextViewManager.VT_ESCAPE, new GameEscape(100, 50));

	gTextViewManager.ChangeView(gTextViewManager.VT_TITLE, gTextViewManager.AT_FADE_OUT_IN, 1000);
}

void StepHero::Update()
{
	gTextViewManager.Update();
}

void StepHero::Render()
{
	gTextViewManager.Render();
}

bool StepHero::IsEnd()
{
	return false;
}

void StepHero::Release()
{

}
