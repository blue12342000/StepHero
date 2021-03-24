#include "stdafx.h"
#include "GameOver.h"


GameOver::GameOver()
{
}

GameOver::GameOver(int width, int height)
{
	this->width = width;
	this->height = height;
	view.Init(width, height);
	view.AddLayout(view.TL_TOP, view.RT_STATIC, 0, 11);
	view.AddLayout(view.TL_CONTENT, view.RT_STATIC, 11, 39);

	view.Write(view.TL_TOP, " ");
	view.Write(view.TL_TOP, " ");
	view.Write(view.TL_TOP, " ");
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#   #    ###    #     #       #####    #####   #####"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#   #   #   #   #     #       #    #     #     #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, " # #   #     #  #     #       #     #    #     ###  "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "  #    #     #  #     #       #     #    #     #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "  #     #   #   #     #       #    #     #     #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "  #      ###     #####        #####    #####   #####"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));

	view.Write(view.TL_CONTENT, " ");
	view.Write(view.TL_CONTENT, " ");
	view.Write(view.TL_CONTENT, " ");
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "                 ###########                 "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "              #################              "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "           #######################           "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "        #############################        "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "      #################################      "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "    #####################################    "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "   #######################################   "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "  #########################################  "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, " ########################################### "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, " ########################################### "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, " ################  ######################### "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "###############    ##########################"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "##############    ###########################"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "#######          ###########          #######"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "######            #########            ######"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "######            #########            ######"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "######            #########            ######"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, " ######          ###########          ###### "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, " ######    ################################# "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, " #####   ################################### "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, " ####################    ################### "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "   #################      ################   "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "      ##############      #############      "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "            ########      ########           "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "            ######################           "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "            ######################           "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "            ######################           "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "             ####################            "));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "                 ############                "));
}

GameOver::~GameOver()
{
}

void GameOver::Init()
{
}

void GameOver::Update()
{
}

void GameOver::Render(vector<string>* targetBuffer)
{
	// 뷰에있는걸 출력
	view.Refresh();
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();
}

void GameOver::Release()
{
}
