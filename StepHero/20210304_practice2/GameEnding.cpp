#include "stdafx.h"
#include "GameEnding.h"


GameEnding::GameEnding()
{
}

GameEnding::GameEnding(int width, int height)
{
	this->width = width;
	this->height = height;
	view.Init(width, height);
	view.AddLayout(view.TL_TOP, view.RT_STATIC, 0, 8);
	view.AddLayout(view.TL_CONTENT, view.RT_EXIST_DATA, 8, 42);

	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "                                                                     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "                                                                     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "                                                                     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "                                                                     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "                                                                     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "                                                                     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
}

GameEnding::~GameEnding()
{
}

void GameEnding::Init()
{
}

void GameEnding::Update()
{
}

void GameEnding::Render(vector<string>* targetBuffer)
{
	// 뷰에있는걸 출력
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();
}

void GameEnding::Release()
{
}
