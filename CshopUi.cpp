#include "framework.h"
#include "CshopUi.h"

HRESULT CshopUi::init()
{
	m_shopUiX = 100;
	m_shopUiY = 150;

	m_shopRect = RectMake(m_shopUiX, m_shopUiY, IMAGE->findImage("상점창")->getWidth(), IMAGE->findImage("상점창")->getHeight());

	m_shopListX = m_shopUiX + 20;
	m_shopListY = m_shopUiY + 25;

	for (int i = 0; i < 6; i++)
	{
		RECT temp;
		temp = RectMake(m_shopListX, m_shopListY + i * 43, 200, 40);
		m_vShopListRect.push_back(temp);
	}

	m_shopInfoX = m_shopUiX + 250;
	m_shopInfoY = m_shopUiY + 35;

	m_shopInfo = RectMake(m_shopInfoX, m_shopInfoY, 200, 80);

	m_shopButtonX = m_shopUiX + 30;
	m_shopButtonY = m_shopUiY + 390;

	m_buyButton = RectMake(m_shopButtonX, m_shopButtonY, 100, 50);

	m_inventoryUiX = 570;
	m_inventoryUiY = 150;

	m_inventoryRect = RectMake(m_inventoryUiX, m_inventoryUiY, IMAGE->findImage("인벤토리1")->getWidth(), IMAGE->findImage("인벤토리1")->getHeight());
	
	m_itemListX = m_inventoryUiX + 20;
	m_itemListY = m_inventoryUiY + 25;

	for (int i = 0; i < 4; i++)
	{
		RECT temp;
		temp = RectMake(m_itemListX, m_itemListY + i * 88, 200, 80);
		m_vItemListRect.push_back(temp);
	}

	m_itemInfoX = m_inventoryUiX + 250;
	m_itemInfoY = m_inventoryUiY + 35;

	m_ItemInfoRect = RectMake(m_itemInfoX, m_itemInfoY, 200, 80);

	m_inventoryButtonX = m_inventoryUiX + 30;
	m_inventoryButtonY = m_inventoryUiY + 390;

	m_sellButton = RectMake(m_inventoryButtonX, m_inventoryButtonY, 70, 20);

	m_selectType = ITEMTYPE::ITEMTYPE_WEAPON;
	m_showIndex = 0;
	isSelectShop = false;
	isSelectRender = false;

	isButtonClick = false;
	isKeyUp = true;

	return S_OK;
}

void CshopUi::release()
{
}

void CshopUi::update()
{
	this->selectItem();
	this->setShowIndex();
	this->selectShopItem();

	if (InputManager->isStayKeyDown(VK_LBUTTON))
	{
		isButtonClick = true;
	}
	else
	{
		isButtonClick = false;
		isKeyUp = true;
	}

	if (isButtonClick)
	{
		if (isKeyUp)
		{
			this->buyItem();
			
		}
	}

	if (m_selectShopItem != nullptr)
	{
		m_selectType = m_selectShopItem->getType();
	}
}

void CshopUi::render()
{
	IMAGE->findImage("상점창")->render(getMemDC(), m_shopRect.left, m_shopRect.top);
	IMAGE->findImage("인벤토리1")->render(getMemDC(), m_inventoryRect.left, m_inventoryRect.top);

	for (auto iter = m_vShopListRect.begin(); iter != m_vShopListRect.end(); ++iter)
	{
		//Rectangle(getMemDC(), (*iter).left, (*iter).top, (*iter).right, (*iter).bottom);
	}

	for (auto iter = m_vItemListRect.begin(); iter != m_vItemListRect.end(); ++iter)
	{
		//Rectangle(getMemDC(), (*iter).left, (*iter).top, (*iter).right, (*iter).bottom);
	}
	Rectangle(getMemDC(), m_sellButton.left, m_sellButton.top, m_sellButton.right, m_sellButton.bottom);
	Rectangle(getMemDC(), m_buyButton.left, m_buyButton.top, m_buyButton.right, m_buyButton.bottom);
	//Rectangle(getMemDC(), m_ItemInfoRect.left, m_ItemInfoRect.top, m_ItemInfoRect.right, m_ItemInfoRect.bottom);

	this->showItemType();
	if (isSelectRender)
		IMAGE->findImage("선택테두리")->render(getMemDC(), m_selectRenderX, m_selectRenderY);
	if (isSelectRender)
		this->showItemInfo();

	if (isSelectShop)
		this->showShopInfo();

	this->showShopItemList(m_shopItem->getTotalList());
}


void CshopUi::showInvenItemList(vector<Citem>* item)
{
	char str[100];
	// show 인덱스는 기본 0~3까지만 보여준다, 이후 스크롤로 1~4 이런식으로 보여주기
	int	i = m_showIndex;
	int j = 0;
	for (auto iter = item->begin() + i; iter != item->end() && i < m_showEndIndex; ++iter, i++, j++)
	{
		IMAGE->findImage((*iter).getSmallImage())->frameRender(getMemDC()
			, m_vItemListRect[j].left, m_vItemListRect[j].top, (*iter).getFrame().x, (*iter).getFrame().y);

		SetTextColor(getMemDC(), RGB(255, 255, 255));
		TextOut(getMemDC(), m_vItemListRect[j].left + 80, m_vItemListRect[j].top + 10, (*iter).getName().c_str(), lstrlen((*iter).getName().c_str()));
		wsprintf(str, "필요 레벨: %d", (*iter).getLimitLevel());
		TextOut(getMemDC(), m_vItemListRect[j].left + 80, m_vItemListRect[j].top + 30, str, lstrlen(str));
		wsprintf(str, "골드: %d", (*iter).getBuyPrice());
		TextOut(getMemDC(), m_vItemListRect[j].left + 80, m_vItemListRect[j].top + 50, str, lstrlen(str));
	}
}

void CshopUi::showItemInfo()
{
	char str[100];

	IMAGE->findImage(m_selectItem->getSmallImage())->frameRender(getMemDC(), m_ItemInfoRect.left + 11, m_ItemInfoRect.top
		, m_selectItem->getFrame().x, m_selectItem->getFrame().y);

	SetTextColor(getMemDC(), RGB(255, 255, 255));
	TextOut(getMemDC(), m_ItemInfoRect.left + 80, m_ItemInfoRect.top + 10, m_selectItem->getName().c_str(), lstrlen(m_selectItem->getName().c_str()));
	wsprintf(str, "필요 레벨: %d", m_selectItem->getLimitLevel());
	TextOut(getMemDC(), m_ItemInfoRect.left + 80, m_ItemInfoRect.top + 30, str, lstrlen(str));
	wsprintf(str, "골드: %d", m_selectItem->getBuyPrice());
	TextOut(getMemDC(), m_ItemInfoRect.left + 80, m_ItemInfoRect.top + 50, str, lstrlen(str));
	
	this->itemStatInfo();


}

void CshopUi::itemStatInfo()
{
	char grade[100];
	char atk[100];
	char def[100];
	char hp[100];
	char mp[100];
	char cri[100];
	char criAtk[100];
	char speed[100];
	char level[100];
	
	wsprintf(grade, "등급 : %s", m_selectItem->getItemGrade().c_str());
	wsprintf(atk, "공격력: %d", m_selectItem->getAtk());
	wsprintf(def, "방어력: %d", m_selectItem->getDef());
	wsprintf(hp, "HP: %d", m_selectItem->getHp());
	wsprintf(mp, "MP: %d", m_selectItem->getMp());
	wsprintf(cri, "크리율: %d%", m_selectItem->getCritical());
	sprintf(criAtk, "크리공격력: %.1f", m_selectItem->getCriticalAtk());
	sprintf(speed, "이동속도: %.1f", m_selectItem->getSpeed());
	wsprintf(level, "제한레벨: %d", m_selectItem->getLimitLevel());

	switch (m_selectItem->getType())
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 280, level, lstrlen(level));
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 280, level, lstrlen(level));
		break;
	case ITEMTYPE::ITEMTYPE_SHOES:
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 280, level, lstrlen(level));
		break;
	case ITEMTYPE::ITEMTYPE_GLOVES:
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 280, level, lstrlen(level));
		break;
	case ITEMTYPE::ITEMTYPE_PENDANT:
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_ItemInfoRect.left + 10, m_ItemInfoRect.top + 280, level, lstrlen(level));
		break;
	default:
		break;
	}
}

void CshopUi::showItemType()
{
	switch (m_selectType)
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		showInvenItemList(m_myInventory->getvWeaponList());
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		showInvenItemList(m_myInventory->getvArmorList());
		break;
	case ITEMTYPE::ITEMTYPE_SHOES:
		showInvenItemList(m_myInventory->getvShoesList());
		break;
	case ITEMTYPE::ITEMTYPE_GLOVES:
		showInvenItemList(m_myInventory->getvGlovesList());
		break;
	case ITEMTYPE::ITEMTYPE_PENDANT:
		showInvenItemList(m_myInventory->getvPendantList());
		break;
	default:
		break;
	}
}

// 아직 예외처리 완성 안됨.. 스크롤 끝까지 내렸을때 예외처리 필요
void CshopUi::setShowIndex()
{
	switch (m_selectType)
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		if (m_myInventory->getvWeaponList()->size() < 4)
			m_showEndIndex = m_myInventory->getvWeaponList()->size() + m_showIndex;
		else
			m_showEndIndex = m_showIndex + 4;
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		if (m_myInventory->getvArmorList()->size() < 4)
			m_showEndIndex = m_myInventory->getvArmorList()->size() + m_showIndex;
		else
			m_showEndIndex = m_showIndex + 4;
		break;
	default:
		break;
	}
}

// 인벤토리 아이템 선택
void CshopUi::selectItem()
{
	vector<Citem>::iterator weaponIter;
	vector<Citem>::iterator armorIter;
	int i = m_showIndex;
	for (auto iter = m_vItemListRect.begin(); iter != m_vItemListRect.end() && i < m_showEndIndex; ++iter, i++)
	{
		if (PtInRect(&(*iter), m_ptMouse))
		{
			if (isButtonClick)
			{
				if (isKeyUp)
				{
					isSelectRender = true;
					m_selectRenderX = (*iter).left - 5;
					m_selectRenderY = (*iter).top - 3;

					// 선택한 아이템 정보 inventory 에서 가져와서 selectItem 변수에 담기
					switch (m_selectType)
					{
					case ITEMTYPE::ITEMTYPE_WEAPON:
						weaponIter = m_myInventory->getvWeaponList()->begin() + i;
						m_selectItem = &(*weaponIter);
						break;
					case ITEMTYPE::ITEMTYPE_ARMOR:
						armorIter = m_myInventory->getvArmorList()->begin() + i;
						m_selectItem = &(*armorIter);
						break;
					default:
						break;

						isKeyUp = false;
					}
				}
			}
		}
	}
}

void CshopUi::showEquipSelect()
{
	// 현재 장착중인 아이템들은 myInventory에서 정보 가져오기 -> 해당 아이템 인덱스를 토대로 장착테두리 표시... 

	/*switch (m_selectType)
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		if (isEquipWeapon)
			IMAGE->findImage("장착테두리")->render(getMemDC(), m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_WEAPON)].x,
				m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_WEAPON)].y);
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		if (isEquipArmor)
			IMAGE->findImage("장착테두리")->render(getMemDC(), m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_ARMOR)].x,
				m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_ARMOR)].y);
		break;
	}*/
}


void CshopUi::sellItem()
{
}

void CshopUi::buyItem()
{
	if (PtInRect(&m_buyButton, m_ptMouse))
	{
		if (m_selectShopItem != nullptr)
		{
			m_myInventory->addItem(m_selectShopItem);
		}
		isKeyUp = false;
	}
}

