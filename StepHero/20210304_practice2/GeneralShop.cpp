#include "stdafx.h"
#include "GeneralShop.h"


GeneralShop::GeneralShop()
{
}

GeneralShop::GeneralShop(int width, int height)
{
	this->width = width;
	this->height = height;
	view.Init(width, height);
	view.AddLayout(view.TL_TOP, view.RT_STATIC, 0, 8);
	view.AddLayout(view.TL_CONTENT, view.RT_EVERYTIME, 8, 20);
	view.AddLayout(view.TL_BOTTOM, view.RT_EVERYTIME, 28, 22);

	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#####  #####  #####  #####  #####  #   #    ##### #   # ##### #####"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#   #  #   #    #      #    #   #  ##  #    #     #   # #   # #   #"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#####  #   #    #      #    #   #  # # #    ##### ##### #   # #####"));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#      #   #    #      #    #   #  # # #        # #   # #   # #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#      #   #    #      #    #   #  #  ##        # #   # #   # #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "#      #####    #    #####  #####  #   #    ##### #   # ##### #    "));
	view.Write(view.TL_TOP, MakeString(TA_CENTER, width, "---------------------------------------------------------------------"));

	shop.AddShopItem({ 100, 999, 5, "전체회복" });
	shop.AddShopItem({ 10, 10, 10, "10회복" });
	shop.AddShopItem({ 20, 20, 20, "20회복" });
	shop.AddShopItem({ 30, 30, 30, "30회복" });
	shop.AddShopItem({ 40, 40, 40, "40회복" });
}

GeneralShop::~GeneralShop()
{
}

void GeneralShop::Init()
{
	state = ShopState::INIT;
	player = gGameData.GetHeroPtr();
}

void GeneralShop::Update()
{
	if (state == ShopState::INIT)
	{
		state = ShopState::READY;
		gKeyManager.Request(KeyManager::InputType::SELECT);
	}
	else if (state == ShopState::READY)
	{
		if (gKeyManager.IsPressKey('w'))
		{
			selectItemNo = (selectItemNo + shop.GetItemCount()) % (shop.GetItemCount() + 1);
			gKeyManager.Request(KeyManager::InputType::SELECT);
		}
		else if (gKeyManager.IsPressKey('s'))
		{
			selectItemNo = (selectItemNo + 1) % (shop.GetItemCount() + 1);
			gKeyManager.Request(KeyManager::InputType::SELECT);
		}
		else if (gKeyManager.IsPressKey(13))
		{
			// ENTER
			if (selectItemNo < shop.GetItemCount())
			{
				if (player->IsPossibleUseGold(shop.GetItemGold(selectItemNo)))
				{
					if (shop.IsBuyItem(selectItemNo))
					{
						player->UseGold(shop.GetItemGold(selectItemNo));
						player->UseItem(shop.BuyItem(selectItemNo));
						gKeyManager.Request(KeyManager::InputType::SELECT);
						state = ShopState::BUY_SUCCESS;
						return;
					}
					else
					{
						state = ShopState::BUY_FAIL_QTY;
						gKeyManager.Request(KeyManager::InputType::ANYKEYS);
					}
				}
				else
				{
					state = ShopState::BUY_FAIL_GOLD;
					gKeyManager.Request(KeyManager::InputType::ANYKEYS);
				}
			}
			else
			{
				// 나가기 선택
				gKeyManager.Clear();
				gTextViewManager.ChangeView(gTextViewManager.VT_INGAME, gTextViewManager.AT_ZIGZAG_OUT_IN, 1000);
			}
		}
	}
	else if (state == ShopState::BUY_SUCCESS
		|| state == ShopState::BUY_FAIL_GOLD
		|| state == ShopState::BUY_FAIL_QTY)
	{
		state = ShopState::READY;
		gKeyManager.Request(KeyManager::InputType::ANYKEYS);
	}
}

void GeneralShop::Render(vector<string>* targetBuffer)
{
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "-------------------------------------"));
	shop.Render(view, selectItemNo);
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, width, "-------------------------------------"));
	view.Write(view.TL_CONTENT, MakeString(TA_CENTER, view.GetWidth(), "CHANGE :: W,S  SELECT :: ENTER"));

	view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, string(90, '-')));
	player->Render(view, view.TL_BOTTOM);
	view.Write(view.TL_BOTTOM, MakeString(TA_CENTER, width, string(90, '-')));
	if (state == ShopState::READY)
	{
		if (gKeyManager.inputResult == KeyManager::InputResult::FAIL)
		{
			view.Write(view.TL_BOTTOM, " ");
			view.Write(view.TL_BOTTOM, ":: [ ERROR ]");
			view.Write(view.TL_BOTTOM, ":: 입력이 잘못되었습니다.");
			view.Write(view.TL_BOTTOM, ":: 한글로 되어있다면 한/영 키로 영어로 바꿔주세요.");
		}
	}
	else if (state == ShopState::BUY_SUCCESS)
	{
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: 소지금 " + to_string(shop.GetItemGold(selectItemNo)) + "GOLD를 사용하였습니다.");
	}
	else if (state == ShopState::BUY_FAIL_GOLD)
	{
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: 소지금이 부족합니다");
	}
	else if (state == ShopState::BUY_FAIL_QTY)
	{
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: 매진된 상품입니다.");
	}

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

	// 뷰에있는걸 출력
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();
}

void GeneralShop::Release()
{
}
