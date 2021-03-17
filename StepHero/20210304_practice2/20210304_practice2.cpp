// 20210304_practice2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "stdafx.h"
#include "StepHero.h"

#include "FileManager.h"
#include "KeyManager.h"
#include "TextRender.h"
#include "DungeonTemplate.h"

TextRender gTextRender(50, 100);
DungeonTemplate gDungeonTemplate[10];
MonsterTable gMonsterTable;
KeyManager gKeyManager;

// 0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 불, 5: 탈출구
char G_CHAR_FIELD_TYPE[6] = { '.', 'T', '~', '#', 'A', 'E' };
// 레벨별 경험치 테이블
int G_MAX_LEVEL = 15;
int G_LVL_EXP_TABLE[15][2] = { {1, 10}
							  ,{2, 10}
							  ,{3, 10}
							  ,{4, 10}
							  ,{5, 10}
							  ,{6, 10}
							  ,{7, 10}
							  ,{8, 10}
							  ,{9, 10}
							  ,{10, 50}
							  ,{11, 50}
							  ,{12, 50}
							  ,{13, 50}
							  ,{14, 50}
							  ,{15, 50} };

enum QusetType { HUNT, MOVE, ETC };
struct QuestNode
{
	int id;
	string name;
	string desc;

	void* owner = nullptr;

	FunctionPtr QuestClear = nullptr;
	FunctionPtr QuestFailed = nullptr;

	QuestNode() {}
	QuestNode(int id, string name, string desc, FunctionPtr fnQuestClear, FunctionPtr fnQuestFailed):id(id),name(name),desc(desc), QuestClear(fnQuestClear), QuestFailed(fnQuestFailed) { }

	// 선택퀘스트 - 본인과 같은 뎁스
	QuestNode* firendQuset = nullptr;
	// 이전퀘스트
	QuestNode* exChainQuset = nullptr;
	// 다음 연계 퀘스트
	QuestNode* nextChainQuset = nullptr;
};

struct QuestList
{
	int size = 0;
	QuestNode* QusetList = nullptr;
};

struct QusetTable
{
	QuestNode questNode[10];
} gQusetTable;

struct EquipItem
{

};

struct EquipShop
{

};