void CshopUi::showShopItemList(vector<Citem>* item)
{
	char str[100];

	int i = 0;
	for (auto iter = item->begin(); iter != item->end(); ++iter, i++)
	{
		IMAGE->findImage((*iter).getSmallImage())->frameRender(getMemDC()
			, m_vShopListRect[i].left, m_vShopListRect[i].top, (*iter).getFrame().x, (*iter).getFrame().y);

		SetTextColor(getMemDC(), RGB(255, 255, 255));
		TextOut(getMemDC(), m_vShopListRect[i].left + 40, m_vShopListRect[i].top + 15, (*iter).getName().c_str(), lstrlen((*iter).getName().c_str()));
	/*	wsprintf(str, "필요 레벨: %d", (*iter).getLimitLevel());
		TextOut(getMemDC(), m_vShopListRect[i].left + 80, m_vShopListRect[i].top + 30, str, lstrlen(str));*/
		wsprintf(str, "골드: %d", (*iter).getBuyPrice());
		TextOut(getMemDC(), m_vShopListRect[i].left + 130, m_vShopListRect[i].top + 15, str, lstrlen(str));
	}
}

void CshopUi::selectShopItem()
{
	vector<Citem>::iterator shopItemIter;

	int i = 0;
	for (auto iter = m_vShopListRect.begin(); iter != m_vShopListRect.end() && i < 5; ++iter, i++)
	{
		if (PtInRect(&(*iter), m_ptMouse))
		{
			if (InputManager->isOnceKeyDown(VK_LBUTTON))
			{
				isSelectShop = true;
				shopItemIter = m_shopItem->getTotalList()->begin() + i;
				m_selectShopItem = &(*shopItemIter);
			}
		}
	}
}

void CshopUi::showShopInfo()
{
	char str[100];

	IMAGE->findImage(m_selectShopItem->getSmallImage())->frameRender(getMemDC(), m_shopInfo.left + 11, m_shopInfo.top
		, m_selectShopItem->getFrame().x, m_selectShopItem->getFrame().y);

	SetTextColor(getMemDC(), RGB(255, 255, 255));
	TextOut(getMemDC(), m_shopInfo.left + 80, m_shopInfo.top + 10, m_selectShopItem->getName().c_str(), lstrlen(m_selectShopItem->getName().c_str()));
	wsprintf(str, "필요 레벨: %d", m_selectShopItem->getLimitLevel());
	TextOut(getMemDC(), m_shopInfo.left + 80, m_shopInfo.top + 30, str, lstrlen(str));
	wsprintf(str, "골드: %d", m_selectShopItem->getBuyPrice());
	TextOut(getMemDC(), m_shopInfo.left + 80, m_shopInfo.top + 50, str, lstrlen(str));

	this->shopItemStatInfo();
}

void CshopUi::shopItemStatInfo()
{
	char grade[100];
	char atk[100];
	char def[100];
	char hp[100];
	char mp[100];
	char cri[100];
	char criAtk[100];
	char speed[100];
	char level[100];

	wsprintf(grade, "등급 : %s", m_selectShopItem->getItemGrade().c_str());
	wsprintf(atk, "공격력: %d", m_selectShopItem->getAtk());
	wsprintf(def, "방어력: %d", m_selectShopItem->getDef());
	wsprintf(hp, "HP: %d", m_selectShopItem->getHp());
	wsprintf(mp, "MP: %d", m_selectShopItem->getMp());
	wsprintf(cri, "크리율: %d%", m_selectShopItem->getCritical());
	sprintf(criAtk, "크리공격력: %.1f", m_selectShopItem->getCriticalAtk());
	sprintf(speed, "이동속도: %.1f", m_selectShopItem->getSpeed());
	wsprintf(level, "제한레벨: %d", m_selectShopItem->getLimitLevel());

	switch (m_selectShopItem->getType())
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 280, level, lstrlen(level));
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 280, level, lstrlen(level));
		break;
	case ITEMTYPE::ITEMTYPE_SHOES:
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 280, level, lstrlen(level));
		break;
	case ITEMTYPE::ITEMTYPE_GLOVES:
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 280, level, lstrlen(level));
		break;
	case ITEMTYPE::ITEMTYPE_PENDANT:
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 120, grade, lstrlen(grade));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 140, atk, lstrlen(atk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 160, def, lstrlen(def));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 180, hp, lstrlen(hp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 200, mp, lstrlen(mp));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 220, cri, lstrlen(cri));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 240, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 260, speed, lstrlen(speed));
		TextOut(getMemDC(), m_shopInfo.left + 10, m_shopInfo.top + 280, level, lstrlen(level));
		break;
	default:
		break;
	}
}
