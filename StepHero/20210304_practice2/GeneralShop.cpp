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

	shop.AddShopItem({ 100, 999, 5, "��üȸ��" });
	shop.AddShopItem({ 10, 10, 10, "10ȸ��" });
	shop.AddShopItem({ 20, 20, 20, "20ȸ��" });
	shop.AddShopItem({ 30, 30, 30, "30ȸ��" });
	shop.AddShopItem({ 40, 40, 40, "40ȸ��" });
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
				// ������ ����
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
			view.Write(view.TL_BOTTOM, ":: �Է��� �߸��Ǿ����ϴ�.");
			view.Write(view.TL_BOTTOM, ":: �ѱ۷� �Ǿ��ִٸ� ��/�� Ű�� ����� �ٲ��ּ���.");
		}
	}
	else if (state == ShopState::BUY_SUCCESS)
	{
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: ������ " + to_string(shop.GetItemGold(selectItemNo)) + "GOLD�� ����Ͽ����ϴ�.");
	}
	else if (state == ShopState::BUY_FAIL_GOLD)
	{
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: �������� �����մϴ�");
	}
	else if (state == ShopState::BUY_FAIL_QTY)
	{
		view.Write(view.TL_BOTTOM, " ");
		view.Write(view.TL_BOTTOM, ":: ������ ��ǰ�Դϴ�.");
	}

	//			for (int i = 0; i < itemCount; ++i)
	//			{
	//				ShopItem& item = shop.items[i];
	//				string itemLabel = to_string((i + 1)) + ". " + item.name + " (" + to_string(item.gold) + " GOLD)\t�������� : " + to_string(item.qty);
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
	//			cout << setw((selectItem == itemCount)?29:30) << ((selectItem == itemCount) ? ">>> " : "") << (itemCount+1) << ". ������" << ((selectItem == itemCount) ? " <<<" : "") << endl;
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
	//				// ������ǰ ����
	//				if (selectItem < itemCount)
	//				{ 
	//					if (player.IsPossibleUseGold(shop.items[selectItem].gold))
	//					{
	//						// �ش� ���� �����ִ�.
	//						if (shop.IsBuyItem(selectItem))
	//						{
	//							// ������ ���������� ������
	//							const ShopItem& item = shop.BuyItem(selectItem);
	//							player.UseGold(item.gold);
	//							player.UseItem(item);
	//							cout << left << ":::: ������ " << item.gold << " GOLD ����Ͽ����ϴ�." << endl;
	//							cout << "---------------------------------------------------------------------" << endl;
	//							cout << right << setw(43 + item.name.length()) << string("******* ü�� " + item.name + " *******") << endl;
	//						}
	//						else
	//						{
	//							// ������ ��� ���� �Ұ�
	//							cout << left << ":::: ǰ���� ��ǰ�Դϴ� " << endl;
	//							cout << "---------------------------------------------------------------------" << endl;
	//						}
	//					}
	//					else cout << right << setw(54) << "******* �������� �����մϴ�!!! *******" << endl << endl;
	//				}
	//				else
	//				{
	//					isUseShop = false;
	//					cout << right << setw(51) << "******* ������ �������ϴ� *******" << endl << endl;
	//				}
	//			
	//				cout << "::::  �ƹ�Ű�� ��������.... ";
	//				_getch();
	//				break;
	//			default:
	//				if (_kbhit())
	//				{
	//					_getch();
	//					cout << endl;
	//					cout << ":::::  ERROR  :::::" << endl;
	//					cout << ":::::  �Է��� �߸��Ǿ����ϴ�.  :::::" << endl;
	//					cout << ":::::  ���� �ѱ۷� �Ǿ��ִٸ� ��/��Ű�� ����� �ٲ��ּ���  :::::" << endl;
	//					_getch();
	//				}
	//				break;
	//			}
	//			// ������ �����ҰŶ�� �������� ������
	//			if (!isUseShop) break;

	// �信�ִ°� ���
	if (targetBuffer) view.CopyTo(*targetBuffer);
	else view.Render();
}

void GeneralShop::Release()
{
}
