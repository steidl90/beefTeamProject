#include"framework.h"
#include "CinventoryUi.h"
#include "Cplayer.h"
CinventoryUi::CinventoryUi()
{
}

CinventoryUi::~CinventoryUi()
{
}

HRESULT CinventoryUi::init()
{
	// 전체 UI 좌표
	m_uiX = 200;
	m_uiY = 200;

	m_InventoryRect = RectMake(m_uiX, m_uiY, IMAGE->findImage("인벤토리3")->getWidth(),
		IMAGE->findImage("인벤토리3")->getHeight());

	// 장비창 내부 rect 기준 좌표
	m_EquipItemX = m_uiX + 60;
	m_EquipItemY = m_uiY + 70;

	// 서브템 rect
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			RECT temp;
			temp = RectMake(m_EquipItemX + j * 100, m_EquipItemY + i * 250, 70, 70);
			m_vEquipItemSubRect.push_back(temp);
		}
	}
	// 메인템 rect
	for (int i = 0; i < 3; i++)
	{
		RECT temp;
		temp = RectMake(m_EquipItemX + i * 100, m_EquipItemY + 110, 70, 107);
		m_vEquipItemMainRect.push_back(temp);
	}

	// 보유 리스트 내부 rect 기준 좌표
	m_itemListX = m_uiX + 410;
	m_itemListY = m_uiY + 25;

	for (int i = 0; i < 4; i++)
	{
		RECT temp;
		temp = RectMake(m_itemListX, m_itemListY + i * 88, 200, 80);
		m_vItemListRect.push_back(temp);
	}

	// 
	m_itemInfoX = m_uiX + 645;
	m_itemInfoY = m_uiY + 30;

	m_ItemInfoRect = RectMake(m_itemInfoX, m_itemInfoY, 180, 80);

	// 기능 버튼 rect 기준 좌표
	m_functionButtonX = m_uiX + 430;
	m_functionButtonY = m_uiY + 390;

	m_equipButton = RectMake(m_functionButtonX, m_functionButtonY, 70, 20);
	m_unEquipButton = RectMake(m_functionButtonX + 90, m_functionButtonY, 70, 20);
	m_abandonButton = RectMake(m_functionButtonX, m_functionButtonY + 24, 70, 20);
	m_exitButton = RectMake(m_functionButtonX + 90, m_functionButtonY + 24, 70, 20);

	isKeyUp = false;
	isButtonClick = false;

	m_showIndex = 0;
	m_showEndIndex = 0;
	m_selectType = ITEMTYPE::ITEMTYPE_ARMOR;
	isSelectRender = false;
	m_clickCount = 0;

	isEquipWeapon = false;
	return S_OK;
}

void CinventoryUi::release()
{
}

void CinventoryUi::update()
{
	this->selectItem();
	this->selectEquipItem();
	this->selectItemType();
	this->abandonItem();
	// 상시 set을 적용할것인지 특정 타이밍에 set할것인지 일단 고민
	this->setShowIndex();

	if (InputManager->isStayKeyDown(VK_LBUTTON))
	{
		isButtonClick = true;
	}
	else
	{
		isButtonClick = false;
		isKeyUp = true;
	}
}

void CinventoryUi::render()
{
	IMAGE->findImage("인벤토리3")->render(getMemDC(), m_uiX, m_uiY);
	
	for (auto iter = m_vItemListRect.begin(); iter != m_vItemListRect.end(); ++iter)
	{
		//Rectangle(getMemDC(), (*iter).left, (*iter).top, (*iter).right, (*iter).bottom);
	}
	for (auto iter = m_vEquipItemSubRect.begin(); iter != m_vEquipItemSubRect.end(); ++iter)
	{
		//Rectangle(getMemDC(), (*iter).left, (*iter).top, (*iter).right, (*iter).bottom);
	}
	for (auto iter = m_vEquipItemMainRect.begin(); iter != m_vEquipItemMainRect.end(); ++iter)
	{
		//Rectangle(getMemDC(), (*iter).left, (*iter).top, (*iter).right, (*iter).bottom);
	}
	//Rectangle(getMemDC(), m_ItemInfoRect.left, m_ItemInfoRect.top, m_ItemInfoRect.right, m_ItemInfoRect.bottom);

	Rectangle(getMemDC(), m_equipButton.left, m_equipButton.top, m_equipButton.right, m_equipButton.bottom);
	Rectangle(getMemDC(), m_unEquipButton.left, m_unEquipButton.top, m_unEquipButton.right, m_unEquipButton.bottom);
	Rectangle(getMemDC(), m_abandonButton.left, m_abandonButton.top, m_abandonButton.right, m_abandonButton.bottom);
	Rectangle(getMemDC(), m_exitButton.left, m_exitButton.top, m_exitButton.right, m_exitButton.bottom);
	
	// 선택된 타입별 아이템 출력
	this->showListItemType();
	// 장착 테두리
	this->showEquipSelect();
	// 선택시 출력 부분
	if (isSelectRender)
	{
		IMAGE->findImage("선택테두리")->render(getMemDC(), m_selectRenderX, m_selectRenderY);
		this->showItemInfo();
	}
	// 장비창 렌더
	if (isEquipWeapon)
	{
		IMAGE->findImage((*m_equipWeapon).getBigImage())->frameRender(getMemDC(), m_vEquipItemMainRect[0].left - 10, m_vEquipItemMainRect[0].top + 10, (*m_equipWeapon).getFrame().x, (*m_equipWeapon).getFrame().y);
		IMAGE->findImage((*m_equipWeapon).getBigImage())->frameRender(getMemDC(), m_vEquipItemMainRect[2].left - 10, m_vEquipItemMainRect[2].top + 10, (*m_equipWeapon).getFrame().x, (*m_equipWeapon).getFrame().y);
	}
	if (isEquipArmor)
	{
		IMAGE->findImage((*m_equipArmor).getBigImage())->frameRender(getMemDC(), m_vEquipItemMainRect[1].left - 10, m_vEquipItemMainRect[1].top + 10 ,(*m_equipArmor).getFrame().x, (*m_equipArmor).getFrame().y);
	}
}

// 아직 예외처리 완성 안됨.. 스크롤 끝까지 내렸을때 예외처리 필요
// 스크롤 기능이 있을때, 인벤토리 리스트에 보여줄 4개 인덱스 범위
// 나머지 타입 추가 필요
void CinventoryUi::setShowIndex()
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
	case ITEMTYPE::ITEMTYPE_SHOES:
		if (m_myInventory->getvShoesList()->size() < 4)
			m_showEndIndex = m_myInventory->getvShoesList()->size() + m_showIndex;
		else
			m_showEndIndex = m_showIndex + 4;
		break;
	case ITEMTYPE::ITEMTYPE_GLOVES:
		if (m_myInventory->getvGlovesList()->size() < 4)
			m_showEndIndex = m_myInventory->getvGlovesList()->size() + m_showIndex;
		else
			m_showEndIndex = m_showIndex + 4;
		break;
	case ITEMTYPE::ITEMTYPE_PENDANT:
		if (m_myInventory->getvPendantList()->size() < 4)
			m_showEndIndex = m_myInventory->getvPendantList()->size() + m_showIndex;
		else
			m_showEndIndex = m_showIndex + 4;
		break;
	default:
		break;
	}
}

