// 20210304_practice2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "stdafx.h"
#include "StepHero.h"

#include "FileManager.h"
#include "KeyManager.h"
#include "DungeonTemplate.h"
#include "TextViewManager.h"
#include "GameData.h"

DungeonTemplate gDungeonTemplate[10];
MonsterTable gMonsterTable;
KeyManager gKeyManager;
TextViewManager gTextViewManager;
Difficulty difficulty;
GameData gGameData;

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

	vector<FunctionPtr> questClearFnList;
	vector<FunctionPtr> questFailedFnList;

	QuestNode() { questClearFnList.push_back([]() {}); }
	QuestNode(int id, string name, string desc):id(id),name(name),desc(desc) { }

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

/*
	해야할것.
	1. 배틀 패배 승리 구현 ㅇ
	2. 배틀 이후 상점 구현 ㅇ
	3. 탈출구로 탈출시 엔딩으로 ㅇ
	4. TextRender에서 팝업, ALERT창을 나오게 할수있게 구현해놔야함 ㅇ
	5. 퀘스트
	6. 장비상점
	7. 인벤토리
	8. 세이브 / 로드
	9. Dungeon 객채 캡슐화
	10. Hero 클래스 정리
	11. Release 메모리 누수되는 애들 처리
	12, 던전 템플릿 클래스 구조 변경 & ini파일 읽어오게 변경
*/

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
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴
