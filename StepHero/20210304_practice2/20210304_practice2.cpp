// 20210304_practice2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iomanip>
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <map>
#include <functional>

using namespace std;
using FunctionPtr = function<void(void*)>;

// 0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 불, 5: 탈출구
char G_CHAR_FIELD_TYPE[6] = {'.', 'T', '~', '#', 'A', 'E'};
enum FieldType { empty=0, wood, swamp, wall, fire, out, eof};
enum HeroState { IDLE = 0, BATTLE, SHOP, DIE };

enum QusetType { HUNT, MOVE, ETC };

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

struct InputKeyRequest
{
	int size = 0;
	char* keyGroup = nullptr;

	InputKeyRequest() {}
	InputKeyRequest(int size, char* keyGroup) :size(size), keyGroup(keyGroup) {}

	void Release()
	{
		if (keyGroup) delete[] keyGroup;
	}
};

struct InputKey
{
	enum InputType
	{
		ANYKEYS = 0, SELECT, INGAME, NONE
	};

	enum InputResult
	{
		SUCCESS = 0, FAIL, EMPTY
	};

	char keys[256];
	InputResult inputResult = InputResult::EMPTY;
	InputKeyRequest inputKeyRequest[InputType::NONE];

	int currRequest = 0;
	int requestCount = 0;
	InputType requestBuffer[5];

	InputKey()
	{
		inputKeyRequest[InputType::SELECT] = InputKeyRequest(3, new char[3]{ 'w', 's', 13 });
		inputKeyRequest[InputType::INGAME] = InputKeyRequest(5, new char[5]{ 'w', 'a', 's', 'd', 13 });
	}

	void Requset(InputType inputType)
	{
		requestBuffer[(currRequest + requestCount + 1) % 5] = inputType;
		++requestCount;
	}

	void Clear()
	{
		currRequest = 0;
		requestCount = 0;
	}

	void Update()
	{
		inputResult = InputResult::EMPTY;
		if (requestCount > 0)
		{
			bool isOK = false;
			char inputChar = _getch();
			if (_kbhit()) _getch();
			switch (requestBuffer[currRequest])
			{
			case InputType::ANYKEYS:
				isOK = true;
				break;
			case InputType::SELECT:
			case InputType::INGAME:
				for (int i = 0; i < inputKeyRequest[requestBuffer[currRequest]].size; ++i)
				{
					if (inputChar == inputKeyRequest[requestBuffer[currRequest]].keyGroup[i])
					{
						isOK = true;
					}
				}
				break;
			}

			inputResult = InputResult::FAIL;
			if (isOK)
			{
				inputResult = InputResult::SUCCESS;
				currRequest = (currRequest + 1) % 5;
				--requestCount;
			}
		}
	}

} gInputKey;

struct TextBuffer
{
	enum TextAlign
	{
		LEFT, CENTER, RIGHT
	};

	bool isRefreshed = false;
	TextAlign textAlign = TextAlign::LEFT;

	int offset = 0;
	int rows = 0;
	int cols = 0;

	int currPeek = 0;
	int dataCount = 0;
	int lastRenderLine = 0;
	string buffer[100];
	
	TextBuffer() {}
	TextBuffer(TextAlign align, int offset, int rows, int cols, bool isRefreshed):textAlign(align), offset(offset), rows(rows), cols(cols), isRefreshed(isRefreshed){}

	string* GetLine(int readPeek)
	{
		int localY = readPeek - offset;
		if (localY < 0 || localY >= dataCount) return nullptr;
		lastRenderLine = localY;
		return &buffer[(currPeek + localY) % 100];
	}

	void Refresh()
	{
		if (!isRefreshed)
		{
			currPeek = (currPeek + lastRenderLine + 1) % 100;
			dataCount -= lastRenderLine;
		}
	}

	void PushBack(string str)
	{
		buffer[currPeek + dataCount] = string(cols, ' ');

		int size = (str.length() < cols) ? str.length() : cols;
		int begin = 0;
		switch (textAlign)
		{
		case TextAlign::LEFT:
			break;
		case TextAlign::CENTER:
			begin = (cols - size) / 2;
			break;
		case TextAlign::RIGHT:
			begin = cols - size;
			break;
		}
		buffer[currPeek + dataCount].replace(begin, size, str);
		++dataCount;
	}

	void Release()
	{

	}
};

struct TextLayout
{
	enum LayoutKind
	{
		TITLE = 0, INGAME, NONE
	};

	enum LayoutPos
	{
		TOP = 0, CONTENT, BOTTOM
	};

	int size = 0;

	TextBuffer** textBufferTree = nullptr;
	map<LayoutPos, TextBuffer> textBufferMap;

	TextLayout() {}

	void Initialize(LayoutKind layoutKind)
	{
		switch (layoutKind)
		{
		case LayoutKind::TITLE:

			textBufferMap.insert(make_pair(LayoutPos::TOP, TextBuffer(TextBuffer::TextAlign::CENTER, 0, 16, 100, false)));
			textBufferMap.insert(make_pair(LayoutPos::CONTENT, TextBuffer(TextBuffer::TextAlign::CENTER, 16, 34, 100, false)));

			textBufferTree = new TextBuffer*[2]{ &textBufferMap[LayoutPos::TOP], &textBufferMap[LayoutPos::CONTENT] };
			size = 2;
			break;
		case LayoutKind::INGAME:
			break;
		}
	}

	void Refresh()
	{
		for (int i = 0; i < size; ++i)
		{
			textBufferTree[i]->Refresh();
		}
	}

	string GetLine(int readPeek)
	{
		for (int i = 0; i < size; ++i)
		{
			if (textBufferTree[i]->offset <= readPeek && readPeek < textBufferTree[i]->offset + textBufferTree[i]->rows)
			{
				string* line = textBufferTree[i]->GetLine(readPeek);
				if (line) return *line;
			}
		}
		return "";
	}

	void Release()
	{
		delete[] textBufferTree;
		for (auto p : textBufferMap)
		{
			p.second.Release();
		}
	}
};

struct TextRender
{
	int rows;
	int cols;

	string* buffer = nullptr;

	TextLayout* textLayout = nullptr;

	TextRender() {}
	TextRender(int rows, int cols) :rows(rows), cols(cols)
	{
		system(string("MODE CON COLS=" + to_string(cols+1) + " LINES=" + to_string(rows+1)).c_str());
		buffer = new string[rows];
		BufferClear();

		textLayout = new TextLayout[TextLayout::LayoutKind::NONE];
		for (int i = 0; i < TextLayout::LayoutKind::NONE; ++i)
		{
			textLayout[i].Initialize((TextLayout::LayoutKind)i);
		}
	}

	void BufferClear()
	{
		for (int i = 0; i < rows; ++i)
		{
			buffer[i] = string(cols, '-');
		}
	}
	void BufferClear(int startOffset, int lines)
	{
		for (int i = startOffset; i < lines && i < rows; ++i)
		{
			buffer[i] = string(cols, ' ');
		}
	}

	void AppendBuffer(TextLayout::LayoutKind layoutKind, TextLayout::LayoutPos layoutPos, string str)
	{
		textLayout[layoutKind].textBufferMap[layoutPos].PushBack(str);
	}