// 선택한 타입의 아이템 타입만 인벤토리 리스트에 보여짐
// 나머지 타입 추가 필요
void CinventoryUi::showListItemType()
{
	switch (m_selectType)
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		showItemList(m_myInventory->getvWeaponList());
		
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		showItemList(m_myInventory->getvArmorList());
		break;
	case ITEMTYPE::ITEMTYPE_GLOVES:
		showItemList(m_myInventory->getvGlovesList());

		break;
	case ITEMTYPE::ITEMTYPE_SHOES:
		showItemList(m_myInventory->getvShoesList());

		break;
	case ITEMTYPE::ITEMTYPE_PENDANT:
		showItemList(m_myInventory->getvPendantList());

		break;
	default:
		break;
	}
}
// 인덱스 기준으로 벡터를 참조하여
void CinventoryUi::showItemList(vector<Citem>* list)
{

	char str[100];
	// show 인덱스는 기본 0~3까지만 보여준다, 이후 스크롤로 1~4 이런식으로 보여주기
	int	i = m_showIndex;
	int j = 0;
		for (auto iter = list->begin() + i; iter != list->end() && i < m_showEndIndex; ++iter, i++, j++)
		{
			IMAGE->findImage((*iter).getSmallImage())->frameRender(getMemDC()
				,m_vItemListRect[j].left, m_vItemListRect[j].top, (*iter).getFrame().x, (*iter).getFrame().y);

			SetTextColor(getMemDC(), RGB(255, 255, 255));
			TextOut(getMemDC(), m_vItemListRect[j].left + 80, m_vItemListRect[j].top + 10, (*iter).getName().c_str(), lstrlen((*iter).getName().c_str()));
			wsprintf(str, "필요 레벨: %d", (*iter).getLimitLevel());
			TextOut(getMemDC(), m_vItemListRect[j].left + 80, m_vItemListRect[j].top + 30, str, lstrlen(str));
			wsprintf(str, "골드: %d", (*iter).getBuyPrice());
			TextOut(getMemDC(), m_vItemListRect[j].left + 80, m_vItemListRect[j].top + 50, str, lstrlen(str));
		}
}

// 나머지 타입 추가 필요
void CinventoryUi::showEquipSelect()
{
	switch (m_selectType)
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
	case ITEMTYPE::ITEMTYPE_GLOVES:
		if (isEquipWeapon)
			IMAGE->findImage("장착테두리")->render(getMemDC(), m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_GLOVES)].x,
				m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_GLOVES)].y);
		break;
	case ITEMTYPE::ITEMTYPE_SHOES:
		if (isEquipWeapon)
			IMAGE->findImage("장착테두리")->render(getMemDC(), m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_SHOES)].x,
				m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_SHOES)].y);
		break;
	case ITEMTYPE::ITEMTYPE_PENDANT:
		if (isEquipWeapon)
			IMAGE->findImage("장착테두리")->render(getMemDC(), m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_PENDANT)].x,
				m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_PENDANT)].y);
		break;
		break;
	}

}

// 마우스 1클릭으로 아이템 선택표시
void CinventoryUi::selectItem()
{
	vector<Citem>::iterator itemIter;

	int i = m_showIndex;
	for (auto iter = m_vItemListRect.begin(); iter != m_vItemListRect.end() && i < m_showEndIndex; ++iter, i++)
	{
		if (PtInRect(&(*iter), m_ptMouse))
		{
			if (InputManager->isOnceKeyDown(VK_LBUTTON))
			{
				isSelectRender = true;
				m_selectRenderX = (*iter).left - 5;
				m_selectRenderY = (*iter).top - 3;
				
				// 선택한 아이템 정보 inventory 에서 가져와서 selectItem 변수에 담기
				// 나머지 타입 추가 필요
				switch (m_selectType)
				{
				case ITEMTYPE::ITEMTYPE_WEAPON:
					itemIter = m_myInventory->getvWeaponList()->begin() + i;
					m_selectItem = &(*itemIter);
					break;
				case ITEMTYPE::ITEMTYPE_ARMOR:
					itemIter = m_myInventory->getvArmorList()->begin() + i;
					m_selectItem = &(*itemIter);
					break;
				case ITEMTYPE::ITEMTYPE_GLOVES:
					itemIter = m_myInventory->getvGlovesList()->begin() + i;
					m_selectItem = &(*itemIter);
					break;
				case ITEMTYPE::ITEMTYPE_SHOES:
					itemIter = m_myInventory->getvShoesList()->begin() + i;
					m_selectItem = &(*itemIter);
					break;
				case ITEMTYPE::ITEMTYPE_PENDANT:
					itemIter = m_myInventory->getvPendantList()->begin() + i;
					m_selectItem = &(*itemIter);
					break;
				default:
					break;
				}
			}
		}
	}
}
// 마우스 2클릭으로 아이템 장착표시 / 장착
// 일단 같은렉트 클릭조건 예외처리는 안함 -> 추후에 추가필요
void CinventoryUi::selectEquipItem()
{
	int i = m_showIndex;
	for (auto iter = m_vItemListRect.begin(); iter != m_vItemListRect.end() && i < m_showEndIndex; ++iter, i++)
	{
		if (PtInRect(&(*iter), m_ptMouse))
		{
			// 같은 렉트 클릭하고있는지 체크
			if (m_clickCheckRect.top != (*iter).top)
				m_clickCount = 0;
			m_clickCheckRect = (*iter);
			
			if (InputManager->isStayKeyDown(VK_LBUTTON))
			{
				isButtonClick = true;
				if (isButtonClick)
				{
					if (isKeyUp)
					{
						if (m_clickCount == 0)
							m_compareTime = TIME->getWorldTime();
						m_clickCount++;

						// 마우스 더블클릭 되는 시점!
						// 아이템 장착에 관한 모든 것 세팅
						if (m_clickCount > 1)
						{
							m_equipRenderX = (*iter).left - 5;
							m_equipRenderY = (*iter).top - 3;
							this->setEquipItem(i, m_equipRenderX, m_equipRenderY);
						}
						isKeyUp = false;
					}
				}
			}
			else
			{
				isButtonClick = false;
				isKeyUp = true;
			}
		}
	}
	if (m_compareTime + 1 < TIME->getWorldTime())
		m_clickCount = 0;
}
// 아이템 장착!
// 아이템 장착시 필요한 변수에 장착할 아이템 정보 넣어주기
// 나머지 타입 추가 필요
void CinventoryUi::setEquipItem(int index, int x, int y)
{
	Citem* weaponTemp;
	Citem* armorTemp;
	
	this->setPlayerStat(index);

	// 장착 아이템변수에 담기
	switch (m_selectType)
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		// 이터레이터 접근해서 받은 원소는 해당 원소 포인터 변수에 직접 못넣어서 다시 변환해줌
		m_myInventory->setEquipWeapon(&(*(m_myInventory->getvWeaponList()->begin() + index)));
		weaponTemp = m_myInventory->getEquipWeapon();
		// 이유는 모르겠지만 직접 인벤토리 리스트 정보 전달하면 나중에 인벤토리에서 다른거 지워질때 영향받음
		// 임시 아이템 배열에 복사해서 그걸 UI 장착변수에 담음
		m_itemInit[static_cast<int>(ITEMTYPE::ITEMTYPE_WEAPON)] = (*weaponTemp);
		m_equipWeapon = &m_itemInit[static_cast<int>(ITEMTYPE::ITEMTYPE_WEAPON)];

		m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_WEAPON)] = PointMake(x, y);
		isEquipWeapon = true;
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		m_myInventory->setEquipArmor(&(*(m_myInventory->getvArmorList()->begin() + index)));
		armorTemp = m_myInventory->getEquipArmor();
		m_itemInit[static_cast<int>(ITEMTYPE::ITEMTYPE_ARMOR)] = (*armorTemp);
		m_equipArmor = &m_itemInit[static_cast<int>(ITEMTYPE::ITEMTYPE_ARMOR)];

		m_equipRenderPoint[static_cast<int>(ITEMTYPE::ITEMTYPE_ARMOR)] = PointMake(x, y);
		isEquipArmor = true;
		break;
	default:
		break;
	}
}

