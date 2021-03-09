﻿// 20210304_practice2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iomanip>
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <conio.h>

using namespace std;

// 0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 탈출구
char G_CHAR_FIELD_TYPE[5] = {'.', 'T', '~', '#', 'E'};
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
	Monster monster[3][4];

	// 몬스터 생성	
	Monster* CreateMonster(int fieldType)
	{
		// 0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 탈출구
		if (fieldType == 3 || fieldType == 4) return nullptr;
		return new Monster(monster[fieldType][rand() % 4]);
	}
};

struct Room
{
	int locX;
	int locY;

	int fieldType;
	//char data;

	// 0 위 1 아래 2 왼쪽 3 오른쪽
	Room* next[4] = { nullptr, nullptr, nullptr, nullptr };
	Monster* monster = nullptr;
};

struct Dungeon
{
	int rows;
	int cols;
	int exitPosX;
	int exitPosY;

	int noticeCount;
	string notice[7];

	Room** map;
};

struct Difficulty
{
	int dungeonSize[3] = { 10, 13, 15 };
	int heroHP[3] = { 100, 90, 80 };
	int monsterStrength[3] = { 1, 2, 3 };
	int monsterEncounter[3] = { 20, 40, 60 };
	string name[3] = { "EASY", "NORMAL", "HARD" };
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

	string name;

	// 0: IDLE 1: BATTLE 2: SHOP 3: DIE
	int state = 0;

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

	int HitDamage(int damage)
	{
		hp -= damage;
		if (hp <= 0)
		{
			hp = 0;
			state = 3;
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
		cout << left << "     [[ " << hero.name << " ]]" << right << setw(25 + monster.name.length()) << "[[ " << monster.name << " ]] " << endl;
		cout << "    레벨\t: " << right << setw(10) << hero.lvl << endl;
		cout << "    공격력\t: " << right << setw(10) << hero.attack << setw(20) << "공격력\t: " << setw(10) << monster.attack << endl;
		cout << "    체  력\t: " << right << setw(10) << string(to_string(hero.hp) + " / " + to_string(hero.maxHP)) << setw(20) << " 체  력\t: " << setw(10) << string(to_string(monster.hp) + " / " + to_string(monster.maxHP)) << endl;
		cout << "    경험치\t: " << right << setw(10) << hero.exp << endl;
		cout << "    소지금\t: " << right << setw(10) << hero.gold << endl;
		cout << "---------------------------------------------------------------------" << endl;
	}
};

int main()
{
	srand(time(NULL));

	system("MODE CON COLS=100 LINES=50");

	// 난이도 정보
	Difficulty difficulty;
	int difficultyInput = 0;
	bool isUseShop = false;
	int isMonsterMove = 0;
	Hero player;
	Dungeon dungeon;
	dungeon.notice[0] = "* 던전에서 탈출에 성공하세요 *";
	dungeon.notice[1] = "";
	dungeon.notice[2] = "  O : 플레이어";
	dungeon.notice[3] = "  E : 탈출구";
	dungeon.notice[4] = "";
	dungeon.notice[5] = "  P : 몬스터위치 확인";
	dungeon.notice[6] = "  M : 소지금 1000골드 UP";
	dungeon.noticeCount = 7;

	// 맵 디버그 모드
	bool debug = false;

	// 타이틀
	bool isInitalized = false;
	int initalizeStep = 0;
	while (!isInitalized)
	{
		system("cls");

		cout << "\n\n\n";
		cout << "      ----------------------------------------------------------     " << endl;
		cout << "---------------------------------------------------------------------" << endl;
		cout << "      #####  #####  #####  #####      #   #  #####  #####  #####     " << endl;
		cout << "      #        #    #      #   #      #   #  #      #   #  #   #     " << endl;
		cout << "      #####    #    ###    #####      #####  ###    #####  #   #     " << endl;
		cout << "          #    #    #      #          #   #  #      # #    #   #     " << endl;
		cout << "          #    #    #      #          #   #  #      #  #   #   #     " << endl;
		cout << "      #####    #    #####  #          #   #  #####  #   #  #####     " << endl;
		cout << "---------------------------------------------------------------------" << endl;
		cout << "     -----------------------------------------------------------     " << endl;
		cout << "\n\n";
		
		cout << "                                ------                               " << endl;
		cout << "                              ----  ----                             " << endl;
		cout << "                                                                     " << endl;

		cout << right << setw(35 + difficulty.name[difficultyInput].length() / 2) << difficulty.name[difficultyInput] << endl;

		if (initalizeStep == 0)
		{
			cout << "                                                                     " << endl;
			cout << "                    CHANGE :: W,S  SELECT :: ENTER                   " << endl;
		}
		cout << "                                                                     " << endl;
		cout << "                              ----  ----                             " << endl;
		cout << "                                ------                               " << endl;
	
		if (initalizeStep == 0)
		{
			switch (_getch())
			{
			case 'w':
			case 'W':
				difficultyInput = (difficultyInput + 2) % 3;
				break;
			case 's':
			case 'S':
				difficultyInput = (difficultyInput + 1) % 3;
				break;
			case 13:
				++initalizeStep;

				// 플레이어의 난이도별 능력치 설정
				dungeon.rows = difficulty.dungeonSize[difficultyInput];
				dungeon.cols = difficulty.dungeonSize[difficultyInput];
				player = { difficulty.heroHP[difficultyInput], difficulty.heroHP[difficultyInput], 1000, 0, 0, 25, 1, 0 };
				break;
			case 27:
				// ESC 입력 게임종료;
				return 0;
				break;
			default:
				if (_kbhit())
				{
					_getch();
					cout << ":::::  ERROR  :::::" << endl;
					cout << ":::::  입력이 잘못되었습니다.  :::::" << endl;
					cout << ":::::  만약 한글로 되어있다면 한/영키로 영어로 바꿔주세요  :::::" << endl;
					_getch();
				}
				break;
			}
		}
		else
		{
			// 영웅의 이름을 입력해주세요.
			cout << "\n               영웅의 이름을 입력해주세요. :: ";
			cin >> player.name;

			// 초기정보 입력 끝
			isInitalized = true;
		}
	}

	// 상점의 정보
	Shop shop;
	shop.items[0] = { 100, 999, 5, "전체회복" };
	shop.items[1] = { 10, 10, 10, "10회복" };
	shop.items[2] = { 20, 20, 20, "20회복" };
	shop.items[3] = { 30, 30, 30, "30회복" };
	shop.items[4] = { 40, 40, 40, "40회복" };

	// 배틀정보
	BattleInfo battleInfo;
	battleInfo.item[0] = { "가위" };
	battleInfo.item[1] = { "바위" };
	battleInfo.item[2] = { " 보 " };

	// 몬스터 설정
	MonsterTable monsterTable;
	int mStrength = difficulty.monsterStrength[difficultyInput];

	// 0: 땅, 1: 숲, 2: 늪, 3: 벽, 4: 탈출구
	monsterTable.monster[0][0] = { 10 * mStrength, 10 * mStrength, 10 * mStrength, 20, 30, 0, "두더지" };
	monsterTable.monster[0][1] = { 12 * mStrength, 20 * mStrength, 20 * mStrength, 25, 50, 0, "슬라임" };
	monsterTable.monster[0][2] = { 14 * mStrength, 30 * mStrength, 30 * mStrength, 30, 80, 0, "도적" };
	monsterTable.monster[0][3] = { 16 * mStrength, 40 * mStrength, 40 * mStrength, 35, 100, 0, "산적" };

	monsterTable.monster[1][0] = { 10 * mStrength, 10 * mStrength, 10 * mStrength, 20, 30, 0, "토끼" };
	monsterTable.monster[1][1] = { 12 * mStrength, 20 * mStrength, 20 * mStrength, 25, 50, 0, "사슴" };
	monsterTable.monster[1][2] = { 14 * mStrength, 30 * mStrength, 30 * mStrength, 30, 80, 0, "늑대" };
	monsterTable.monster[1][3] = { 16 * mStrength, 40 * mStrength, 40 * mStrength, 35, 100, 0, "늑대인간" };

	monsterTable.monster[2][0] = { 10 * mStrength, 10 * mStrength, 10 * mStrength, 20, 30, 0, "두꺼비" };
	monsterTable.monster[2][1] = { 12 * mStrength, 20 * mStrength, 20 * mStrength, 25, 50, 0, "뱀" };
	monsterTable.monster[2][2] = { 14 * mStrength, 30 * mStrength, 30 * mStrength, 30, 80, 0, "저주받은 나무" };
	monsterTable.monster[2][3] = { 16 * mStrength, 40 * mStrength, 40 * mStrength, 35, 100, 0, "식인식물" };

	// 맵 초기화
	dungeon.map = new Room*[difficulty.dungeonSize[difficultyInput]];
	for (int i = 0; i < dungeon.rows; i++)		// y축
	{
		dungeon.map[i] = new Room[difficulty.dungeonSize[difficultyInput]];
		for (int j = 0; j < dungeon.cols; j++)	// x축
		{
			dungeon.map[i][j].locX = j;
			dungeon.map[i][j].locY = i;

			dungeon.map[i][j].fieldType = rand() % 4;
			if (!(i == 0 && j == 0) && rand() % 300 < difficulty.monsterEncounter[difficultyInput])
			{
				// 몬스터 생성
				//dungeon.map[i][j].data = 'M';
				dungeon.map[i][j].monster = monsterTable.CreateMonster(dungeon.map[i][j].fieldType);
			}

			// 0 위 1 아래 2 왼쪽 3 오른쪽
			if (i > 0)
			{
				dungeon.map[i][j].next[0] = &dungeon.map[i - 1][j];
				dungeon.map[i - 1][j].next[1] = &dungeon.map[i][j];
			}
			if (j > 0)
			{
				dungeon.map[i][j].next[2] = &dungeon.map[i][j - 1];
				dungeon.map[i][j - 1].next[3] = &dungeon.map[i][j];
			}
		}
	}

	// 탈출구 정보 적용
	int randNum = rand() % (dungeon.rows * dungeon.cols - 2) + 1;
	dungeon.exitPosX = randNum % dungeon.cols;
	dungeon.exitPosY = randNum / dungeon.rows;
	dungeon.map[dungeon.exitPosY][dungeon.exitPosX].fieldType = 4;

	while (1)
	{
		system("cls");

		// 맵 출력
		cout << "---------------------------------------------------------------------" << endl;
		cout << "      #####  #####  #####  #####      #   #  #####  #####  #####     " << endl;
		cout << "      #        #    #      #   #      #   #  #      #   #  #   #     " << endl;
		cout << "      #####    #    ###    #####      #####  ###    #####  #   #     " << endl;
		cout << "          #    #    #      #          #   #  #      # #    #   #     " << endl;
		cout << "          #    #    #      #          #   #  #      #  #   #   #     " << endl;
		cout << "      #####    #    #####  #          #   #  #####  #   #  #####     " << endl;
		cout << "---------------------------------------------------------------------" << endl;
		cout << "   " << setfill('=') << setw((dungeon.cols * 2 + 3)) << "" << endl;
		cout.copyfmt(std::ios(NULL));
		for (int i = 0; i < dungeon.rows; i++)		// y축
		{
			cout << "   : ";
			for (int j = 0; j < dungeon.cols; j++)	// x축
			{
				if (i == player.posY && j == player.posX) cout << "O ";
				else if (dungeon.map[i][j].monster && !debug) cout << ". ";
				else cout << G_CHAR_FIELD_TYPE[dungeon.map[i][j].fieldType] << " ";
			}
			cout << ":\t    ";
			if (i > 0 && i <= dungeon.noticeCount) cout << dungeon.notice[i - 1] << endl;
			else cout << endl;
		}
		cout << "   " << setfill('=') << setw(dungeon.cols * 2 + 3) << "" << endl;
		cout.copyfmt(std::ios(NULL));

		if (G_CHAR_FIELD_TYPE[dungeon.map[player.posY][player.posX].fieldType] == 'E')
		{
			// 탈출
			cout << "---------------------------------------------------------------------" << endl;
			cout << "     #####     #####      #####         #        #####     #####     " << endl;
			cout << "     #         #          #            # #       #   #     #         " << endl;
			cout << "     ###       #####      #           #   #      #####     ###       " << endl;
			cout << "     #             #      #           #####      #         #         " << endl;
			cout << "     #             #      #           #   #      #         #         " << endl;
			cout << "     #####     #####      #####       #   #      #         #####     " << endl;
			cout << "---------------------------------------------------------------------" << endl;
			break;
		}
		else if (player.state == 0 && dungeon.map[player.posY][player.posX].monster)
		{
			player.state = 1;

			// 몬스터가 있는 위치에 들어옴
			Monster& monster = *(dungeon.map[player.posY][player.posX].monster);
			battleInfo.PrintBattleStatus(player, monster);
			cout << right << setw(53) << "******* 몬스터와 조우하였다!! *******" << endl << endl;
			int playerInput = rand() % 3;
			int monsterInput = rand() % 3;
			string playerBattleLogStr = player.name + " [[ " + battleInfo.item[playerInput].name + " ]]";
			string monsterBattleLogStr = "[[ " + battleInfo.item[monsterInput].name + " ]] " + monster.name;

			cout << right << setw(45) << "*** 가위! 바위! 보! ***" << endl;
			while (playerInput == monsterInput)
			{
				// 무승부 처리
				cout << right << setw(31) << playerBattleLogStr << "  VS  " << monsterBattleLogStr << endl;
				cout << right << setw(42) << "*** 무 승 부 ***" << endl;
				
				playerInput = rand() % 3;
				monsterInput = rand() % 3;
				playerBattleLogStr = player.name + " [[ " + battleInfo.item[playerInput].name + " ]]";
				monsterBattleLogStr = "[[ " + battleInfo.item[monsterInput].name + " ]] " + monster.name;
			}

			cout << right << setw(31) << playerBattleLogStr << "  VS  " << monsterBattleLogStr << endl;
			if (playerInput == (monsterInput + 1) % 3)
			{
				// 승리 처리
				cout << right << setw(42) << "***  승  리  ***" << endl;
				cout << ":::: 몬스터와의 전투에서 승리하였다" << endl;
				if (monster.HitDamage(player.attack) == 1)
				{
					cout << ":::: " << monster.rootExp << " 경험치 획득!!" << endl;
					cout << ":::: " << monster.rootGold << " 골드 획득!!" << endl;

					// 죽었는가?
					player.gold += monster.rootGold;
					int lvlUp = player.RootExp(monster.rootExp);
					if (lvlUp)
					{
						cout << ":::: 플레이어 레벨이 " << lvlUp << " 올랐습니다." << endl;
					}

					delete &monster;
					dungeon.map[player.posY][player.posX].monster = nullptr;
				}
				else
				{
					cout << ":::: 몬스터에게 " << player.attack << " 데미지를 주었다." << endl;
					cout << ":::: 몬스터가 도망갔습니다." << endl;
				}
			}
			else
			{
				// 패배 처리
				cout << right << setw(42) << "***  패  배  ***" << endl;
				cout << ":::: 몬스터와의 전투에서 패배하였다" << endl;
				cout << ":::: " << monster.attack << "의 데미지를 받았다!!" << endl;

				// 플레이어의 상태가 죽은상태인가?
				if (player.HitDamage(monster.attack) == 3)
				{
					cout << "---------------------------------------------------------------------" << endl;
					cout << "        #   #    ###    #     #       #####    #####   #####         " << endl;
					cout << "        #   #   #   #   #     #       #    #     #     #             " << endl;
					cout << "         # #   #     #  #     #       #     #    #     ###           " << endl;
					cout << "          #    #     #  #     #       #     #    #     #             " << endl;
					cout << "          #     #   #   #     #       #    #     #     #             " << endl;
					cout << "          #      ###     #####        #####    #####   #####         " << endl;
					cout << "---------------------------------------------------------------------" << endl;
					break;
				}
			}

			cout << "---------------------------------------------------------------------" << endl;
			cout << ":::: 상점을 이용하시겠습니까? ( 0. 이용하지 않는다 / 1 : 이용한다 ) : ";
			cin >> isUseShop;
		}
		
		if (isUseShop)
		{
			player.state = 2;

			// 상점이용
			int selectItem = 0;
			int itemCount = sizeof(shop.items) / sizeof(ShopItem);
			while (1)
			{
				system("cls");
				cout << "---------------------------------------------------------------------" << endl;
				cout << " #####  #####  #####  #####  #####  #   #    ##### #   # ##### ##### " << endl;
				cout << " #   #  #   #    #      #    #   #  ##  #    #     #   # #   # #   # " << endl;
				cout << " #####  #   #    #      #    #   #  # # #    ##### ##### #   # ##### " << endl;
				cout << " #      #   #    #      #    #   #  # # #        # #   # #   # #     " << endl;
				cout << " #      #   #    #      #    #   #  #  ##        # #   # #   # #     " << endl;
				cout << " #      #####    #    #####  #####  #   #    ##### #   # ##### #     " << endl;
				cout << "---------------------------------------------------------------------" << endl;

				cout << "\n\n                  ---------------------------------                  " << endl;
				for (int i = 0; i < itemCount; ++i)
				{
					ShopItem& item = shop.items[i];
					string itemLabel = to_string((i + 1)) + ". " + item.name + " (" + to_string(item.gold) + " GOLD)\t남은수량 : " + to_string(item.qty);
					if (i == selectItem)
					{
						itemLabel = ">>> " + itemLabel + " <<<";
						cout << right << setw(12 + itemLabel.length()) << itemLabel << endl;
					}
					else
					{
						cout << right << setw(17 + itemLabel.length()) << itemLabel << endl;
					}
				}
				cout << setw((selectItem == itemCount)?29:30) << ((selectItem == itemCount) ? ">>> " : "") << (itemCount+1) << ". 나가기" << ((selectItem == itemCount) ? " <<<" : "") << endl;
				cout << "                  ---------------------------------                  " << endl;
				cout << setw(50) << "CHANGE :: W,S  SELECT :: ENTER" << endl << endl;

				player.PrintHeroStatus();
				//cout << "---------------------------------------------------------------------" << endl;
				//cout << "     [[ " << player.name << " ]]" << endl;
				//cout << "     체력\t: "  << right << setw(10) << string(to_string(player.hp) + " / " + to_string(player.maxHP)) << endl;
				//cout << "     소지금\t: " << right << setw(10) << player.gold << endl;
				//cout << "---------------------------------------------------------------------" << endl;
				
				switch (_getch())
				{
				case 'w':
				case 'W':
					selectItem = (selectItem + itemCount) % (itemCount + 1);
					break;
				case 's':
				case 'S':
					selectItem = (selectItem + 1) % (itemCount + 1);
					break;
				case 13:
					// 상점물품 선택
					if (selectItem < itemCount)
					{ 
						if (player.IsPossibleUseGold(shop.items[selectItem].gold))
						{
							// 해당 골드는 갖고있다.
							if (shop.IsBuyItem(selectItem))
							{
								// 물건이 정상적으로 사졌다
								const ShopItem& item = shop.BuyItem(selectItem);
								player.UseGold(item.gold);
								player.UseItem(item);
								cout << left << ":::: 소지금 " << item.gold << " GOLD 사용하였습니다." << endl;
								cout << "---------------------------------------------------------------------" << endl;
								cout << right << setw(43 + item.name.length()) << string("******* 체력 " + item.name + " *******") << endl;
							}
							else
							{
								// 물건이 없어서 구매 불가
								cout << left << ":::: 품절된 상품입니다 " << endl;
								cout << "---------------------------------------------------------------------" << endl;
							}
						}
						else cout << right << setw(54) << "******* 소지금이 부족합니다!!! *******" << endl << endl;
					}
					else
					{
						isUseShop = false;
						cout << right << setw(51) << "******* 상점을 나갔습니다 *******" << endl << endl;
					}
				
					cout << "::::  아무키나 누르세요.... ";
					_getch();
					break;
				default:
					if (_kbhit())
					{
						_getch();
						cout << endl;
						cout << ":::::  ERROR  :::::" << endl;
						cout << ":::::  입력이 잘못되었습니다.  :::::" << endl;
						cout << ":::::  만약 한글로 되어있다면 한/영키로 영어로 바꿔주세요  :::::" << endl;
						_getch();
					}
					break;
				}
				// 상점을 사용안할거라면 상점에서 나가기
				if (!isUseShop) break;
			}

			// 상점에서 나온 후 지워진 맵정보 재출력
			system("cls");
			cout << "---------------------------------------------------------------------" << endl;
			cout << "      #####  #####  #####  #####      #   #  #####  #####  #####     " << endl;
			cout << "      #        #    #      #   #      #   #  #      #   #  #   #     " << endl;
			cout << "      #####    #    ###    #####      #####  ###    #####  #   #     " << endl;
			cout << "          #    #    #      #          #   #  #      # #    #   #     " << endl;
			cout << "          #    #    #      #          #   #  #      #  #   #   #     " << endl;
			cout << "      #####    #    #####  #          #   #  #####  #   #  #####     " << endl;
			cout << "---------------------------------------------------------------------" << endl;
			cout << "   " << setfill('=') << setw((dungeon.cols * 2 + 3)) << "" << endl;
			cout.copyfmt(std::ios(NULL));
			for (int i = 0; i < dungeon.rows; i++)		// y축
			{
				cout << "   : ";
				for (int j = 0; j < dungeon.cols; j++)	// x축
				{
					if (i == player.posY && j == player.posX) cout << "O ";
					else if (dungeon.map[i][j].monster && !debug) cout << ". ";
					else cout << G_CHAR_FIELD_TYPE[dungeon.map[i][j].fieldType] << " ";
				}
				cout << ":\t    ";
				if (i > 0 && i <= dungeon.noticeCount) cout << dungeon.notice[i - 1] << endl;
				else cout << endl;
			}
			cout << "   " << setfill('=') << setw(dungeon.cols * 2 + 3) << "" << endl;
			cout.copyfmt(std::ios(NULL));
		}

		player.PrintHeroStatus();
		//cout << "---------------------------------------------------------------------" << endl;
		//cout << "     [[ " << player.name << " ]]" << endl;
		//cout << "     체력\t: " << right << setw(10) << string(to_string(player.hp) + " / " + to_string(player.maxHP)) << endl;
		//cout << "     소지금\t: " << right << setw(10) << player.gold << endl;
		//cout << "---------------------------------------------------------------------" << endl;
		
		cout << "::::  이동키를 눌러주세요 (W, A, S, D)..... ";
		int inputKey = _getch();
		switch (tolower(inputKey))
		{
		case 'w':
			if (player.posY > 0)
			{
				--player.posY;
				++isMonsterMove;
				player.state = 0;
			}
			break;
		case 's':
			if (player.posY < dungeon.rows - 1)
			{
				++player.posY;
				++isMonsterMove;
				player.state = 0;
			}
			break;
		case 'a':
			if (player.posX > 0)
			{
				--player.posX;
				++isMonsterMove;
				player.state = 0;
			}
			break;
		case 'd':
			if (player.posX < dungeon.cols - 1)
			{
				++player.posX;
				++isMonsterMove;
				player.state = 0;
			}
			break;
		case 'p':
			debug = !debug;
			break;
		case 'm':
			player.gold += 1000;
			break;
		default:
			if (_kbhit())
			{
				_getch();
				cout << endl;
				cout << ":::::  입력이 잘못되었습니다.  :::::" << endl;
				cout << ":::::  만약 한글로 되어있다면 한/영키로 영어로 바꿔주세요  :::::" << endl;
				_getch();
			}
		}

		if (isMonsterMove >= 2)
		{
			// 몬스터 이동
			isMonsterMove = 0;
			// 필드별 몬스터 귀속으로 했기에 이동 불가
			//for (int i = 0; i < dungeon.rows; i++)
			//{
			//	for (int j = 0; j < dungeon.cols; j++)
			//	{
			//		Room& room = dungeon.map[i][j];
			//		if (room.monster)
			//		{
			//			Room* next = room.next[rand() % 4];
			//			if (next && next->data != 'E')
			//			{
			//				swap(room.data, next->data);
			//			}
			//		}
			//	}
			//}
		}
	}

	// 메모리 해제
	for (int i = 0; i < dungeon.rows; i++)
	{
		for (int j = 0; j < dungeon.cols; ++j)
		{
			if (dungeon.map[i][j].monster) delete dungeon.map[i][j].monster;
		}
		delete[] dungeon.map[i];
	}
	delete[] dungeon.map;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴
