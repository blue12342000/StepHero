#include "stdafx.h"
#include "GameEscape.h"


GameEscape::GameEscape()
{
}

GameEscape::GameEscape(int width, int height)
{
	this->width = width;
	this->height = height;
	view.Init(width, height);
	view.AddLayout(view.TL_TOP, view.RT_STATIC, 0, 11);
	view.AddLayout(view.TL_CONTENT, view.RT_EXIST_DATA, 11, 39);

	view.Write(view.TL_TOP, " ");
	view.Write(view.TL_TOP, " ");
	view.Write(view.TL_TOP, " ");
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "     #####     #####      #####         #        #####     #####     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "     #         #          #            # #       #   #     #         "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "     ###       #####      #           #   #      #####     ###       "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "     #             #      #           #####      #         #         "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "     #             #      #           #   #      #         #         "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "     #####     #####      #####       #   #      #         #####     "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
}

GameEscape::~GameEscape()
{
}

void GameEscape::Init()
{
}

void GameEscape::Update()
{
}

void GameEscape::Render(vector<string>* targetBuffer)
{
	// 뷰에있는걸 출력
	view.Refresh();
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();
}

void GameEscape::Release()
{
}