// 나머지 타입 추가 필요
void CinventoryUi::setPlayerStat(int index)
{
	Citem* Temp;
	switch (m_selectType)
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		Temp = &(*(m_myInventory->getvWeaponList()->begin() + index));
		if (m_myInventory->getEquipWeapon() == nullptr)
		{
			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		else
		{	// 이미 장착중 아이템 능력치 해제
			m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipWeapon()->getAtk());
			m_player->setDef(m_player->getDef() - m_myInventory->getEquipWeapon()->getDef());
			m_player->setHp(m_player->getHp() - m_myInventory->getEquipWeapon()->getHp());
			m_player->setMp(m_player->getMp() - m_myInventory->getEquipWeapon()->getMp());
			m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipWeapon()->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipWeapon()->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipWeapon()->getSpeed());

			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		Temp = &(*(m_myInventory->getvArmorList()->begin() + index));
		if (m_myInventory->getEquipArmor() == nullptr)
		{
			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		else
		{	// 이미 장착중 아이템 능력치 해제
			m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipArmor()->getAtk());
			m_player->setDef(m_player->getDef() - m_myInventory->getEquipArmor()->getDef());
			m_player->setHp(m_player->getHp() - m_myInventory->getEquipArmor()->getHp());
			m_player->setMp(m_player->getMp() - m_myInventory->getEquipArmor()->getMp());
			m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipArmor()->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipArmor()->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipArmor()->getSpeed());

			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		break;
	case ITEMTYPE::ITEMTYPE_SHOES:
		Temp = &(*(m_myInventory->getvShoesList()->begin() + index));
		if (m_myInventory->getEquipShoes() == nullptr)
		{
			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		else
		{	// 이미 장착중 아이템 능력치 해제
			m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipShoes()->getAtk());
			m_player->setDef(m_player->getDef() - m_myInventory->getEquipShoes()->getDef());
			m_player->setHp(m_player->getHp() - m_myInventory->getEquipShoes()->getHp());
			m_player->setMp(m_player->getMp() - m_myInventory->getEquipShoes()->getMp());
			m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipShoes()->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipShoes()->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipShoes()->getSpeed());

			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		break;
	case ITEMTYPE::ITEMTYPE_GLOVES:
		Temp = &(*(m_myInventory->getvShoesList()->begin() + index));
		if (m_myInventory->getEquipShoes() == nullptr)
		{
			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		else
		{	// 이미 장착중 아이템 능력치 해제
			m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipGloves()->getAtk());
			m_player->setDef(m_player->getDef() - m_myInventory->getEquipGloves()->getDef());
			m_player->setHp(m_player->getHp() - m_myInventory->getEquipGloves()->getHp());
			m_player->setMp(m_player->getMp() - m_myInventory->getEquipGloves()->getMp());
			m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipGloves()->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipGloves()->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipGloves()->getSpeed());

			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		break;

	case ITEMTYPE::ITEMTYPE_PENDANT:
		Temp = &(*(m_myInventory->getvPendantList()->begin() + index));
		if (m_myInventory->getEquipPendant() == nullptr)
		{
			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		else
		{	// 이미 장착중 아이템 능력치 해제
			m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipPendant()->getAtk());
			m_player->setDef(m_player->getDef() - m_myInventory->getEquipPendant()->getDef());
			m_player->setHp(m_player->getHp() - m_myInventory->getEquipPendant()->getHp());
			m_player->setMp(m_player->getMp() - m_myInventory->getEquipPendant()->getMp());
			m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipPendant()->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipPendant()->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipPendant()->getSpeed());

			m_player->setAtk(m_player->getAtk() + Temp->getAtk());
			m_player->setDef(m_player->getDef() + Temp->getDef());
			m_player->setHp(m_player->getHp() + Temp->getHp());
			m_player->setMp(m_player->getMp() + Temp->getMp());
			m_player->setCritical(m_player->getCritical() + Temp->getCritical());
			m_player->setCriticalAtk(m_player->getCriticalAtk() + Temp->getCriticalAtk());
			m_player->setSpeed(m_player->getSpeed() + Temp->getSpeed());
		}
		break;
	default:
		break;
	}
}

// 장비창 아이템칸 클릭시 작동되는 것들
void CinventoryUi::selectItemType()
{
	for (auto iter = m_vEquipItemMainRect.begin(); iter != m_vEquipItemMainRect.end(); ++iter)
	{
		if (PtInRect(&(*iter), m_ptMouse))
		{
			// 같은 렉트 클릭하고있는지 체크
			if (m_clickCheckRect.left != (*iter).left)
				m_clickCount = 0;
			m_clickCheckRect = (*iter);
			if (isButtonClick)
			{
				if (isKeyUp)
				{
					if (m_clickCount == 0)
						m_compareTime = TIME->getWorldTime();
					m_clickCount++;

					// 야매로 비교.. 가운데 장비창인지 확인 else 는 양옆 장비창
					if ((*iter).left == m_vEquipItemMainRect[1].left)
					{
						m_selectType = ITEMTYPE::ITEMTYPE_ARMOR;
					}
					else
						m_selectType = ITEMTYPE::ITEMTYPE_WEAPON;

					// 마우스 더블클릭 되는 시점!
					// 장착 해제
					// 장착 해제시 장착 테두리도 타입별로 만들어 줘야함.. 일단 보류
					if (m_clickCount > 1)
					{
						if ((*iter).left == m_vEquipItemMainRect[1].left)
						{
							if(m_equipArmor != nullptr)
								this->unEquipItem();
							isEquipArmor = false;
							m_equipArmor = nullptr;
						}
						else
						{
							if(m_equipWeapon != nullptr)
								this->unEquipItem();
							isEquipWeapon = false;
							m_equipWeapon = nullptr;
						}
					}
					isKeyUp = false;
				}
			}
		}
	}
	if (m_compareTime + 1 < TIME->getWorldTime())
		m_clickCount = 0;
}

void CinventoryUi::showItemInfo()
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

	this->showItemCompare();

}