	void Refresh(TextLayout::LayoutKind layoutKind)
	{
		textLayout[layoutKind].Refresh();
	}

	void Render(TextLayout::LayoutKind layoutKind)
	{
		for (int i = 0; i < rows; ++i)
		{
			buffer[i].replace(0, cols, textLayout[layoutKind].GetLine(i));
			cout << buffer[i] << endl;
		}
	}

	void Release()
	{
		for (int i = 0; i < TextLayout::LayoutKind::NONE; ++i) textLayout[i].Release();
		delete[] textLayout;
		delete[] buffer;
	}

} gTextRender(50, 100);

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

struct DungeonSet
{
	int rows;
	int cols;

	int** fieldSet = nullptr;

	void Release()
	{
		for (int i = 0; i < rows; ++i)
		{
			delete[] fieldSet[i];
		}
		delete[] fieldSet;
	}
} gDungeonSet[10];

struct Monster
{
	int attack;
	int maxHP;
	int hp;
	int rootExp;
	int rootGold;

	// 0: LIVE, 1: DIE
	int state = 0;

	string name;

	// 추후 사용
	//Room* loc = nullptr;

	int HitDamage(int damage)
	{
		hp -= damage;
		if (hp <= 0)
		{
			hp = 0;
			state = 1;
		}
		return state;
	}
};

struct MonsterTable
{
	Monster monster[FieldType::wall][4];

	// 몬스터 생성	
	Monster* CreateMonster(int fieldType)
	{
		// 0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 탈출구
		if (fieldType == 3 || fieldType == 4) return nullptr;
		return new Monster(monster[fieldType][rand() % 4]);
	}

	Monster GetMonster(FieldType fieldType)
	{
		return monster[fieldType][rand() % 4];
	}
};

struct Room
{
	int locX;
	int locY;

	FieldType fieldType = FieldType::eof;
	//char data;

	bool isMonster = false;
	// 0 위 1 아래 2 왼쪽 3 오른쪽
	Room* next[4] = { nullptr, nullptr, nullptr, nullptr };
	Monster* monster = nullptr;

	void Release()
	{
		if (monster) delete monster;
	}
};

struct ShopItem
{
	int gold;
	int heal;
	int qty;
	string name;
};

struct Shop
{
	int posX;
	int posY;
	string name;
	ShopItem items[5];

	// 구매가 가능한가?
	bool IsBuyItem(int selectItemNo)
	{
		if (items[selectItemNo].qty > 0)
		{
			return true;
		}
		return false;
	}

	const ShopItem& BuyItem(int selectItemNo)
	{
		--items[selectItemNo].qty;
		return items[selectItemNo];
	}
};

struct Hero
{
	int maxHP;
	int hp;
	int gold;
	int posX;
	int posY;

	int attack;
	int lvl;
	int exp;

	float sight;

	string name;

	int lRows;
	int lCols;
	int** lightMap = nullptr;
	
	// 0: IDLE 1: BATTLE 2: SHOP 3: DIE
	HeroState state = HeroState::IDLE;

	int RootExp(int rootExp)
	{
		int lvlUp = 0;
		while (lvl < G_MAX_LEVEL && exp >= G_LVL_EXP_TABLE[lvl][1])
		{
			exp -= G_LVL_EXP_TABLE[lvl][1];
			++lvl;
			++lvlUp;

			// 레벨업시 풀피 회복
			hp = maxHP;
		}
		return lvlUp;
	}

	int RootGold(int rootGold)
	{
		gold += rootGold;
		return gold;
	}

	int HitDamage(int damage)
	{
		hp -= damage;
		if (hp <= 0)
		{
			hp = 0;
			state = HeroState::DIE;
		}

		return state;
	}

	bool IsPossibleUseGold(int needGold)
	{
		if (gold >= needGold) return true;
		return false;
	}

	void UseItem(const ShopItem& item)
	{
		hp += item.heal;
		if (hp > maxHP) hp = maxHP;
	}

	void UseGold(int useGold)
	{
		gold -= useGold;
	}

	void ResetLightMap()
	{
		for (int i = 0; i < lRows; ++i)
		{
			memset(lightMap[i], 0, sizeof(int) * lCols);
		}
	}

	void CreateLightMap(int rows, int cols)
	{
		lRows = rows;
		lCols = cols;

		lightMap = new int*[rows];
		for (int i = 0;i < rows; ++i)
		{
			lightMap[i] = new int[cols] {0, };
		}
	}

	void PrintHeroStatus()
	{
		cout << "---------------------------------------------------------------------" << endl;
		cout << "     [[ " << name << " ]]" << endl;
		cout << "     레벨\t: " << right << setw(10) << lvl << endl;
		cout << "     공격력\t: " << right << setw(10) << attack << endl;
		cout << "     체력\t: " << right << setw(10) << string(to_string(hp) + " / " + to_string(maxHP)) << endl;
		cout << "     경험치\t: " << right << setw(10) << exp << endl;
		cout << "     소지금\t: " << right << setw(10) << gold << endl;
		cout << "---------------------------------------------------------------------" << endl;
	}

	void Release()
	{
		for (int r = 0; r < lRows; ++r)
		{
			delete[] lightMap[r];
		}
		delete[] lightMap;
	}
};

struct Region
{
	bool isUsed = false;
	int templateNo;
	bool isFlip = false;
};

struct Dungeon
{
	int rows;
	int cols;
	int exitPosX;
	int exitPosY;

	int noticeCount;
	string notice[9];

	struct LightNode
	{
		int posX = 0;
		int posY = 0;
		float bright = 0.f;

		LightNode* next = nullptr;

		LightNode(int x, int y, float bright) :posX(x), posY(y), bright(bright) {}
	};
	struct LightList
	{
		int size = 0;
		LightNode* head = nullptr;

		void PushBack(int x, int y, float bright)
		{
			if (!head)
			{
				size = 1;
				head = new LightNode(x, y, bright);
			}
			else
			{
				++size;
				LightNode* currLight = head;
				while (currLight->next)
				{
					currLight = currLight->next;
				}
				currLight->next = new LightNode(x, y, bright);
			}
		}

		void ForEachCallSetLightMap(Dungeon* dungeon)
		{
			if (head)
			{
				LightNode* currLight = head;
				for (int i = 0; i < size; ++i, currLight = currLight->next)
				{
					dungeon->SetLightMap(currLight->posX, currLight->posY, currLight->bright);
				}
			}
		}

		void Release()
		{
			if (head)
			{
				LightNode* currLight = head;
				for (int i = 0; i < size; ++i, currLight = currLight->next)
				{
					LightNode* nextLight = currLight->next;
					delete currLight;
					currLight = nextLight;
				}
			}
		}
	} lightList;
	int** lightMap = nullptr;
	Room** room = nullptr;

