// 20210304_practice2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iomanip>
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <conio.h>

using namespace std;

struct Monster
{
	int attack;
	string name;
};

struct MonsterGroup
{
	Monster monster[4];
};

struct Hero
{
	int maxHP;
	int hp;
	int gold;
	int posX;
	int posY;
	string name;
};

struct Difficulty
{
	int heroHP[3] = { 100, 90, 80 };
	int monsterStrength[3] = { 1, 2, 3 };
	int monsterEncounter[3] = { 20, 40, 60 };
	string name[3] = { "EASY", "NORMAL", "HARD" };
};

struct ShopItem
{
	int gold;
	int heal;
	string name;
};

struct Shop
{
	int posX;
	int posY;
	string name;
	ShopItem items[5];
};

struct BattleItem
{
	string name;
};

struct BattleInfo
{
	BattleItem item[3];
};

struct Dungeon
{
	int rows;
	int cols;
	int exitPosX;
	int exitPosY;

	int noticeCount;
	string notice[7];
};

int main()
{
	srand(time(NULL));

	// 난이도 정보
	Difficulty difficulty;
	int difficultyInput = 0;
	int dungeonSizeX = 10, dungeonSizeY = 10;
	int exitPosX = 0, exitPosY = 0;
	bool isUseShop = false;
	Hero player;
	Dungeon dungeon = { 10, 10, 0, 0 };
	dungeon.notice[0] = "* 던전에서 탈출에 성공하세요 *";
	dungeon.notice[1] = "";
	dungeon.notice[2] = "  O : 플레이어";
	dungeon.notice[3] = "  E : 탈출구";
	dungeon.notice[4] = "";
	dungeon.notice[5] = "  P : 몬스터위치 확인";
	dungeon.notice[6] = "  M : 소지금 1000골드 UP";
	dungeon.noticeCount = sizeof(dungeon.notice) / sizeof(string);

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
				player = { difficulty.heroHP[difficultyInput], difficulty.heroHP[difficultyInput], 1000, 0, 0 };
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

	// 맵 데이터 저장
	char map[10][10];
	for (int i = 0; i < dungeonSizeY; i++)		// y축
	{
		for (int j = 0; j < dungeonSizeX; j++)	// x축
		{
			map[i][j] = '.';
			if (i != 0 && j != 0
				&& rand() % 100 < difficulty.monsterEncounter[difficultyInput]) map[i][j] = 'M';
		}
	}

	// 탈출구 정보 적용
	int randNum = rand() % (dungeonSizeX * dungeonSizeY - 2) + 1;
	exitPosX = randNum % dungeonSizeX;
	exitPosY = randNum / dungeonSizeY;
	map[exitPosY][exitPosX] = 'E';

	// 상점의 정보
	Shop shop;
	shop.items[0] = { 100, 999, "전체회복" };
	shop.items[1] = { 10, 10, "10회복" };
	shop.items[2] = { 20, 20, "20회복" };
	shop.items[3] = { 30, 30, "30회복" };
	shop.items[4] = { 40, 40, "40회복" };

	// 배틀정보
	BattleInfo battleInfo;
	battleInfo.item[0] = { "가위" };
	battleInfo.item[1] = { "바위" };
	battleInfo.item[2] = { " 보 " };

	// 몬스터 설정
	MonsterGroup MonsterGroup;
	MonsterGroup.monster[0] = { 10 * difficulty.monsterStrength[difficultyInput], "청룡" };
	MonsterGroup.monster[1] = { 10 * difficulty.monsterStrength[difficultyInput], "주작" };
	MonsterGroup.monster[2] = { 10 * difficulty.monsterStrength[difficultyInput], "백호" };
	MonsterGroup.monster[3] = { 10 * difficulty.monsterStrength[difficultyInput], "현무" };

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
		cout << "   " << setfill('=') << setw((dungeonSizeX * 2 + 3)) << "" << endl;
		cout.copyfmt(std::ios(NULL));
		for (int i = 0; i < dungeonSizeY; i++)		// y축
		{
			cout << "   : ";
			for (int j = 0; j < dungeonSizeX; j++)	// x축
			{
				if (i == player.posY && j == player.posX) cout << "O ";
				else if (map[i][j] == 'M' && !debug) cout << ". ";
				else cout << map[i][j] << " ";
			}
			cout << ":\t    ";
			if (i > 0 && i <= dungeon.noticeCount) cout << dungeon.notice[i - 1] << endl;
			else cout << endl;
		}
		cout << "   " << setfill('=') << setw(dungeonSizeX * 2 + 3) << "" << endl;
		cout.copyfmt(std::ios(NULL));

		if (map[player.posY][player.posX] == 'E')
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
		else if (map[player.posY][player.posX] == 'M')
		{
			// 몬스터가 있는 위치에 들어옴
			Monster monster = MonsterGroup.monster[rand()%4];
			cout << "---------------------------------------------------------------------" << endl;
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
				int dropGold = rand() % 100 + 1;
				player.gold += dropGold;
				cout << right << setw(42) << "***  승  리  ***" << endl;
				cout << "---------------------------------------------------------------------" << endl;
				cout << ":::: 몬스터와의 전투에서 승리하였다 ==" << endl;
				cout << ":::: " << dropGold << "골드 획득!! ==" << endl;
				map[player.posY][player.posX] = '.';
			}
			else
			{
				// 패배 처리
				player.hp -= monster.attack;
				if (player.hp <= 0) player.hp = 0;
				cout << right << setw(42) << "***  패  배  ***" << endl;
				cout << "---------------------------------------------------------------------" << endl;
				cout << ":::: 몬스터와의 전투에서 패배하였다 ==" << endl;
				cout << ":::: " << monster.attack << "의 데미지를 받았다!! ==" << endl;
			}

			if (player.hp == 0)
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

			cout << "---------------------------------------------------------------------" << endl;
			cout << ":::: 상점을 이용하시겠습니까? ( 0. 이용하지 않는다 / 1 : 이용한다 ) : ";
			cin >> isUseShop;
		}
		
		if (isUseShop)
		{
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
					string itemLabel = to_string((i + 1)) + ". " + item.name + " (" + to_string(item.gold) + " GOLD)";
					if (i == selectItem)
					{
						cout << setw(23) << ">>> " << itemLabel << " <<<" << endl;
					}
					else
					{
						cout << right << setw(24 + itemLabel.length()) << itemLabel << endl;
					}
				}
				cout << setw((selectItem == itemCount)?23:24) << ((selectItem == itemCount) ? ">>> " : "") << (itemCount+1) << ". 나가기" << ((selectItem == itemCount) ? " <<<" : "") << endl;
				cout << "                  ---------------------------------                  " << endl;
				cout << setw(50) << "CHANGE :: W,S  SELECT :: ENTER" << endl << endl;
				cout << "---------------------------------------------------------------------" << endl;
				cout << "     [[ " << player.name << " ]]" << endl;
				cout << "     체력\t: "  << right << setw(10) << string(to_string(player.hp) + " / " + to_string(player.maxHP)) << endl;
				cout << "     소지금\t: " << right << setw(10) << player.gold << endl;
				cout << "---------------------------------------------------------------------" << endl;
				
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
						if (player.gold - shop.items[selectItem].gold >= 0)
						{
							player.hp += shop.items[selectItem].heal;
							if (player.hp > player.maxHP) player.hp = player.maxHP;
							player.gold -= shop.items[selectItem].gold;
							cout << right << setw(43 + shop.items[selectItem].name.length()) << string("******* 체력 " + shop.items[selectItem].name + " *******") << endl;
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
			cout << "   " << setfill('=') << setw((dungeonSizeX * 2 + 3)) << "" << endl;
			cout.copyfmt(std::ios(NULL));
			for (int i = 0; i < dungeonSizeY; i++)		// y축
			{
				cout << "   : ";
				for (int j = 0; j < dungeonSizeX; j++)	// x축
				{
					if (i == player.posY && j == player.posX) cout << "O ";
					else if (map[i][j] == 'M' && !debug) cout << ". ";
					else cout << map[i][j] << " ";
				}
				cout << ":\t    ";
				if (i > 0 && i <= dungeon.noticeCount) cout << dungeon.notice[i - 1] << endl;
				else cout << endl;
			}
			cout << "   " << setfill('=') << setw(dungeonSizeX * 2 + 3) << "" << endl;
			cout.copyfmt(std::ios(NULL));
		}

		cout << "---------------------------------------------------------------------" << endl;
		cout << "     [[ " << player.name << " ]]" << endl;
		cout << "     체력\t: " << right << setw(10) << string(to_string(player.hp) + " / " + to_string(player.maxHP)) << endl;
		cout << "     소지금\t: " << right << setw(10) << player.gold << endl;
		cout << "---------------------------------------------------------------------" << endl;
		
		cout << "::::  이동키를 눌러주세요 (W, A, S, D)..... ";
		int inputKey = _getch();
		switch (tolower(inputKey))
		{
		case 'w':
			if (player.posY > 0)
			{
				--player.posY;
			}
			break;
		case 's':
			if (player.posY < 9)
			{
				++player.posY;
			}
			break;
		case 'a':
			if (player.posX > 0)
			{
				--player.posX;
			}
			break;
		case 'd':
			if (player.posX < 9)
			{
				++player.posX;
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
	}
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴
