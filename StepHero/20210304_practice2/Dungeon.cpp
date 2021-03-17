#include "stdafx.h"
#include "Dungeon.h"

Dungeon::Dungeon()
{
	fieldTypeStr.insert(make_pair(Room::FieldType::empty, "."));
	fieldTypeStr.insert(make_pair(Room::FieldType::wood, "T"));
	fieldTypeStr.insert(make_pair(Room::FieldType::swamp, "~"));
	fieldTypeStr.insert(make_pair(Room::FieldType::wall, "#"));
	fieldTypeStr.insert(make_pair(Room::FieldType::fire, "A"));
	fieldTypeStr.insert(make_pair(Room::FieldType::out, "E"));

	notice[0] = "* 던전에서 탈출에 성공하세요 *";
	notice[1] = "";
	notice[2] = "  O : 플레이어";
	notice[3] = "  E : 탈출구";
	notice[4] = "  . : 공터, T : 숲, ~ : 늪";
	notice[5] = "  # : 벽, A : 햇불";
	notice[6] = "";
	notice[7] = "  P : 몬스터위치 확인";
	notice[8] = "  M : 소지금 1000골드 UP";
	noticeCount = 9;
}

Dungeon::~Dungeon()
{
}

void Dungeon::CreateDungeon(int _size)
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
			for (int ty = 0; ty < gDungeonTemplate[region[y][x]->templateNo].rows / 5; ++ty)
			{
				for (int tx = 0; tx < gDungeonTemplate[region[y][x]->templateNo].cols / 5; ++tx)
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
	for (int i = 0; i < rows; i += 5)		// y축
	{
		for (int j = 0; j < cols; j += 5)	// x축
		{
			if (region[i / 5][j / 5]->isUsed) continue;
			int templateNo = region[i / 5][j / 5]->templateNo;

			if (region[i / 5][j / 5]->isFlip)
			{
				for (int ty = i - (i / 5) * 5; ty < gDungeonTemplate[templateNo].rows; ++ty)
				{
					for (int tx = j - (j / 5) * 5; tx < gDungeonTemplate[templateNo].cols; ++tx)
					{
						if (room[i + ty][j + tx].fieldType != Room::FieldType::out)
						{
							room[i + ty][j + tx].fieldType = (Room::FieldType)gDungeonTemplate[templateNo].fieldSet[ty][gDungeonTemplate[templateNo].cols - 1 - tx];
						}
						if (room[i + ty][j + tx].fieldType == Room::FieldType::fire)
						{
							lightList.PushBack(j + tx, i + ty, 2.5f);
						}
					}
				}
			}
			else
			{
				for (int ty = i - (i / 5) * 5; ty < gDungeonTemplate[templateNo].rows; ++ty)
				{
					for (int tx = j - (j / 5) * 5; tx < gDungeonTemplate[templateNo].cols; ++tx)
					{
						if (room[i + ty][j + tx].fieldType != Room::FieldType::out)
						{
							room[i + ty][j + tx].fieldType = (Room::FieldType)gDungeonTemplate[templateNo].fieldSet[ty][tx];
						}

						if (room[i + ty][j + tx].fieldType == Room::FieldType::fire)
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
			if (room[y][x].fieldType == Room::FieldType::eof) room[y][x].fieldType = Room::FieldType::empty;
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

				for (int ry = i; ry < i + gDungeonTemplate[templateNo].rows / 5; ++ry)
				{
					for (int rx = l; rx < l + gDungeonTemplate[templateNo].cols / 5; ++rx)
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
void Dungeon::CreateLightMap()
{
	lightMap = new int*[rows];
	for (int i = 0; i < rows; ++i)
	{
		lightMap[i] = new int[cols] {0, };
	}
}

void Dungeon::ResetLightMap()
{
	for (int i = 0; i < rows; ++i)
	{
		memset(lightMap[i], 0, sizeof(int) * cols);
	}
}

void Dungeon::SetMonsters(int monsterEncount, Hero* player, int safeZoneSize)
{
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			if (player)
			{
				if (abs(player->GetPosX() - x) <= 2 && abs(player->GetPosY() - y) <= 2)
				{
					continue;
				}
			}

			if (rand() % 100 < monsterEncount) room[y][x].isMonster = true;
		}
	}
}
void Dungeon::SetExit(int _exitPosX, int _exitPosY)
{
	exitPosX = _exitPosX;
	exitPosY = _exitPosY;
	room[exitPosY][exitPosX].fieldType = Room::FieldType::out;
}

bool Dungeon::IsPossibleMove(int x, int y)
{
	if (x < 0 || y < 0 || x >= cols || y >= rows) return false;
	return room[y][x].fieldType != Room::FieldType::wall && room[y][x].fieldType != Room::FieldType::fire;
}

void Dungeon::SetLightMap(Hero* player)
{
	// 기본 라이트맵 생성 및 초기화
	if (!player->lightMap) player->CreateLightMap(rows, cols);
	else player->ResetLightMap();

	int pX = player->GetPosX();
	int pY = player->GetPosY();
	float pSight = player->GetSight();
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
		if (room[dy][pX].fieldType != Room::FieldType::wall && room[dy][pX].fieldType != Room::FieldType::fire) player->lightMap[dy][pX] = 2;
		else
		{
			player->lightMap[dy][pX] = 1;
			break;
		}
	}
	// 하
	for (int dy = pY + 1; dy < rows && dy <= pY + pSight; ++dy)
	{
		if (room[dy][pX].fieldType != Room::FieldType::wall && room[dy][pX].fieldType != Room::FieldType::fire) player->lightMap[dy][pX] = 2;
		else
		{
			player->lightMap[dy][pX] = 1;
			break;
		}
	}
	// 좌
	for (int dx = pX - 1; dx >= 0 && dx >= pX - pSight; --dx)
	{
		if (room[pY][dx].fieldType != Room::FieldType::wall && room[pY][dx].fieldType != Room::FieldType::fire) player->lightMap[pY][dx] = 2;
		else
		{
			player->lightMap[pY][dx] = 1;
			break;
		}
	}
	// 우
	for (int dx = pX + 1; dx < cols && dx <= pX + pSight; ++dx)
	{
		if (room[pY][dx].fieldType != Room::FieldType::wall && room[pY][dx].fieldType != Room::FieldType::fire) player->lightMap[pY][dx] = 2;
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
		bool isNotObstacle = room[targetY][targetX].fieldType != Room::FieldType::wall && room[targetY][targetX].fieldType != Room::FieldType::fire;

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
		bool isNotObstacle = room[targetY][targetX].fieldType != Room::FieldType::wall && room[targetY][targetX].fieldType != Room::FieldType::fire;

		player->lightMap[targetY][targetX] = ((isLightOnX && isLightOnY) || ((isLightOnX ^ isLightOnY) && isLightOnXY)) ? 2 : 0;
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
		bool isNotObstacle = room[targetY][targetX].fieldType != Room::FieldType::wall && room[targetY][targetX].fieldType != Room::FieldType::fire;

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
		bool isNotObstacle = room[targetY][targetX].fieldType != Room::FieldType::wall && room[targetY][targetX].fieldType != Room::FieldType::fire;

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
void Dungeon::SetLightMap(int x, int y, float lightBright)
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
		if (room[worldY][pX].fieldType != Room::FieldType::wall && room[worldY][pX].fieldType != Room::FieldType::fire) tempLightMap[localY][localX] = 2;
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
		if (room[worldY][pX].fieldType != Room::FieldType::wall && room[worldY][pX].fieldType != Room::FieldType::fire) tempLightMap[localY][localX] = 2;
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
		if (room[pY][worldX].fieldType != Room::FieldType::wall && room[pY][worldX].fieldType != Room::FieldType::fire) tempLightMap[localY][localX] = 2;
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
		if (room[pY][worldX].fieldType != Room::FieldType::wall && room[pY][worldX].fieldType != Room::FieldType::fire) tempLightMap[localY][localX] = 2;
		else
		{
			tempLightMap[localY][localX] = 1;
			break;
		}
	}
#pragma endregion

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
			bool isNotObstacle = room[worldY][worldX].fieldType != Room::FieldType::wall && room[worldY][worldX].fieldType != Room::FieldType::fire;

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
			bool isNotObstacle = room[worldY][worldX].fieldType != Room::FieldType::wall && room[worldY][worldX].fieldType != Room::FieldType::fire;

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
			bool isNotObstacle = room[worldY][worldX].fieldType != Room::FieldType::wall && room[worldY][worldX].fieldType != Room::FieldType::fire;

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
			bool isNotObstacle = room[worldY][worldX].fieldType != Room::FieldType::wall && room[worldY][worldX].fieldType != Room::FieldType::fire;

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

void Dungeon::Update()
{
	// 플레이어 플레이어 진행 턴에 따라 몬스터 이동 로직
}

//void Dungeon::PrintDungeon(Hero* player, bool isDebug)
//{
//	SetLightMap(player);
//
//	cout << "   " << setfill('=') << setw((cols * 2 + 3)) << "" << endl;
//	cout.copyfmt(std::ios(NULL));
//	for (int i = 0; i < rows; i++)		// y축
//	{
//		cout << "   : ";
//		for (int j = 0; j < cols; j++)	// x축
//		{
//			if (player->lightMap[i][j] || lightMap[i][j])
//			{
//				if (player && i == player->posY && j == player->posX) cout << "O ";
//				else if ((room[i][j].monster || room[i][j].isMonster) && !isDebug) cout << "M ";
//				else cout << G_CHAR_FIELD_TYPE[room[i][j].fieldType] << " ";
//			}
//			else cout << "  ";
//		}
//		cout << ":\t    ";
//		if (i > 0 && i <= noticeCount) cout << notice[i - 1] << endl;
//		else cout << endl;
//	}
//	cout << "   " << setfill('=') << setw(cols * 2 + 3) << "" << endl;
//	cout.copyfmt(std::ios(NULL));
//}
void Dungeon::Render(Hero* player, bool isDebug)
{
	SetLightMap(player);

	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::CONTENT, string(cols * 2 + 3, '=') + string(45 - (cols / 2), ' '));
	for (int i = 0; i < rows; i++)		// y축
	{
		string mapRow = string(cols * 2 + 3, ' '), noticeRow(30, ' ');
		mapRow.replace(0, 2, ": ");
		for (int j = 0; j < cols; j++)	// x축
		{
			if (player->lightMap[i][j] || lightMap[i][j])
			{
				if (player && i == player->GetPosY() && j == player->GetPosX()) mapRow.replace(j * 2 + 2, 1, "O");
				else if ((room[i][j].monster || room[i][j].isMonster)) mapRow.replace(j * 2 + 2 , 1, "M");
				else mapRow.replace(j * 2 + 2, 1, fieldTypeStr[room[i][j].fieldType]);
			}
		}
		mapRow.replace(cols * 2 + 2, 1, ":");
		if (i > 0 && i <= noticeCount) noticeRow.replace(0, notice[i - 1].length(), notice[i - 1]);

		gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::CONTENT, mapRow + string(15 - (cols / 2), ' ') + noticeRow);
	}
	gTextRender.AppendBuffer(TextLayout::LayoutKind::INGAME, TextLayout::LayoutPos::CONTENT, string(cols * 2 + 3, '=') + string(45 - (cols / 2), ' '));
}

void Dungeon::Release()
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