	void CreateDungeon(int _size, int _difficulty)
	{
		rows = _size;
		cols = _size;

		CreateLightMap();
		room = new Room*[rows];
		for (int i = 0; i < rows; i++)		// y축
		{
			room[i] = new Room[cols];
			for (int j = 0; j < cols; j++)	// x축
			{
				room[i][j].locX = j;
				room[i][j].locY = i;
				room[i][j].monster = nullptr;
				// 0 위 1 아래 2 왼쪽 3 오른쪽
				if (i > 0)
				{
					room[i][j].next[0] = &room[i - 1][j];
					room[i - 1][j].next[1] = &room[i][j];
				}
				if (j > 0)
				{
					room[i][j].next[2] = &room[i][j - 1];
					room[i][j - 1].next[3] = &room[i][j];
				}
			}
		}

		/*
			0-1 : type 1 10x10 2x2 yx
			2-3 : type 2 5x10 1x2 yx
			4-5 : type 3 10x5 2x1 yx
			6-9 : type 4 5x5 1x1 yx
		*/
		// 던전 구축을 위한 뼈대 삽입
		// type 1 max : 2 type 3, 4 max 1;
		int regionRows = rows / 5;
		int regionCols = cols / 5;
		Region*** region = new Region**[regionRows];
		for (int i = 0; i < regionRows; ++i)
		{
			region[i] = new Region*[regionCols];
			for (int l = 0; l < regionCols; ++l)
			{
				region[i][l] = nullptr;
			}
		}

		for (int y = 0; y < regionRows; ++y)
		{
			for (int x = 0; x < regionCols; ++x)
			{
				if (region[y][x])
				{
					//cout << region[y][x]->templateNo << " ";
					continue;
				}

				bool isExpansX = false;
				bool isExpansY = false;
				bool isExpansXY = false;

				isExpansX = (x < regionCols - 2 && !region[y][x + 1]);
				isExpansY = (y < regionRows - 2 && !region[y + 1][x]);
				isExpansXY = isExpansX && isExpansY && x < regionCols - 2 && y < regionRows - 2 && region[y + 1][x + 1];

				// 10x10, 10x5, 5x10, 5x5 가 가능하다.
				/*
					0-1 : type 1 10x10 2x2 yx
					2-3 : type 2 5x10 1x2 yx
					4-5 : type 3 10x5 2x1 yx
					6-9 : type 4 5x5 1x1 yx
				*/
				if (isExpansX && !isExpansY)
				{
					// 10x10, 10x5가 불가능 type1, 2 0-1, 4-5 
					region[y][x] = new Region();
					if (rand() % 2) region[y][x]->templateNo = rand() % 2 + 2;
					else region[y][x]->templateNo = rand() % 4 + 6;
				}
				else if (!isExpansX && isExpansY)
				{
					// 10x10, 5x10가 불가능 type1, 3 0-1, 2-3
					region[y][x] = new Region();
					region[y][x]->templateNo = rand() % 5 + 4;
				}
				else
				{
					// 전부 가능
					region[y][x] = new Region();
					if (isExpansXY)
					{
						region[y][x]->templateNo = rand() % 10;
					}
					else
					{
						region[y][x]->templateNo = rand() % 4 + 6;
					}
				}
				region[y][x]->isFlip = rand() % 3 < 1;
				for (int ty = 0; ty < gDungeonSet[region[y][x]->templateNo].rows / 5; ++ty)
				{
					for (int tx = 0; tx < gDungeonSet[region[y][x]->templateNo].cols / 5; ++tx)
					{
						if (ty + y == y && tx + x == x) continue;
						region[ty + y][tx + x] = region[y][x];
					}
				}
			}
		}

		// 탈출구 삽입
		int randNum = rand() % (rows * cols - 2) + 1;
		SetExit(randNum % cols, randNum / rows);

		// 만들어진 뼈대를 기반으로 Room데이터 삽입
		for (int i = 0; i < rows; i+=5)		// y축
		{
			for (int j = 0; j < cols; j+=5)	// x축
			{
				if (region[i / 5][j / 5]->isUsed) continue;
				int templateNo = region[i / 5][j / 5]->templateNo;

				if (region[i / 5][j / 5]->isFlip)
				{
					for (int ty = i - (i / 5) * 5; ty < gDungeonSet[templateNo].rows; ++ty)
					{
						for (int tx = j - (j / 5) * 5; tx < gDungeonSet[templateNo].cols; ++tx)
						{
							if (room[i + ty][j + tx].fieldType != FieldType::out)
							{
								room[i + ty][j + tx].fieldType = (FieldType)gDungeonSet[templateNo].fieldSet[ty][gDungeonSet[templateNo].cols - 1 - tx];
							}
							if (room[i + ty][j + tx].fieldType == FieldType::fire)
							{
								lightList.PushBack(j + tx, i + ty, 2.5f);
							}
						}
					}
				}
				else
				{
					for (int ty = i - (i / 5) * 5; ty < gDungeonSet[templateNo].rows; ++ty)
					{
						for (int tx = j - (j / 5) * 5; tx < gDungeonSet[templateNo].cols; ++tx)
						{
							if (room[i + ty][j + tx].fieldType != FieldType::out)
							{
								room[i + ty][j + tx].fieldType = (FieldType)gDungeonSet[templateNo].fieldSet[ty][tx];
							}

							if (room[i + ty][j + tx].fieldType == FieldType::fire)
							{
								lightList.PushBack(j + tx, i + ty, 2.5f);
							}
						}
					}
				}
				region[i / 5][j / 5]->isUsed = true;
			}
		}

		// 템플릿 미적용된 타일 빈공간으로 채움
		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < cols; ++x)
			{
				if (room[y][x].fieldType == FieldType::eof) room[y][x].fieldType = FieldType::empty;
				room[y][x].monster = nullptr;
			}
		}

		lightList.ForEachCallSetLightMap(this);
		lightList.Release();

		// 리전 메모리 해제
		for (int i = 0; i < regionRows; ++i)
		{
			for (int l = 0; l < regionCols; ++l)
			{
				if (region[i][l])
				{
					int templateNo = region[i][l]->templateNo;
					delete region[i][l];

					for (int ry = i; ry < i + gDungeonSet[templateNo].rows / 5; ++ry)
					{
						for (int rx = l; rx < l + gDungeonSet[templateNo].cols / 5; ++rx)
						{
							region[ry][rx] = nullptr;
						}
					}
				}
			}
			delete[] region[i];
		}
		delete[] region;
	}
	void CreateLightMap()
	{
		lightMap = new int*[rows];
		for (int i = 0; i < rows; ++i)
		{
			lightMap[i] = new int[cols] {0, };
		}
	}

	void ResetLightMap()
	{
		for (int i = 0; i < rows; ++i)
		{
			memset(lightMap[i], 0, sizeof(int) * cols);
		}
	}

	void SetMonsters(int monsterEncount, Hero* player = nullptr, int safeZoneSize = 2)
	{
		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < cols; ++x)
			{
				if (player)
				{
					if (abs(player->posX - x) <= 2 && abs(player->posY - y) <= 2)
					{
						continue;
					}
				}

				if (rand() % 100 < monsterEncount) room[y][x].isMonster = true;
			}
		}
	}
	void SetExit(int _exitPosX, int _exitPosY)
	{
		exitPosX = _exitPosX;
		exitPosY = _exitPosY;
		room[exitPosY][exitPosX].fieldType = FieldType::out;
	}

	bool IsPossibleMove(int x, int y)
	{
		return room[y][x].fieldType != FieldType::wall &&  room[y][x].fieldType != FieldType::fire;
	}

	void SetLightMap(Hero* player)
	{
		// 기본 라이트맵 생성 및 초기화
		if (!player->lightMap) player->CreateLightMap(rows, cols);
		else player->ResetLightMap();

		int pX = player->posX;
		int pY = player->posY;
		float pSight = player->sight;
		player->lightMap[pY][pX] = 2;
		
		/*
			1. 플레이어의 위치에서 직선으로 상하좌우 확인
			2. 1분면부터 4분면까지 남은 구역확인
			   남은 구역은 이전의 값이 or 연산을 통해 결과값을 도출
		*/
#pragma region Step.1
		// 상
		for (int dy = pY - 1; dy >= 0 && dy >= pY - pSight; --dy)
		{
			if (room[dy][pX].fieldType != FieldType::wall && room[dy][pX].fieldType != FieldType::fire) player->lightMap[dy][pX] = 2;
			else
			{
				player->lightMap[dy][pX] = 1;
				break;
			}
		}
		// 하
		for (int dy = pY + 1; dy < rows && dy <= pY + pSight; ++dy)
		{
			if (room[dy][pX].fieldType != FieldType::wall && room[dy][pX].fieldType != FieldType::fire) player->lightMap[dy][pX] = 2;
			else
			{
				player->lightMap[dy][pX] = 1;
				break;
			}
		}
		// 좌
		for (int dx = pX - 1; dx >= 0 && dx >= pX - pSight; --dx)
		{
			if (room[pY][dx].fieldType != FieldType::wall && room[pY][dx].fieldType != FieldType::fire) player->lightMap[pY][dx] = 2;
			else
			{
				player->lightMap[pY][dx] = 1;
				break;
			}
		}
		// 우
		for (int dx = pX + 1; dx < cols && dx <= pX + pSight; ++dx)
		{
			if (room[pY][dx].fieldType != FieldType::wall && room[pY][dx].fieldType != FieldType::fire) player->lightMap[pY][dx] = 2;
			else
			{
				player->lightMap[pY][dx] = 1;
				break;
			}
		}
#pragma endregion

#pragma region Step.2
		int targetX, targetY, deltaX, deltaY;
		bool isCheckDirASC = true; // true : 위로 올라가면서 확인(y감소), false : 아래로 내려가면서 확인(y증가)
		// 1분면 : 상우
		targetX = pX + 1;
		targetY = pY - 1;
		deltaX = targetX - pX;
		deltaY = targetY - pY;
		while (deltaX * deltaX + deltaY * deltaY <= pSight * pSight)
		{
			if (targetX < 0 || targetY < 0 || targetX >= cols || targetY >= rows) break;

			bool isLightOnX = player->lightMap[targetY][targetX - 1] == 2;
			bool isLightOnY = player->lightMap[targetY + 1][targetX] == 2;
			bool isLightOnXY = player->lightMap[targetY + 1][targetX - 1] == 2;
			bool isNotObstacle = room[targetY][targetX].fieldType != FieldType::wall && room[targetY][targetX].fieldType != FieldType::fire;

			player->lightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY)) ? 2 : 0;
			if (!isNotObstacle && player->lightMap[targetY][targetX] == 2) player->lightMap[targetY][targetX] = 1;

			if (isCheckDirASC)
			{
				// y 감소하며 이동
				--targetY;
				--targetX;
				if (targetY == pY || targetX == pX)
				{
					++targetX;
					isCheckDirASC = false;
				}
			}
			else
			{
				// y 증가하며 이동
				++targetY;
				++targetX;
				if (targetY == pY || targetX == pX)
				{
					--targetY;
					isCheckDirASC = true;
				}
			}
			deltaX = targetX - pX;
			deltaY = targetY - pY;
		}
		// 2분면 : 상좌
		targetX = pX - 1;
		targetY = pY - 1;
		deltaX = targetX - pX;
		deltaY = targetY - pY;
		while (deltaX * deltaX + deltaY * deltaY <= pSight * pSight)
		{
			if (targetX < 0 || targetY < 0 || targetX >= cols || targetY >= rows) break;

			bool isLightOnX = player->lightMap[targetY][targetX + 1] == 2;
			bool isLightOnY = player->lightMap[targetY + 1][targetX] == 2;
			bool isLightOnXY = player->lightMap[targetY + 1][targetX + 1] == 2;
			bool isNotObstacle = room[targetY][targetX].fieldType != FieldType::wall && room[targetY][targetX].fieldType != FieldType::fire;

			player->lightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY))?2:0;
			if (!isNotObstacle && player->lightMap[targetY][targetX] == 2) player->lightMap[targetY][targetX] = 1;

			if (isCheckDirASC)
			{
				// y 감소하며 이동
				--targetY;
				++targetX;
				if (targetY == pY || targetX == pX)
				{
					--targetX;
					isCheckDirASC = false;
				}
			}
			else
			{
				// y 증가하며 이동
				++targetY;
				--targetX;
				if (targetY == pY || targetX == pX)
				{
					--targetY;
					isCheckDirASC = true;
				}
			}
			deltaX = targetX - pX;
			deltaY = targetY - pY;
		}
		// 3분면 : 하좌
		targetX = pX - 1;
		targetY = pY + 1;
		deltaX = targetX - pX;
		deltaY = targetY - pY;
		while (deltaX * deltaX + deltaY * deltaY <= pSight * pSight)
		{
			if (targetX < 0 || targetY < 0 || targetX >= cols || targetY >= rows) break;

			bool isLightOnX = player->lightMap[targetY][targetX + 1] == 2;
			bool isLightOnY = player->lightMap[targetY - 1][targetX] == 2;
			bool isLightOnXY = player->lightMap[targetY - 1][targetX + 1] == 2;
			bool isNotObstacle = room[targetY][targetX].fieldType != FieldType::wall && room[targetY][targetX].fieldType != FieldType::fire;

			player->lightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY)) ? 2 : 0;
			if (!isNotObstacle && player->lightMap[targetY][targetX] == 2) player->lightMap[targetY][targetX] = 1;

			if (isCheckDirASC)
			{
				// y 감소하며 이동
				--targetY;
				--targetX;
				if (targetY == pY || targetX == pX)
				{
					++targetY;
					isCheckDirASC = false;
				}
			}
			else
			{
				// y 증가하며 이동
				++targetY;
				++targetX;
				if (targetY == pY || targetX == pX)
				{
					--targetX;
					isCheckDirASC = true;
				}
			}
			deltaX = targetX - pX;
			deltaY = targetY - pY;
		}
		// 4분면 : 하우
		targetX = pX + 1;
		targetY = pY + 1;
		deltaX = targetX - pX;
		deltaY = targetY - pY;
		while (deltaX * deltaX + deltaY * deltaY <= pSight * pSight)
		{
			if (targetX < 0 || targetY < 0 || targetX >= cols || targetY >= rows) break;

			bool isLightOnX = player->lightMap[targetY][targetX - 1] == 2;
			bool isLightOnY = player->lightMap[targetY - 1][targetX] == 2;
			bool isLightOnXY = player->lightMap[targetY - 1][targetX - 1] == 2;
			bool isNotObstacle = room[targetY][targetX].fieldType != FieldType::wall && room[targetY][targetX].fieldType != FieldType::fire;

			player->lightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY)) ? 2 : 0;
			if (!isNotObstacle && player->lightMap[targetY][targetX] == 2) player->lightMap[targetY][targetX] = 1;

			if (isCheckDirASC)
			{
				// y 감소하며 이동
				--targetY;
				++targetX;
				if (targetY == pY || targetX == pX)
				{
					++targetY;
					isCheckDirASC = false;
				}
			}
			else
			{
				// y 증가하며 이동
				++targetY;
				--targetX;
				if (targetY == pY || targetX == pX)
				{
					++targetX;
					isCheckDirASC = true;
				}
			}
			deltaX = targetX - pX;
			deltaY = targetY - pY;
		}
#pragma endregion
	}
	void SetLightMap(int x, int y, float lightBright)
	{
		if (!lightMap) CreateLightMap();

		// 기본 라이트맵 생성 및 초기화
		const int pX = x;
		const int pY = y;
		const float pSight = lightBright;

		int tempListMapSize = ((int)lightBright + 1) * 2 + 1;
		int centerX = tempListMapSize / 2;
		int centerY = tempListMapSize / 2;
		int** tempLightMap = new int*[tempListMapSize];
		for (int targetY = 0; targetY < tempListMapSize; ++targetY)
		{
			tempLightMap[targetY] = new int[tempListMapSize] {0, };
			for (int targetX = 0; targetX < tempListMapSize; ++targetX)
			{
				int worldX = pX + (targetX - centerX);
				int worldY = pY + (targetY - centerY);

				if (worldX < 0 || worldY < 0 || worldX >= rows || worldY >= cols)
				{
					tempLightMap[targetY][targetX] = -1;
				}
			}
		}
		tempLightMap[centerY][centerX] = 2;

		for (int i = 0; i < tempListMapSize; ++i)
		{
			for (int j = 0; j < tempListMapSize; ++j)
			{
				int worldX = pX + j - centerX;
				int worldY = pY + i - centerY;

				if (worldX < 0 || worldY < 0 || worldX >= cols || worldY >= rows) cout << "9 ";
				else cout << room[worldY][worldX].fieldType << " ";
			}
			cout << endl;
		}

		cout << "-----------------------" << endl;

		/*
			1. 플레이어의 위치에서 직선으로 상하좌우 확인
			2. 1분면부터 4분면까지 남은 구역확인
			   남은 구역은 이전의 값이 or 연산을 통해 결과값을 도출
		*/
#pragma region Step.1
		// 상
		for (int dy = centerY - 1; dy >= 0; --dy)
		{
			int localX = centerX;
			int localY = dy;
			int worldY = pY + localY - centerY;


			if (tempLightMap[localY][localX] == -1) break;
			if (room[worldY][pX].fieldType != FieldType::wall && room[worldY][pX].fieldType != FieldType::fire) tempLightMap[localY][localX] = 2;
			else
			{
				tempLightMap[localY][localX] = 1;
				break;
			}
		}
		// 하
		for (int dy = centerY + 1; dy < tempListMapSize; ++dy)
		{
			int localX = centerX;
			int localY = dy;
			int worldY = pY + localY - centerY;

			if (tempLightMap[localY][localX] == -1) break;
			if (room[worldY][pX].fieldType != FieldType::wall && room[worldY][pX].fieldType != FieldType::fire) tempLightMap[localY][localX] = 2;
			else
			{
				tempLightMap[localY][localX] = 1;
				break;
			}
		}
		// 좌
		for (int dx = centerX - 1; dx >= 0; --dx)
		{
			int localX = dx;
			int localY = centerY;
			int worldX = pX + localX - centerX;

			if (tempLightMap[localY][localX] == -1) break;
			if (room[pY][worldX].fieldType != FieldType::wall && room[pY][worldX].fieldType != FieldType::fire) tempLightMap[localY][localX] = 2;
			else
			{
				tempLightMap[localY][localX] = 1;
				break;
			}
		}
		// 우
		for (int dx = centerX + 1; dx < tempListMapSize; ++dx)
		{
			int localX = dx;
			int localY = centerY;
			int worldX = pX + localX - centerX;

			if (tempLightMap[localY][localX] == -1) break;
			if (room[pY][worldX].fieldType != FieldType::wall && room[pY][worldX].fieldType != FieldType::fire) tempLightMap[localY][localX] = 2;
			else
			{
				tempLightMap[localY][localX] = 1;
				break;
			}
		}
#pragma endregion

		for (int i = 0; i < tempListMapSize; ++i)
		{
			for (int j = 0; j < tempListMapSize; ++j)
			{
				int worldX = pX + j - centerX;
				int worldY = pY + i - centerY;

				if (worldX < 0 || worldY < 0 || worldX >= cols || worldY >= rows) cout << "9 ";
				else cout << room[worldY][worldX].fieldType << " ";
			}
			cout << endl;
		}

		cout << "-----------------------" << endl;

		for (int i = 0; i < tempListMapSize; ++i)
		{
			for (int j = 0; j < tempListMapSize; ++j)
			{
				cout << tempLightMap[i][j] << " ";
			}
			cout << endl;
		}

#pragma region Step.2
		int targetX, targetY, deltaX, deltaY;
		bool isCheckDirASC = true; // true : 위로 올라가면서 확인(y감소), false : 아래로 내려가면서 확인(y증가)
		// 1분면 : 상우
		targetX = centerX + 1;
		targetY = centerY - 1;
		deltaX = targetX - centerX;
		deltaY = targetY - centerY;
		while (deltaX * deltaX + deltaY * deltaY <= pSight * pSight)
		{
			if (targetX < 0 || targetY < 0 || targetX >= tempListMapSize || targetY >= tempListMapSize) break;
			if (tempLightMap[targetY][targetX] != -1)
			{
				int worldX = pX + deltaX;
				int worldY = pY + deltaY;

				bool isLightOnX = tempLightMap[targetY][targetX - 1] == 2;
				bool isLightOnY = tempLightMap[targetY + 1][targetX] == 2;
				bool isLightOnXY = tempLightMap[targetY + 1][targetX - 1] == 2;
				bool isNotObstacle = room[worldY][worldX].fieldType != FieldType::wall && room[worldY][worldX].fieldType != FieldType::fire;

				tempLightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY)) ? 2 : 0;
				if (!isNotObstacle && tempLightMap[targetY][targetX] == 2) tempLightMap[targetY][targetX] = 1;
			}

			if (isCheckDirASC)
			{
				// y 감소하며 이동
				--targetY;
				--targetX;
				if (targetY == centerY || targetX == centerX)
				{
					++targetX;
					isCheckDirASC = false;
				}
			}
			else
			{
				// y 증가하며 이동
				++targetY;
				++targetX;
				if (targetY == centerY || targetX == centerX)
				{
					--targetY;
					isCheckDirASC = true;
				}
			}
			deltaX = targetX - centerX;
			deltaY = targetY - centerY;
		}
		// 2분면 : 상좌
		targetX = centerX - 1;
		targetY = centerY - 1;
		deltaX = targetX - centerX;
		deltaY = targetY - centerY;
		while (deltaX * deltaX + deltaY * deltaY <= pSight * pSight)
		{
			if (targetX < 0 || targetY < 0 || targetX >= tempListMapSize || targetY >= tempListMapSize) break;
			if (tempLightMap[targetY][targetX] != -1)
			{
				int worldX = pX + deltaX;
				int worldY = pY + deltaY;

				bool isLightOnX = tempLightMap[targetY][targetX + 1] == 2;
				bool isLightOnY = tempLightMap[targetY + 1][targetX] == 2;
				bool isLightOnXY = tempLightMap[targetY + 1][targetX + 1] == 2;
				bool isNotObstacle = room[worldY][worldX].fieldType != FieldType::wall && room[worldY][worldX].fieldType != FieldType::fire;

				tempLightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY)) ? 2 : 0;
				if (!isNotObstacle && tempLightMap[targetY][targetX] == 2) tempLightMap[targetY][targetX] = 1;
			}
			if (isCheckDirASC)
			{
				// y 감소하며 이동
				--targetY;
				++targetX;
				if (targetY == centerY || targetX == centerX)
				{
					--targetX;
					isCheckDirASC = false;
				}
			}
			else
			{
				// y 증가하며 이동
				++targetY;
				--targetX;
				if (targetY == centerY || targetX == centerX)
				{
					--targetY;
					isCheckDirASC = true;
				}
			}
			deltaX = targetX - centerX;
			deltaY = targetY - centerY;
		}
		// 3분면 : 하좌
		targetX = centerX - 1;
		targetY = centerY + 1;
		deltaX = targetX - centerX;
		deltaY = targetY - centerY;
		while (deltaX * deltaX + deltaY * deltaY <= pSight * pSight)
		{
			if (targetX < 0 || targetY < 0 || targetX >= tempListMapSize || targetY >= tempListMapSize) break;
			if (tempLightMap[targetY][targetX] != -1)
			{
				int worldX = pX + deltaX;
				int worldY = pY + deltaY;

				bool isLightOnX = tempLightMap[targetY][targetX + 1] == 2;
				bool isLightOnY = tempLightMap[targetY - 1][targetX] == 2;
				bool isLightOnXY = tempLightMap[targetY - 1][targetX + 1] == 2;
				bool isNotObstacle = room[worldY][worldX].fieldType != FieldType::wall && room[worldY][worldX].fieldType != FieldType::fire;

				tempLightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY)) ? 2 : 0;
				if (!isNotObstacle && tempLightMap[targetY][targetX] == 2) tempLightMap[targetY][targetX] = 1;
			}

			if (isCheckDirASC)
			{
				// y 감소하며 이동
				--targetY;
				--targetX;
				if (targetY == centerY || targetX == centerX)
				{
					++targetY;
					isCheckDirASC = false;
				}
			}
			else
			{
				// y 증가하며 이동
				++targetY;
				++targetX;
				if (targetY == centerY || targetX == centerX)
				{
					--targetX;
					isCheckDirASC = true;
				}
			}
			deltaX = targetX - centerX;
			deltaY = targetY - centerY;
		}
		// 4분면 : 하우
		targetX = centerX + 1;
		targetY = centerY + 1;
		deltaX = targetX - centerX;
		deltaY = targetY - centerY;
		while (deltaX * deltaX + deltaY * deltaY <= pSight * pSight)
		{
			if (targetX < 0 || targetY < 0 || targetX >= tempListMapSize || targetY >= tempListMapSize) break;

			if (tempLightMap[targetY][targetX] != -1)
			{
				int worldX = pX + deltaX;
				int worldY = pY + deltaY;

				bool isLightOnX = tempLightMap[targetY][targetX - 1] == 2;
				bool isLightOnY = tempLightMap[targetY - 1][targetX] == 2;
				bool isLightOnXY = tempLightMap[targetY - 1][targetX - 1] == 2;
				bool isNotObstacle = room[worldY][worldX].fieldType != FieldType::wall && room[worldY][worldX].fieldType != FieldType::fire;

				tempLightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY)) ? 2 : 0;
				if (!isNotObstacle && tempLightMap[targetY][targetX] == 2) tempLightMap[targetY][targetX] = 1;
			}

			if (isCheckDirASC)
			{
				// y 감소하며 이동
				--targetY;
				++targetX;
				if (targetY == centerY || targetX == centerX)
				{
					++targetY;
					isCheckDirASC = false;
				}
			}
			else
			{
				// y 증가하며 이동
				++targetY;
				--targetX;
				if (targetY == centerY || targetX == centerX)
				{
					++targetX;
					isCheckDirASC = true;
				}
			}
			deltaX = targetX - centerX;
			deltaY = targetY - centerY;
		}
#pragma endregion
#pragma region Step. 3

		for (int tempY = 0; tempY < tempListMapSize; ++tempY)
		{
			for (int tempX = 0; tempX < tempListMapSize; ++tempX)
			{
				if (tempLightMap[tempY][tempX] != -1)
				{
					int worldX = pX + (tempX - centerX);
					int worldY = pY + (tempY - centerY);

					if (lightMap[worldY][worldX] == 0) lightMap[worldY][worldX] = tempLightMap[tempY][tempX];
				}
			}
			delete[] tempLightMap[tempY];
		}
		delete[] tempLightMap;

#pragma endregion

	}

	void PrintDungeon(Hero* player, bool isDebug = false)
	{
		SetLightMap(player);

		cout << "   " << setfill('=') << setw((cols * 2 + 3)) << "" << endl;
		cout.copyfmt(std::ios(NULL));
		for (int i = 0; i < rows; i++)		// y축
		{
			cout << "   : ";
			for (int j = 0; j < cols; j++)	// x축
			{
				if (player->lightMap[i][j] || lightMap[i][j])
				{
					if (player && i == player->posY && j == player->posX) cout << "O ";
					else if ((room[i][j].monster || room[i][j].isMonster) && !isDebug) cout << "M ";
					else cout << G_CHAR_FIELD_TYPE[room[i][j].fieldType] << " ";
				}
				else cout << "  ";
			}
			cout << ":\t    ";
			if (i > 0 && i <= noticeCount) cout << notice[i - 1] << endl;
			else cout << endl;
		}
		cout << "   " << setfill('=') << setw(cols * 2 + 3) << "" << endl;
		cout.copyfmt(std::ios(NULL));
	}
	void Render(Hero* player, bool isDebug = false)
	{
		SetLightMap(player);

		cout << "   " << setfill('=') << setw((cols * 2 + 3)) << "" << endl;
		cout.copyfmt(std::ios(NULL));
		for (int i = 0; i < rows; i++)		// y축
		{
			cout << "   : ";
			for (int j = 0; j < cols; j++)	// x축
			{
				if (player->lightMap[i][j] || lightMap[i][j])
				{
					if (player && i == player->posY && j == player->posX) cout << "O ";
					else if ((room[i][j].monster || room[i][j].isMonster) && !isDebug) cout << "M ";
					else cout << G_CHAR_FIELD_TYPE[room[i][j].fieldType] << " ";
				}
				else cout << "  ";
			}
			cout << ":\t    ";
			if (i > 0 && i <= noticeCount) cout << notice[i - 1] << endl;
			else cout << endl;
		}
		cout << "   " << setfill('=') << setw(cols * 2 + 3) << "" << endl;
		cout.copyfmt(std::ios(NULL));
	}

	void Release()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; ++j)
			{
				room[i][j].Release();
			}
			delete[] lightMap[i];
			delete[] room[i];
		}
		delete[] lightMap;
		delete[] room;
	}
};

struct Difficulty
{
	int dungeonSize[3] = { 10, 15, 20 };
	int heroHP[3] = { 100, 90, 80 };
	float monsterStrength[3] = { 1, 1.5f, 2 };
	int monsterEncounter[3] = { 20, 10, 10 };
	string name[3] = { "EASY", "NORMAL", "HARD" };
};

struct BattleItem
{
	string name;
};

struct BattleInfo
{
	BattleItem item[3];

	void PrintBattleStatus(Hero& hero, Monster& monster)
	{
		string line;
		cout << "---------------------------------------------------------------------" << endl;
		cout << left << setw(38) << ("  [[ " + hero.name + " ]]") << ("[[ " + monster.name + " ]] ") << endl;
		cout << "    레벨\t: " << right << setw(10) << hero.lvl << endl;
		cout << "    공격력\t: " << right << setw(10) << hero.attack << setw(20) << "공격력\t: " << setw(10) << monster.attack << endl;
		cout << "    체  력\t: " << right << setw(10) << string(to_string(hero.hp) + " / " + to_string(hero.maxHP)) << setw(20) << " 체  력\t: " << setw(10) << string(to_string(monster.hp) + " / " + to_string(monster.maxHP)) << endl;
		cout << "---------------------------------------------------------------------" << endl;
	}
};

struct StepHero
{
	enum GameState
	{
		TITLE = 0, INGAME, BATTLE, SHOP, ENDING
	};

	GameState gameState = GameState::TITLE;
	Difficulty difficulty;
	Hero* player = nullptr;
	Shop* shop = nullptr;
	Dungeon* dungeon = nullptr;

	void Init()
	{
		gTextRender = TextRender(100, 50);
	}

	void Update()
	{
		switch (gameState)
		{
		case GameState::TITLE:

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



			break;
		case GameState::INGAME:
		case GameState::BATTLE:

			break;
		case GameState::SHOP:
			break;
		}
	}

	void Render()
	{
		switch (gameState)
		{
		case GameState::TITLE:
			gTextRender.Render(TextLayout::LayoutKind::TITLE);
			break;
		case GameState::INGAME:
		case GameState::BATTLE:

			break;
		case GameState::SHOP:
			break;
		}
	}

	void Release()
	{
		gTextRender.Release();
	}
};

int main()
{
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

	gTextRender.Render(TextLayout::LayoutKind::TITLE);
	gInputKey.Requset(InputKey::InputType::ANYKEYS);
	gInputKey.Requset(InputKey::InputType::SELECT);
	gInputKey.Requset(InputKey::InputType::INGAME);
	while (1)
	{
		gInputKey.Update();
		cout << gInputKey.inputResult << endl;
	}

	/*
		0-1 : type 1 10x10
		2-3 : type 2 5x10
		4-5 : type 3 10x5
		6-9 : type 4 5x5
		던전의 형태 템플릿
		0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 탈출구
	*/
#pragma region 던전템플릿
	
	gDungeonSet[0] = { 10, 10 };
	gDungeonSet[0].fieldSet = new int* [gDungeonSet[0].rows];
	gDungeonSet[0].fieldSet[0] = new int[gDungeonSet[0].cols]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	gDungeonSet[0].fieldSet[1] = new int[gDungeonSet[0].cols]{ 0, 0, 0, 0, 0, 0, 0, 2, 1, 1 };
	gDungeonSet[0].fieldSet[2] = new int[gDungeonSet[0].cols]{ 0, 0, 0, 0, 0, 3, 2, 2, 2, 1 };
	gDungeonSet[0].fieldSet[3] = new int[gDungeonSet[0].cols]{ 0, 0, 0, 0, 3, 2, 2, 2, 0, 2 };
	gDungeonSet[0].fieldSet[4] = new int[gDungeonSet[0].cols]{ 0, 0, 0, 0, 0, 3, 2, 0, 0, 0 };
	gDungeonSet[0].fieldSet[5] = new int[gDungeonSet[0].cols]{ 0, 0, 3, 0, 0, 2, 2, 4, 0, 0 };
	gDungeonSet[0].fieldSet[6] = new int[gDungeonSet[0].cols]{ 0, 0, 0, 1, 0, 1, 1, 2, 0, 0 };
	gDungeonSet[0].fieldSet[7] = new int[gDungeonSet[0].cols]{ 0, 0, 1, 1, 0, 1, 1, 1, 2, 0 };
	gDungeonSet[0].fieldSet[8] = new int[gDungeonSet[0].cols]{ 0, 0, 0, 0, 1, 0, 0, 1, 1, 2 };
	gDungeonSet[0].fieldSet[9] = new int[gDungeonSet[0].cols]{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 2 };

	gDungeonSet[1] = { 10, 10 };
	gDungeonSet[1].fieldSet = new int* [gDungeonSet[1].rows];
	gDungeonSet[1].fieldSet[0] = new int[gDungeonSet[1].cols]{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 };
	gDungeonSet[1].fieldSet[1] = new int[gDungeonSet[1].cols]{ 0, 0, 0, 0, 1, 1, 1, 1, 0, 0 };
	gDungeonSet[1].fieldSet[2] = new int[gDungeonSet[1].cols]{ 0, 0, 0, 1, 1, 1, 0, 0, 3, 0 };
	gDungeonSet[1].fieldSet[3] = new int[gDungeonSet[1].cols]{ 0, 0, 1, 1, 0, 0, 0, 3, 3, 0 };
	gDungeonSet[1].fieldSet[4] = new int[gDungeonSet[1].cols]{ 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 };
	gDungeonSet[1].fieldSet[5] = new int[gDungeonSet[1].cols]{ 0, 1, 1, 1, 3, 0, 4, 3, 3, 0 };
	gDungeonSet[1].fieldSet[6] = new int[gDungeonSet[1].cols]{ 0, 1, 1, 3, 3, 0, 3, 0, 0, 0 };
	gDungeonSet[1].fieldSet[7] = new int[gDungeonSet[1].cols]{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	gDungeonSet[1].fieldSet[8] = new int[gDungeonSet[1].cols]{ 0, 0, 0, 3, 3, 0, 3, 0, 0, 0 };
	gDungeonSet[1].fieldSet[9] = new int[gDungeonSet[1].cols]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	gDungeonSet[2] = { 5, 10 };
	gDungeonSet[2].fieldSet = new int*[gDungeonSet[2].rows];
	gDungeonSet[2].fieldSet[0] = new int[gDungeonSet[2].cols]{ 0, 0, 1, 1, 0, 0, 2, 2, 0, 0 };
	gDungeonSet[2].fieldSet[1] = new int[gDungeonSet[2].cols]{ 0, 1, 1, 1, 1, 0, 2, 2, 2, 0 };
	gDungeonSet[2].fieldSet[2] = new int[gDungeonSet[2].cols]{ 0, 0, 1, 1, 1, 3, 0, 0, 2, 2 };
	gDungeonSet[2].fieldSet[3] = new int[gDungeonSet[2].cols]{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 2 };
	gDungeonSet[2].fieldSet[4] = new int[gDungeonSet[2].cols]{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 2 };

	gDungeonSet[3] = { 5, 10 };
	gDungeonSet[3].fieldSet = new int*[gDungeonSet[3].rows];
	gDungeonSet[3].fieldSet[0] = new int[gDungeonSet[3].cols]{ 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
	gDungeonSet[3].fieldSet[1] = new int[gDungeonSet[3].cols]{ 0, 0, 1, 1, 1, 0, 3, 0, 0, 0 };
	gDungeonSet[3].fieldSet[2] = new int[gDungeonSet[3].cols]{ 0, 0, 1, 1, 0, 3, 0, 0, 1, 0 };
	gDungeonSet[3].fieldSet[3] = new int[gDungeonSet[3].cols]{ 0, 1, 1, 1, 3, 0, 1, 1, 1, 0 };
	gDungeonSet[3].fieldSet[4] = new int[gDungeonSet[3].cols]{ 0, 0, 0, 1, 0, 1, 1, 1, 0, 0 };

	gDungeonSet[4] = { 10, 5 };
	gDungeonSet[4].fieldSet = new int* [gDungeonSet[4].rows];
	gDungeonSet[4].fieldSet[0] = new int[gDungeonSet[4].cols]{ 0, 0, 0, 0, 0 };
	gDungeonSet[4].fieldSet[1] = new int[gDungeonSet[4].cols]{ 0, 1, 1, 1, 1 };
	gDungeonSet[4].fieldSet[2] = new int[gDungeonSet[4].cols]{ 1, 1, 1, 1, 1 };
	gDungeonSet[4].fieldSet[3] = new int[gDungeonSet[4].cols]{ 0, 0, 1, 0, 1 };
	gDungeonSet[4].fieldSet[4] = new int[gDungeonSet[4].cols]{ 0, 0, 1, 0, 1 };
	gDungeonSet[4].fieldSet[5] = new int[gDungeonSet[4].cols]{ 0, 0, 2, 0, 2 };
	gDungeonSet[4].fieldSet[6] = new int[gDungeonSet[4].cols]{ 0, 2, 2, 2, 2 };
	gDungeonSet[4].fieldSet[7] = new int[gDungeonSet[4].cols]{ 0, 2, 2, 2, 2 };
	gDungeonSet[4].fieldSet[8] = new int[gDungeonSet[4].cols]{ 2, 2, 2, 2, 2 };
	gDungeonSet[4].fieldSet[9] = new int[gDungeonSet[4].cols]{ 2, 2, 2, 2, 2 };

	gDungeonSet[5] = { 10, 5 };
	gDungeonSet[5].fieldSet = new int* [gDungeonSet[5].rows];
	gDungeonSet[5].fieldSet[0] = new int[gDungeonSet[5].cols]{ 0, 0, 0, 0, 0 };
	gDungeonSet[5].fieldSet[1] = new int[gDungeonSet[5].cols]{ 0, 1, 0, 1, 1 };
	gDungeonSet[5].fieldSet[2] = new int[gDungeonSet[5].cols]{ 0, 1, 1, 1, 1 };
	gDungeonSet[5].fieldSet[3] = new int[gDungeonSet[5].cols]{ 0, 0, 0, 0, 1 };
	gDungeonSet[5].fieldSet[4] = new int[gDungeonSet[5].cols]{ 0, 0, 0, 0, 1 };
	gDungeonSet[5].fieldSet[5] = new int[gDungeonSet[5].cols]{ 0, 0, 1, 0, 0 };
	gDungeonSet[5].fieldSet[6] = new int[gDungeonSet[5].cols]{ 0, 1, 0, 0, 0 };
	gDungeonSet[5].fieldSet[7] = new int[gDungeonSet[5].cols]{ 1, 1, 0, 0, 0 };
	gDungeonSet[5].fieldSet[8] = new int[gDungeonSet[5].cols]{ 0, 1, 0, 0, 0 };
	gDungeonSet[5].fieldSet[9] = new int[gDungeonSet[5].cols]{ 0, 0, 0, 0, 0 };

	gDungeonSet[6] = { 5, 5 };
	gDungeonSet[6].fieldSet = new int*[gDungeonSet[6].rows];
	gDungeonSet[6].fieldSet[0] = new int[gDungeonSet[6].cols]{ 0, 3, 0, 3, 0 };
	gDungeonSet[6].fieldSet[1] = new int[gDungeonSet[6].cols]{ 0, 0, 2, 0, 0 };
	gDungeonSet[6].fieldSet[2] = new int[gDungeonSet[6].cols]{ 0, 2, 2, 2, 0 };
	gDungeonSet[6].fieldSet[3] = new int[gDungeonSet[6].cols]{ 0, 0, 2, 0, 0 };
	gDungeonSet[6].fieldSet[4] = new int[gDungeonSet[6].cols]{ 0, 3, 0, 3, 0 };

	gDungeonSet[7] = { 5, 5 };
	gDungeonSet[7].fieldSet = new int*[gDungeonSet[7].rows];
	gDungeonSet[7].fieldSet[0] = new int[gDungeonSet[7].cols]{ 0, 0, 3, 3, 0 };
	gDungeonSet[7].fieldSet[1] = new int[gDungeonSet[7].cols]{ 3, 0, 0, 0, 0 };
	gDungeonSet[7].fieldSet[2] = new int[gDungeonSet[7].cols]{ 3, 0, 4, 0, 3 };
	gDungeonSet[7].fieldSet[3] = new int[gDungeonSet[7].cols]{ 0, 0, 0, 0, 3 };
	gDungeonSet[7].fieldSet[4] = new int[gDungeonSet[7].cols]{ 0, 3, 3, 0, 0 };

	gDungeonSet[8] = { 5, 5 };
	gDungeonSet[8].fieldSet = new int*[gDungeonSet[8].rows];
	gDungeonSet[8].fieldSet[0] = new int[gDungeonSet[8].cols]{ 0, 2, 0, 3, 0 };
	gDungeonSet[8].fieldSet[1] = new int[gDungeonSet[8].cols]{ 2, 2, 3, 0, 0 };
	gDungeonSet[8].fieldSet[2] = new int[gDungeonSet[8].cols]{ 3, 0, 0, 0, 3 };
	gDungeonSet[8].fieldSet[3] = new int[gDungeonSet[8].cols]{ 0, 0, 3, 1, 1 };
	gDungeonSet[8].fieldSet[4] = new int[gDungeonSet[8].cols]{ 0, 0, 3, 1, 1 };

	gDungeonSet[9] = { 5, 5 };
	gDungeonSet[9].fieldSet = new int*[gDungeonSet[9].rows];
	gDungeonSet[9].fieldSet[0] = new int[gDungeonSet[9].cols]{ 0, 0, 1, 0, 0 };
	gDungeonSet[9].fieldSet[1] = new int[gDungeonSet[9].cols]{ 0, 3, 0, 3, 0 };
	gDungeonSet[9].fieldSet[2] = new int[gDungeonSet[9].cols]{ 1, 0, 4, 0, 1 };
	gDungeonSet[9].fieldSet[3] = new int[gDungeonSet[9].cols]{ 0, 3, 0, 3, 0 };
	gDungeonSet[9].fieldSet[4] = new int[gDungeonSet[9].cols]{ 0, 0, 1, 0, 0 };
#pragma endregion

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