void CinventoryUi::showItemCompare()
{
	char atk[100];
	char def[100];
	char hp[100];
	char mp[100];
	char cri[100];
	char criAtk[100];
	char speed[100];

	int temp_atk, temp_def, temp_hp, temp_mp, temp_cri;
	float temp_criAtk, temp_speed;

	switch (m_selectItem->getType())
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		if (m_myInventory->getEquipWeapon() == nullptr)
		{
			temp_atk = temp_def = temp_hp = temp_mp = temp_cri = 0;
			temp_criAtk = temp_speed = 0;
		}
		else
		{
			temp_atk = m_myInventory->getEquipWeapon()->getAtk();
			temp_def = m_myInventory->getEquipWeapon()->getDef();
			temp_hp = m_myInventory->getEquipWeapon()->getHp();
			temp_mp = m_myInventory->getEquipWeapon()->getMp();
			temp_cri = m_myInventory->getEquipWeapon()->getCritical();
			temp_criAtk = m_myInventory->getEquipWeapon()->getCriticalAtk();
			temp_speed = m_myInventory->getEquipWeapon()->getSpeed();
		}

		if (m_selectItem->getAtk() - temp_atk > 0)
			wsprintf(atk, "+%d", (m_selectItem->getAtk() - temp_atk));
		else
			wsprintf(atk, "%d", (m_selectItem->getAtk() - temp_atk));
		if (m_selectItem->getDef() - temp_def > 0)
			wsprintf(def, "+%d", (m_selectItem->getDef() - temp_def));
		else
			wsprintf(def, "%d", (m_selectItem->getDef() - temp_def));
		if (m_selectItem->getHp() - temp_hp > 0)
			wsprintf(hp, "+%d", (m_selectItem->getHp() - temp_hp));
		else
			wsprintf(hp, "%d", (m_selectItem->getHp() - temp_hp));
		if (m_selectItem->getMp() - temp_mp > 0)
			wsprintf(mp, "+%d", (m_selectItem->getMp() - temp_mp));
		else
			wsprintf(mp, "%d", (m_selectItem->getMp() - temp_mp));
		if (m_selectItem->getCritical() - temp_cri > 0)
			wsprintf(cri, "+%d", (m_selectItem->getCritical() - temp_cri));
		else
			wsprintf(cri, "%d", (m_selectItem->getCritical() - temp_cri));
		if (m_selectItem->getCriticalAtk() - temp_criAtk > 0)
			sprintf(criAtk, "+%.1f", m_selectItem->getCriticalAtk() - temp_criAtk);
		else
			sprintf(criAtk, "%.1f", m_selectItem->getCriticalAtk() - temp_criAtk);
		if (m_selectItem->getSpeed() - temp_speed > 0)
			sprintf(speed, "+%.1f", m_selectItem->getSpeed() - temp_speed);
		else
			sprintf(speed, "%.1f", m_selectItem->getSpeed() - temp_speed);

		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 100, atk, lstrlen(atk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 120, def, lstrlen(def));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 140, hp, lstrlen(hp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 160, mp, lstrlen(mp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 180, cri, lstrlen(cri));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 200, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 220, speed, lstrlen(speed));

		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		if (m_myInventory->getEquipArmor() == nullptr)
		{
			temp_atk = temp_def = temp_hp = temp_mp = temp_cri = 0;
			temp_criAtk = temp_speed = 0;
		}
		else
		{
			temp_atk = m_myInventory->getEquipArmor()->getAtk();
			temp_def = m_myInventory->getEquipArmor()->getDef();
			temp_hp = m_myInventory->getEquipArmor()->getHp();
			temp_mp = m_myInventory->getEquipArmor()->getMp();
			temp_cri = m_myInventory->getEquipArmor()->getCritical();
			temp_criAtk = m_myInventory->getEquipArmor()->getCriticalAtk();
			temp_speed = m_myInventory->getEquipArmor()->getSpeed();
		}

		if (m_selectItem->getAtk() - temp_atk > 0)
			wsprintf(atk, "+%d", (m_selectItem->getAtk() - temp_atk));
		else
			wsprintf(atk, "%d", (m_selectItem->getAtk() - temp_atk));
		if (m_selectItem->getDef() - temp_def > 0)
			wsprintf(def, "+%d", (m_selectItem->getDef() - temp_def));
		else
			wsprintf(def, "%d", (m_selectItem->getDef() - temp_def));
		if (m_selectItem->getHp() - temp_hp > 0)
			wsprintf(hp, "+%d", (m_selectItem->getHp() - temp_hp));
		else
			wsprintf(hp, "%d", (m_selectItem->getHp() - temp_hp));
		if (m_selectItem->getMp() - temp_mp > 0)
			wsprintf(mp, "+%d", (m_selectItem->getMp() - temp_mp));
		else
			wsprintf(mp, "%d", (m_selectItem->getMp() - temp_mp));
		if (m_selectItem->getCritical() - temp_cri > 0)
			wsprintf(cri, "+%d", (m_selectItem->getCritical() - temp_cri));
		else
			wsprintf(cri, "%d", (m_selectItem->getCritical() - temp_cri));
		if (m_selectItem->getCriticalAtk() - temp_criAtk > 0)
			sprintf(criAtk, "+%.1f", m_selectItem->getCriticalAtk() - temp_criAtk);
		else
			sprintf(criAtk, "%.1f", m_selectItem->getCriticalAtk() - temp_criAtk);
		if (m_selectItem->getSpeed() - temp_speed > 0)
			sprintf(speed, "+%.1f", m_selectItem->getSpeed() - temp_speed);
		else
			sprintf(speed, "%.1f", m_selectItem->getSpeed() - temp_speed);

		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 100, atk, lstrlen(atk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 120, def, lstrlen(def));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 140, hp, lstrlen(hp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 160, mp, lstrlen(mp));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 180, cri, lstrlen(cri));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 200, criAtk, lstrlen(criAtk));
		TextOut(getMemDC(), m_ItemInfoRect.left + 140, m_ItemInfoRect.top + 220, speed, lstrlen(speed));
		break;
	/*
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
	*/
	default:
		break;
	}
}

void CinventoryUi::abandonItem()
{
	if (PtInRect(&m_abandonButton, m_ptMouse))
	{

		if (isButtonClick)
		{
			if (isKeyUp)
			{
				// 선택 아이템이 있을때만 실행
				// 장착중이지 않은 아이템만 실행
				if (m_selectItem != nullptr && this->checkEquipItem())
				{
					m_myInventory->AbandonItem(m_selectItem);
					m_selectItem = nullptr;
					isSelectRender = false;
				}
				isKeyUp = false;
			}
		}
	}
}

// Id로 비교하도록 수정
bool CinventoryUi::checkEquipItem()
{
	if (m_equipWeapon != nullptr)
	{
		if (m_equipWeapon->getItemId() == m_selectItem->getItemId())
			return false;
	}
	if (m_equipArmor != nullptr)
	{
		if (m_equipArmor->getItemId() == m_selectItem->getItemId())
			return false;
	}
	return true;
}
// 나머지 타입 추가 필요
void CinventoryUi::unEquipItem()
{

	switch (m_selectType)
	{
	case ITEMTYPE::ITEMTYPE_WEAPON:
		m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipWeapon()->getAtk());
		m_player->setDef(m_player->getDef() - m_myInventory->getEquipWeapon()->getDef());
		m_player->setHp(m_player->getHp() - m_myInventory->getEquipWeapon()->getHp());
		m_player->setMp(m_player->getMp() - m_myInventory->getEquipWeapon()->getMp());
		m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipWeapon()->getCritical());
		m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipWeapon()->getCriticalAtk());
		m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipWeapon()->getSpeed());

		m_myInventory->setEquipWeapon(nullptr);
		break;
	case ITEMTYPE::ITEMTYPE_ARMOR:
		m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipArmor()->getAtk());
		m_player->setDef(m_player->getDef() - m_myInventory->getEquipArmor()->getDef());
		m_player->setHp(m_player->getHp() - m_myInventory->getEquipArmor()->getHp());
		m_player->setMp(m_player->getMp() - m_myInventory->getEquipArmor()->getMp());
		m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipArmor()->getCritical());
		m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipArmor()->getCriticalAtk());
		m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipArmor()->getSpeed());

		m_myInventory->setEquipArmor(nullptr);
		break;
	case ITEMTYPE::ITEMTYPE_SHOES:
		m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipShoes()->getAtk());
		m_player->setDef(m_player->getDef() - m_myInventory->getEquipShoes()->getDef());
		m_player->setHp(m_player->getHp() - m_myInventory->getEquipShoes()->getHp());
		m_player->setMp(m_player->getMp() - m_myInventory->getEquipShoes()->getMp());
		m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipShoes()->getCritical());
		m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipShoes()->getCriticalAtk());
		m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipShoes()->getSpeed());

		m_myInventory->setEquipShoes(nullptr);

		break;
	case ITEMTYPE::ITEMTYPE_GLOVES:
		m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipGloves()->getAtk());
		m_player->setDef(m_player->getDef() - m_myInventory->getEquipGloves()->getDef());
		m_player->setHp(m_player->getHp() - m_myInventory->getEquipGloves()->getHp());
		m_player->setMp(m_player->getMp() - m_myInventory->getEquipGloves()->getMp());
		m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipGloves()->getCritical());
		m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipGloves()->getCriticalAtk());
		m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipGloves()->getSpeed());

		m_myInventory->setEquipGloves(nullptr);

		break;
	case ITEMTYPE::ITEMTYPE_PENDANT:
		m_player->setAtk(m_player->getAtk() - m_myInventory->getEquipPendant()->getAtk());
		m_player->setDef(m_player->getDef() - m_myInventory->getEquipPendant()->getDef());
		m_player->setHp(m_player->getHp() - m_myInventory->getEquipPendant()->getHp());
		m_player->setMp(m_player->getMp() - m_myInventory->getEquipPendant()->getMp());
		m_player->setCritical(m_player->getCritical() - m_myInventory->getEquipPendant()->getCritical());
		m_player->setCriticalAtk(m_player->getCriticalAtk() - m_myInventory->getEquipPendant()->getCriticalAtk());
		m_player->setSpeed(m_player->getSpeed() - m_myInventory->getEquipPendant()->getSpeed());

		m_myInventory->setEquipPendant(nullptr);

		break;
	default:
		break;
	}
}