int main()
{
	srand(time(NULL));

	StepHero stepHero;
	stepHero.Init();
	while (1)
	{
		gKeyManager.Update();
		stepHero.Update();
		system("cls");
		stepHero.Render();

		if (stepHero.IsEnd()) break;
	}

	gTextRender.Release();

	/*
		0-1 : type 1 10x10
		2-3 : type 2 5x10
		4-5 : type 3 10x5
		6-9 : type 4 5x5
		던전의 형태 템플릿
		0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 탈출구
	*/
	/*
#pragma region 던전템플릿
	
	gDungeonTemplate[0] = { 10, 10 };
	gDungeonTemplate[0].fieldSet = new int* [gDungeonTemplate[0].rows];
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
	gDungeonTemplate[1].fieldSet = new int* [gDungeonTemplate[1].rows];
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
	gDungeonTemplate[4].fieldSet = new int* [gDungeonTemplate[4].rows];
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
	gDungeonTemplate[5].fieldSet = new int* [gDungeonTemplate[5].rows];
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
	*/
	//srand(time(NULL));
	//
	//system("MODE CON COLS=101 LINES=51");
	//
	//// 난이도 정보
	//Difficulty difficulty;
	//int difficultyInput = 0;
	//bool isUseShop = false;
	//int isMonsterMove = 0;
	//Hero player;
	//Dungeon dungeon;
	//dungeon.notice[0] = "* 던전에서 탈출에 성공하세요 *";
	//dungeon.notice[1] = "";
	//dungeon.notice[2] = "  O : 플레이어";
	//dungeon.notice[3] = "  E : 탈출구";
	//dungeon.notice[4] = "  . : 공터, T : 숲, ~ : 늪";
	//dungeon.notice[5] = "  # : 벽, A : 햇불";
	//dungeon.notice[6] = "";
	//dungeon.notice[7] = "  P : 몬스터위치 확인";
	//dungeon.notice[8] = "  M : 소지금 1000골드 UP";
	//dungeon.noticeCount = 9;
	//
	//// 맵 디버그 모드
	//bool debug = false;
	//
	//// 타이틀
	//bool isInitalized = false;
	//int initalizeStep = 0;
	//while (!isInitalized)
	//{
	//	system("cls");
	//
	//	cout << "\n\n\n";
	//	cout << "      ----------------------------------------------------------     " << endl;
	//	cout << "---------------------------------------------------------------------" << endl;
	//	cout << "      #####  #####  #####  #####      #   #  #####  #####  #####     " << endl;
	//	cout << "      #        #    #      #   #      #   #  #      #   #  #   #     " << endl;
	//	cout << "      #####    #    ###    #####      #####  ###    #####  #   #     " << endl;
	//	cout << "          #    #    #      #          #   #  #      # #    #   #     " << endl;
	//	cout << "          #    #    #      #          #   #  #      #  #   #   #     " << endl;
	//	cout << "      #####    #    #####  #          #   #  #####  #   #  #####     " << endl;
	//	cout << "---------------------------------------------------------------------" << endl;
	//	cout << "     -----------------------------------------------------------     " << endl;
	//	cout << "\n\n";
	//	
	//	cout << "                                ------                               " << endl;
	//	cout << "                              ----  ----                             " << endl;
	//	cout << "                                                                     " << endl;
	//
	//	cout << right << setw(35 + difficulty.name[difficultyInput].length() / 2) << difficulty.name[difficultyInput] << endl;
	//	
	//	if (initalizeStep == 0)
	//	{
	//		cout << "                                                                     " << endl;
	//		cout << "                    CHANGE :: W,S  SELECT :: ENTER                   " << endl;
	//	}
	//	cout << "                                                                     " << endl;
	//	cout << "                              ----  ----                             " << endl;
	//	cout << "                                ------                               " << endl;
	//
	//	if (initalizeStep == 0)
	//	{
	//		switch (_getch())
	//		{
	//		case 'w':
	//		case 'W':
	//			difficultyInput = (difficultyInput + 2) % 3;
	//			break;
	//		case 's':
	//		case 'S':
	//			difficultyInput = (difficultyInput + 1) % 3;
	//			break;
	//		case 13:
	//			++initalizeStep;
	//
	//			// 플레이어의 난이도별 능력치 설정
	//			dungeon.rows = difficulty.dungeonSize[difficultyInput];
	//			dungeon.cols = difficulty.dungeonSize[difficultyInput];
	//			player = { difficulty.heroHP[difficultyInput], difficulty.heroHP[difficultyInput], 1000, 0, 0, 25, 1, 0, 3.5f };
	//			break;
	//		case 27:
	//			// ESC 입력 게임종료;
	//			return 0;
	//			break;
	//		default:
	//			if (_kbhit())
	//			{
	//				_getch();
	//				cout << ":::::  ERROR  :::::" << endl;
	//				cout << ":::::  입력이 잘못되었습니다.  :::::" << endl;
	//				cout << ":::::  만약 한글로 되어있다면 한/영키로 영어로 바꿔주세요  :::::" << endl;
	//				_getch();
	//			}
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		// 영웅의 이름을 입력해주세요.
	//		cout << "\n               영웅의 이름을 입력해주세요. :: ";
	//		cin >> player.name;
	//
	//		// 초기정보 입력 끝
	//		isInitalized = true;
	//
	//		gQusetTable.questNode[0] = QuestNode(1, "이동해보세요", "W A S D 를 이용해서 이동해보세요", [](void* obj) { ((Hero*)obj)->RootExp(10); }, nullptr);
	//		gQusetTable.questNode[0] = QuestNode(1, "이동해보세요", "W A S D 를 이용해서 이동해보세요", [](void* obj) { ((Hero*)obj)->RootExp(10); }, nullptr);
	//		gQusetTable.questNode[0] = QuestNode(1, "이동해보세요", "W A S D 를 이용해서 이동해보세요", [](void* obj) { ((Hero*)obj)->RootExp(10); }, nullptr);
	//		gQusetTable.questNode[0] = QuestNode(1, "이동해보세요", "W A S D 를 이용해서 이동해보세요", [](void* obj) { ((Hero*)obj)->RootExp(10); }, nullptr);
	//		gQusetTable.questNode[0] = QuestNode(1, "이동해보세요", "W A S D 를 이용해서 이동해보세요", [](void* obj) { ((Hero*)obj)->RootExp(10); }, nullptr);
	//	}
	//}
	//
	//// 상점의 정보
	//Shop shop;
	//shop.items[0] = { 100, 999, 5, "전체회복" };
	//shop.items[1] = { 10, 10, 10, "10회복" };
	//shop.items[2] = { 20, 20, 20, "20회복" };
	//shop.items[3] = { 30, 30, 30, "30회복" };
	//shop.items[4] = { 40, 40, 40, "40회복" };
	//
	//// 배틀정보
	//BattleInfo battleInfo;
	//battleInfo.item[0] = { "가위" };
	//battleInfo.item[1] = { "바위" };
	//battleInfo.item[2] = { " 보 " };
	//
	//// 몬스터 설정
	//MonsterTable monsterTable;
	//int mStrength = difficulty.monsterStrength[difficultyInput];
	//
	//// 0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 탈출구
	//monsterTable.monster[FieldType::empty][0] = { 10 * mStrength, 10 * mStrength, 10 * mStrength, 20, 30, 0, "두더지" };
	//monsterTable.monster[FieldType::empty][1] = { 12 * mStrength, 20 * mStrength, 20 * mStrength, 25, 50, 0, "슬라임" };
	//monsterTable.monster[FieldType::empty][2] = { 14 * mStrength, 30 * mStrength, 30 * mStrength, 30, 80, 0, "도적" };
	//monsterTable.monster[FieldType::empty][3] = { 16 * mStrength, 40 * mStrength, 40 * mStrength, 35, 100, 0, "산적" };
	//
	//monsterTable.monster[FieldType::wood][0] = { 10 * mStrength, 10 * mStrength, 10 * mStrength, 20, 30, 0, "토끼" };
	//monsterTable.monster[FieldType::wood][1] = { 12 * mStrength, 20 * mStrength, 20 * mStrength, 25, 50, 0, "사슴" };
	//monsterTable.monster[FieldType::wood][2] = { 14 * mStrength, 30 * mStrength, 30 * mStrength, 30, 80, 0, "늑대" };
	//monsterTable.monster[FieldType::wood][3] = { 16 * mStrength, 40 * mStrength, 40 * mStrength, 35, 100, 0, "늑대인간" };
	//
	//monsterTable.monster[FieldType::swamp][0] = { 10 * mStrength, 10 * mStrength, 10 * mStrength, 20, 30, 0, "두꺼비" };
	//monsterTable.monster[FieldType::swamp][1] = { 12 * mStrength, 20 * mStrength, 20 * mStrength, 25, 50, 0, "뱀" };
	//monsterTable.monster[FieldType::swamp][2] = { 14 * mStrength, 30 * mStrength, 30 * mStrength, 30, 80, 0, "저주받은 나무" };
	//monsterTable.monster[FieldType::swamp][3] = { 16 * mStrength, 40 * mStrength, 40 * mStrength, 35, 100, 0, "식인식물" };
	//
	//dungeon.CreateDungeon(difficulty.dungeonSize[difficultyInput], difficultyInput);
	//dungeon.SetMonsters(difficulty.monsterEncounter[difficultyInput], &player);
	//
	//while (1)
	//{
	//	system("cls");
	//
	//	// 맵 출력
	//	cout << "---------------------------------------------------------------------" << endl;
	//	cout << "      #####  #####  #####  #####      #   #  #####  #####  #####     " << endl;
	//	cout << "      #        #    #      #   #      #   #  #      #   #  #   #     " << endl;
	//	cout << "      #####    #    ###    #####      #####  ###    #####  #   #     " << endl;
	//	cout << "          #    #    #      #          #   #  #      # #    #   #     " << endl;
	//	cout << "          #    #    #      #          #   #  #      #  #   #   #     " << endl;
	//	cout << "      #####    #    #####  #          #   #  #####  #   #  #####     " << endl;
	//	cout << "---------------------------------------------------------------------" << endl;
	//	dungeon.PrintDungeon(&player, debug);
	//
	//	if (dungeon.room[player.posY][player.posX].fieldType == FieldType::out)
	//	{
	//		// 탈출
	//		cout << "---------------------------------------------------------------------" << endl;
	//		cout << "     #####     #####      #####         #        #####     #####     " << endl;
	//		cout << "     #         #          #            # #       #   #     #         " << endl;
	//		cout << "     ###       #####      #           #   #      #####     ###       " << endl;
	//		cout << "     #             #      #           #####      #         #         " << endl;
	//		cout << "     #             #      #           #   #      #         #         " << endl;
	//		cout << "     #####     #####      #####       #   #      #         #####     " << endl;
	//		cout << "---------------------------------------------------------------------" << endl;
	//		break;
	//	}
	//	/*
	//	else if (player.state == HeroState::IDLE && dungeon.room[player.posY][player.posX].monster)
	//	{
	//		// 현재 사용하지 않는 전투
	//		player.state = 1;
	//
	//		// 몬스터가 있는 위치에 들어옴
	//		Monster& monster = *(dungeon.room[player.posY][player.posX].monster);
	//		battleInfo.PrintBattleStatus(player, monster);
	//		cout << right << setw(53) << "******* 몬스터와 조우하였다!! *******" << endl << endl;
	//		int playerInput = rand() % 3;
	//		int monsterInput = rand() % 3;
	//		string playerBattleLogStr = player.name + " [[ " + battleInfo.item[playerInput].name + " ]]";
	//		string monsterBattleLogStr = "[[ " + battleInfo.item[monsterInput].name + " ]] " + monster.name;
	//
	//		cout << right << setw(45) << "*** 가위! 바위! 보! ***" << endl;
	//		while (playerInput == monsterInput)
	//		{
	//			// 무승부 처리
	//			cout << right << setw(31) << playerBattleLogStr << "  VS  " << monsterBattleLogStr << endl;
	//			cout << right << setw(42) << "*** 무 승 부 ***" << endl;
	//			
	//			playerInput = rand() % 3;
	//			monsterInput = rand() % 3;
	//			playerBattleLogStr = player.name + " [[ " + battleInfo.item[playerInput].name + " ]]";
	//			monsterBattleLogStr = "[[ " + battleInfo.item[monsterInput].name + " ]] " + monster.name;
	//		}
	//
	//		cout << right << setw(31) << playerBattleLogStr << "  VS  " << monsterBattleLogStr << endl;
	//		if (playerInput == (monsterInput + 1) % 3)
	//		{
	//			// 승리 처리
	//			cout << right << setw(42) << "***  승  리  ***" << endl;
	//			cout << "---------------------------------------------------------------------" << endl;
	//			cout << ":::: 몬스터와의 전투에서 승리하였다" << endl;
	//			if (monster.HitDamage(player.attack) == 1)
	//			{
	//				cout << ":::: " << monster.rootExp << " 경험치 획득!!" << endl;
	//				cout << ":::: " << monster.rootGold << " 골드 획득!!" << endl;
	//
	//				// 죽었는가?
	//				player.gold += monster.rootGold;
	//				int lvlUp = player.RootExp(monster.rootExp);
	//				if (lvlUp)
	//				{
	//					cout << ":::: 플레이어 레벨이 " << lvlUp << " 올랐습니다." << endl;
	//				}
	//
	//				delete &monster;
	//				dungeon.room[player.posY][player.posX].monster = nullptr;
	//			}
	//			else
	//			{
	//				cout << ":::: 몬스터에게 " << player.attack << " 데미지를 주었다." << endl;
	//				cout << ":::: 몬스터가 도망갔습니다." << endl;
	//			}
	//		}
	//		else
	//		{
	//			// 패배 처리
	//			cout << right << setw(42) << "***  패  배  ***" << endl;
	//			cout << "---------------------------------------------------------------------" << endl;
	//			cout << ":::: 몬스터와의 전투에서 패배하였다" << endl;
	//			cout << ":::: " << monster.attack << "의 데미지를 받았다!!" << endl;
	//
	//			// 플레이어의 상태가 죽은상태인가?
	//			if (player.HitDamage(monster.attack) == 3)
	//			{
	//				cout << "---------------------------------------------------------------------" << endl;
	//				cout << "        #   #    ###    #     #       #####    #####   #####         " << endl;
	//				cout << "        #   #   #   #   #     #       #    #     #     #             " << endl;
	//				cout << "         # #   #     #  #     #       #     #    #     ###           " << endl;
	//				cout << "          #    #     #  #     #       #     #    #     #             " << endl;
	//				cout << "          #     #   #   #     #       #    #     #     #             " << endl;
	//				cout << "          #      ###     #####        #####    #####   #####         " << endl;
	//				cout << "---------------------------------------------------------------------" << endl;
	//				break;
	//			}
	//		}
	//
	//		cout << "---------------------------------------------------------------------" << endl;
	//		cout << ":::: 상점을 이용하시겠습니까? ( 0. 이용하지 않는다 / 1 : 이용한다 ) : ";
	//		cin >> isUseShop;
	//	}
	//	*/
	//	else if (player.state == HeroState::IDLE && dungeon.room[player.posY][player.posX].isMonster)
	//	{
	//		// 현재 사용하지 않는 전투
	//		player.state = HeroState::BATTLE;
	//
	//		// 몬스터가 있는 위치에 들어옴
	//		Monster monster = monsterTable.GetMonster(dungeon.room[player.posY][player.posX].fieldType);
	//		cout << right << setw(53) << "******* 몬스터와 조우하였다!! *******" << endl << endl;
	//		while (1)
	//		{
	//			battleInfo.PrintBattleStatus(player, monster);
	//			int playerInput = rand() % 3;
	//			int monsterInput = rand() % 3;
	//			string playerBattleLogStr = player.name + " [[ " + battleInfo.item[playerInput].name + " ]]";
	//			string monsterBattleLogStr = "[[ " + battleInfo.item[monsterInput].name + " ]] " + monster.name;
	//
	//			cout << right << setw(45) << "*** 가위! 바위! 보! ***" << endl;
	//
	//			// 몬스터에게 패배하거나 몬스터가 죽을때까지 승부
	//			while (playerInput == monsterInput)
	//			{
	//				// 무승부 처리
	//				cout << right << setw(31) << playerBattleLogStr << "  VS  " << monsterBattleLogStr << endl;
	//				cout << right << setw(42) << "*** 무 승 부 ***" << endl;
	//
	//				playerInput = rand() % 3;
	//				monsterInput = rand() % 3;
	//				playerBattleLogStr = player.name + " [[ " + battleInfo.item[playerInput].name + " ]]";
	//				monsterBattleLogStr = "[[ " + battleInfo.item[monsterInput].name + " ]] " + monster.name;
	//			}
	//
	//			cout << right << setw(31) << playerBattleLogStr << "  VS  " << monsterBattleLogStr << endl;
	//			if (playerInput == (monsterInput + 1) % 3)
	//			{
	//				// 승리 처리
	//				cout << right << setw(42) << "***  승  리  ***" << endl;
	//				cout << "---------------------------------------------------------------------" << endl;
	//				cout << ":::: 몬스터와의 전투에서 승리하였다" << endl;
	//				if (monster.HitDamage(player.attack) == 1)
	//				{
	//					// 죽었는가?
	//					cout << ":::: " << monster.rootExp << " 경험치 획득!!" << endl;
	//					cout << ":::: " << monster.rootGold << " 골드 획득!!" << endl;
	//
	//					player.gold += monster.rootGold;
	//					int lvlUp = player.RootExp(monster.rootExp);
	//					if (lvlUp)
	//					{
	//						cout << ":::: 플레이어 레벨이 " << lvlUp << " 올랐습니다." << endl;
	//					}
	//					dungeon.room[player.posY][player.posX].isMonster = false;
	//					break;
	//				}
	//				else
	//				{
	//					cout << ":::: 몬스터에게 " << player.attack << " 데미지를 주었다." << endl;
	//				}
	//			}
	//			else
	//			{
	//				// 패배 처리
	//				cout << right << setw(42) << "***  패  배  ***" << endl;
	//				cout << "---------------------------------------------------------------------" << endl;
	//				cout << ":::: 몬스터와의 전투에서 패배하였다" << endl;
	//				cout << ":::: " << monster.attack << "의 데미지를 받았다!!" << endl;
	//				if (player.HitDamage(monster.attack) == HeroState::DIE) break;
	//			}
	//		}
	//
	//		// 플레이어의 상태가 죽은상태인가?
	//		if (player.state == HeroState::DIE)
	//		{
	//			cout << "---------------------------------------------------------------------" << endl;
	//			cout << "        #   #    ###    #     #       #####    #####   #####         " << endl;
	//			cout << "        #   #   #   #   #     #       #    #     #     #             " << endl;
	//			cout << "         # #   #     #  #     #       #     #    #     ###           " << endl;
	//			cout << "          #    #     #  #     #       #     #    #     #             " << endl;
	//			cout << "          #     #   #   #     #       #    #     #     #             " << endl;
	//			cout << "          #      ###     #####        #####    #####   #####         " << endl;
	//			cout << "---------------------------------------------------------------------" << endl;
	//			break;
	//		}
	//
	//		cout << "---------------------------------------------------------------------" << endl;
	//		cout << ":::: 상점을 이용하시겠습니까? ( 0. 이용하지 않는다 / 1 : 이용한다 ) : ";
	//		cin >> isUseShop;
	//	}
	//	
	//	if (isUseShop)
	//	{
	//		player.state = HeroState::SHOP;
	//
	//		// 상점이용
	//		int selectItem = 0;
	//		int itemCount = sizeof(shop.items) / sizeof(ShopItem);
	//		while (1)
	//		{
	//			system("cls");
	//			cout << "---------------------------------------------------------------------" << endl;
	//			cout << " #####  #####  #####  #####  #####  #   #    ##### #   # ##### ##### " << endl;
	//			cout << " #   #  #   #    #      #    #   #  ##  #    #     #   # #   # #   # " << endl;
	//			cout << " #####  #   #    #      #    #   #  # # #    ##### ##### #   # ##### " << endl;
	//			cout << " #      #   #    #      #    #   #  # # #        # #   # #   # #     " << endl;
	//			cout << " #      #   #    #      #    #   #  #  ##        # #   # #   # #     " << endl;
	//			cout << " #      #####    #    #####  #####  #   #    ##### #   # ##### #     " << endl;
	//			cout << "---------------------------------------------------------------------" << endl;
	//
	//			cout << "\n\n                  ---------------------------------                  " << endl;
	//			for (int i = 0; i < itemCount; ++i)
	//			{
	//				ShopItem& item = shop.items[i];
	//				string itemLabel = to_string((i + 1)) + ". " + item.name + " (" + to_string(item.gold) + " GOLD)\t남은수량 : " + to_string(item.qty);
	//				if (i == selectItem)
	//				{
	//					itemLabel = ">>> " + itemLabel + " <<<";
	//					cout << right << setw(12 + itemLabel.length()) << itemLabel << endl;
	//				}
	//				else
	//				{
	//					cout << right << setw(17 + itemLabel.length()) << itemLabel << endl;
	//				}
	//			}
	//			cout << setw((selectItem == itemCount)?29:30) << ((selectItem == itemCount) ? ">>> " : "") << (itemCount+1) << ". 나가기" << ((selectItem == itemCount) ? " <<<" : "") << endl;
	//			cout << "                  ---------------------------------                  " << endl;
	//			cout << setw(50) << "CHANGE :: W,S  SELECT :: ENTER" << endl << endl;
	//
	//			player.PrintHeroStatus();
	//			
	//			switch (_getch())
	//			{
	//			case 'w':
	//			case 'W':
	//				selectItem = (selectItem + itemCount) % (itemCount + 1);
	//				break;
	//			case 's':
	//			case 'S':
	//				selectItem = (selectItem + 1) % (itemCount + 1);
	//				break;
	//			case 13:
	//				// 상점물품 선택
	//				if (selectItem < itemCount)
	//				{ 
	//					if (player.IsPossibleUseGold(shop.items[selectItem].gold))
	//					{
	//						// 해당 골드는 갖고있다.
	//						if (shop.IsBuyItem(selectItem))
	//						{
	//							// 물건이 정상적으로 사졌다
	//							const ShopItem& item = shop.BuyItem(selectItem);
	//							player.UseGold(item.gold);
	//							player.UseItem(item);
	//							cout << left << ":::: 소지금 " << item.gold << " GOLD 사용하였습니다." << endl;
	//							cout << "---------------------------------------------------------------------" << endl;
	//							cout << right << setw(43 + item.name.length()) << string("******* 체력 " + item.name + " *******") << endl;
	//						}
	//						else
	//						{
	//							// 물건이 없어서 구매 불가
	//							cout << left << ":::: 품절된 상품입니다 " << endl;
	//							cout << "---------------------------------------------------------------------" << endl;
	//						}
	//					}
	//					else cout << right << setw(54) << "******* 소지금이 부족합니다!!! *******" << endl << endl;
	//				}
	//				else
	//				{
	//					isUseShop = false;
	//					cout << right << setw(51) << "******* 상점을 나갔습니다 *******" << endl << endl;
	//				}
	//			
	//				cout << "::::  아무키나 누르세요.... ";
	//				_getch();
	//				break;
	//			default:
	//				if (_kbhit())
	//				{
	//					_getch();
	//					cout << endl;
	//					cout << ":::::  ERROR  :::::" << endl;
	//					cout << ":::::  입력이 잘못되었습니다.  :::::" << endl;
	//					cout << ":::::  만약 한글로 되어있다면 한/영키로 영어로 바꿔주세요  :::::" << endl;
	//					_getch();
	//				}
	//				break;
	//			}
	//			// 상점을 사용안할거라면 상점에서 나가기
	//			if (!isUseShop) break;
	//		}
	//
	//		// 상점에서 나온 후 지워진 맵정보 재출력
	//		system("cls");
	//		cout << "---------------------------------------------------------------------" << endl;
	//		cout << "      #####  #####  #####  #####      #   #  #####  #####  #####     " << endl;
	//		cout << "      #        #    #      #   #      #   #  #      #   #  #   #     " << endl;
	//		cout << "      #####    #    ###    #####      #####  ###    #####  #   #     " << endl;
	//		cout << "          #    #    #      #          #   #  #      # #    #   #     " << endl;
	//		cout << "          #    #    #      #          #   #  #      #  #   #   #     " << endl;
	//		cout << "      #####    #    #####  #          #   #  #####  #   #  #####     " << endl;
	//		cout << "---------------------------------------------------------------------" << endl;
	//		dungeon.PrintDungeon(&player, debug);
	//	}
	//
	//	player.PrintHeroStatus();
	//	
	//	cout << "::::  이동키를 눌러주세요 (W, A, S, D)..... ";
	//	int inputKey = _getch();
	//	switch (tolower(inputKey))
	//	{
	//	case 'w':
	//		if (player.posY > 0)
	//		{
	//			if (dungeon.IsPossibleMove(player.posX, player.posY - 1))
	//			{
	//				--player.posY;
	//				++isMonsterMove;
	//				player.state = HeroState::IDLE;
	//			}
	//		}
	//		break;
	//	case 's':
	//		if (player.posY < dungeon.rows - 1)
	//		{
	//			if (dungeon.IsPossibleMove(player.posX, player.posY + 1))
	//			{
	//				++player.posY;
	//				++isMonsterMove;
	//				player.state = HeroState::IDLE;
	//			}
	//		}
	//		break;
	//	case 'a':
	//		if (player.posX > 0)
	//		{
	//			if (dungeon.IsPossibleMove(player.posX - 1, player.posY))
	//			{
	//				--player.posX;
	//				++isMonsterMove;
	//				player.state = HeroState::IDLE;
	//			}
	//		}
	//		break;
	//	case 'd':
	//		if (player.posX < dungeon.cols - 1)
	//		{
	//			if (dungeon.IsPossibleMove(player.posX + 1, player.posY))
	//			{
	//				++player.posX;
	//				++isMonsterMove;
	//				player.state = HeroState::IDLE;
	//			}
	//		}
	//		break;
	//	case 'p':
	//		debug = !debug;
	//		break;
	//	case 'm':
	//		player.gold += 1000;
	//		break;
	//	default:
	//		if (_kbhit())
	//		{
	//			_getch();
	//			cout << endl;
	//			cout << ":::::  입력이 잘못되었습니다.  :::::" << endl;
	//			cout << ":::::  만약 한글로 되어있다면 한/영키로 영어로 바꿔주세요  :::::" << endl;
	//			_getch();
	//		}
	//	}
	//
	//	if (isMonsterMove >= 2)
	//	{
	//		// 몬스터 이동
	//		isMonsterMove = 0;
	//		for (int i = 0; i < dungeon.rows; i++)
	//		{
	//			for (int j = 0; j < dungeon.cols; j++)
	//			{
	//				Room& room = dungeon.room[i][j];
	//				if (room.isMonster)
	//				{
	//					Room* next = room.next[rand() % 4];
	//					if (next && next->fieldType != FieldType::out && next->fieldType != FieldType::wall && next->fieldType != FieldType::fire)
	//					{
	//						swap(room.isMonster, next->isMonster);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//
	//// 메모리 해제
	//player.Release();
	//dungeon.Release();
	//for (int i = 0; i < 10; ++i) gDungeonSet[i].Release();
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴
