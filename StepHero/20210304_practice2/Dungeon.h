#pragma once
#include "Hero.h"
#include "Monster.h"
#include "TextRender.h"
#include "DungeonTemplate.h"

extern TextRender gTextRender;
extern DungeonTemplate gDungeonTemplate[10];
//던전 템플릿 파일 읽어들여서 로드

extern char G_CHAR_FIELD_TYPE[6];

class Room
{
public:
	enum FieldType { empty = 0, wood, swamp, wall, fire, out, eof };

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

class Region
{
public:
	bool isUsed = false;
	int templateNo;
	bool isFlip = false;
};

class Dungeon
{
public:

	int rows;
	int cols;
	int exitPosX;
	int exitPosY;

	int noticeCount;
	string notice[9];

	map<Room::FieldType, string> fieldTypeStr;

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
				for (int i = 0; i < size; ++i)
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

	Dungeon();
	~Dungeon();

	void CreateDungeon(int _size);
	void CreateLightMap();

	void ResetLightMap();

	void SetMonsters(int monsterEncount, Hero* player = nullptr, int safeZoneSize = 2);
	void SetExit(int _exitPosX, int _exitPosY);

	bool IsPossibleMove(int x, int y);
	bool IsInMonster(int x, int y);

	Room::FieldType GetRoomFieldType(int x, int y);

	void SetLightMap(Hero* player);
	void SetLightMap(int x, int y, float lightBright);

	//void PrintDungeon(Hero* player, bool isDebug = false);
	void Update();
	void Render(Hero * player, bool isDebug);
	void Release();
};

