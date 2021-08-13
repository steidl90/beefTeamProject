#include "framework.h"
#include "CsceneTown.h"

CsceneTown::CsceneTown() :m_timerHp(NULL), m_timerMp(NULL)
{
}

CsceneTown::~CsceneTown()
{
}

HRESULT CsceneTown::init()
{
	m_camera = new camera;
	m_camera->init();

	m_town = new Ctown;
	m_town->setCameraMemory(m_camera);
	m_town->init();

	m_player = new CplayerManager;
	m_player->init();
	m_player->setCheckTile(m_town->getMap());
	m_player->setTileAttribute(m_town->getAttribute());

	m_player->getPlayer()->setAtk(DATA->getAtk());
	m_player->getPlayer()->setCritical(DATA->getCritical());
	m_player->getPlayer()->setCriticalAtk(DATA->getCriticalAtk());
	m_player->getPlayer()->setDef(DATA->getDef());
	m_player->getPlayer()->setExp(DATA->getExp());
	m_player->getPlayer()->setGold(DATA->getGold());
	m_player->getPlayer()->setHp(DATA->getHp());
	m_player->getPlayer()->setLv(DATA->getLv());
	m_player->getPlayer()->setMp(DATA->getMp());
	m_player->getPlayer()->setSpeed(DATA->getSpeed());
	m_player->getPlayer()->setPlayerX(DATA->getX());
	m_player->getPlayer()->setPlayerY(DATA->getY());

	m_player->getInventoryMemory()->setEquipWeapon(DATA->getWeaponEquip());
	m_player->getInventoryMemory()->setEquipArmor(DATA->getArmorEquip());
	m_player->getInventoryMemory()->setEquipShoes(DATA->getShoesEquip());
	m_player->getInventoryMemory()->setEquipGloves(DATA->getGlovesEquip());
	m_player->getInventoryMemory()->setEquipPendant(DATA->getPendantEquip());

	m_aStar = new CaStar;
	m_aStar->setCameraMemory(m_camera);
	m_aStar->init();

	m_shop = new Cshop;
	m_shop->setInventoryMemory(m_player->getInventoryMemory());
	m_shop->setPlayerMemory(m_player->getPlayer());
	m_shop->init();
	

	m_shopUi = new CshopUi;
	//m_shopUi->setInventoryMemory(m_player->getInventoryMemory());
	//m_shopUi->setPlayerMemory(m_player->getPlayer());
	m_shopUi->setShopMemory(m_shop);
	m_shopUi->init();

	m_changeRect = RectMake(MAPSIZE - 10, MAPSIZE * 0.25 + 120, 100, 100);

	m_shopRect = RectMake(300, 300, 100, 100);

	isShopOn = false;

	return S_OK;
}

void CsceneTown::release()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_town);
	SAFE_DELETE(m_player);
	SAFE_DELETE(m_aStar);
	SAFE_DELETE(m_shop);
	SAFE_DELETE(m_shopUi);
}

void CsceneTown::update()
{
	m_aStar->update();
	m_aStar->setPlayerIndex(PointMake(m_player->getplayerRect()->left / TILESIZE, m_player->getplayerRect()->top / TILESIZE));
	m_camera->update();
	m_camera->setTargetPoint(PointMake(m_player->getplayerRect()->left, m_player->getplayerRect()->top));
	m_town->update();
	m_town->setFastLoadIndex(m_aStar->getFastLoad());
	m_player->update();
	m_player->setFastLoadLocation(m_aStar->getFastLoadLocation());

	if (isShopOn) m_shopUi->update();

	shopOn();

	//ü�� ����
	if (m_player->getPlayer()->getHp() < m_player->getPlayer()->getMaxHp())
	{
		if (m_timerHp + 3 < TIME->getWorldTime())
		{
			m_player->getPlayer()->setHp(m_player->getPlayer()->getHp() + 10);
			m_timerHp = TIME->getWorldTime();
		}
		if (m_player->getPlayer()->getHp() > m_player->getPlayer()->getMaxHp())
		{
			m_player->getPlayer()->setHp(m_player->getPlayer()->getMaxHp());
		}
	}

	//���� ����
	if (m_player->getPlayer()->getMp() < m_player->getPlayer()->getMaxMp())
	{
		if (m_timerMp + 3 < TIME->getWorldTime())
		{
			m_player->getPlayer()->setMp(m_player->getPlayer()->getMp() + 5);
			m_timerMp = TIME->getWorldTime();
		}
		if (m_player->getPlayer()->getMp() > m_player->getPlayer()->getMaxMp())
		{
			m_player->getPlayer()->setMp(m_player->getPlayer()->getMaxMp());
		}
	}

	sceneChange();
}

void CsceneTown::render()
{
	this->getMapBuffer()->render(getMemDC(), 0, 0, m_camera->getCameraPoint().x, m_camera->getCameraPoint().y, m_camera->getCameraWidth(), m_camera->getCameraHeight());
	this->getMapBuffer()->mapRender(getMemDC(), WINSIZEX - IMAGE->findImage("MiniMapUi")->getWidth(), 70);

	m_camera->render();
	m_town->render();
	m_player->render();

	if (isShopOn) m_shopUi->render();

	Rectangle(getMapDC(), m_shopRect.left, m_shopRect.top, m_shopRect.right, m_shopRect.bottom);
}

void CsceneTown::sceneChange()
{
	RECT temp;
	if (IntersectRect(&temp, m_player->getplayerRect(), &m_changeRect))
	{
		DATA->setData(m_player->getPlayer()->getAtk(),
			m_player->getPlayer()->getDef(),
			m_player->getPlayer()->getHp(),
			m_player->getPlayer()->getMp(),
			m_player->getPlayer()->getCritical(),
			m_player->getPlayer()->getLv(),
			m_player->getPlayer()->getExp(),
			m_player->getPlayer()->getGold(),
			m_player->getPlayer()->getCriticalAtk(),
			m_player->getPlayer()->getSpeed(),
			m_player->getPlayer()->getPlayerX(),
			m_player->getPlayer()->getPlayerY());
		DATA->setWeaponEquip(m_player->getInventoryMemory()->getEquipWeapon());
		DATA->setArmorEquip(m_player->getInventoryMemory()->getEquipArmor());
		DATA->setShoesEquip(m_player->getInventoryMemory()->getEquipShoes());
		DATA->setGlovesEquip(m_player->getInventoryMemory()->getEquipGloves());
		DATA->setPendantEquip(m_player->getInventoryMemory()->getEquipPendant());

		SCENE->changeScene("����");
	}
}

void CsceneTown::shopOn()
{
	RECT temp;
	if (IntersectRect(&temp, m_player->getplayerRect(), &m_shopRect))
	{
		if (InputManager->isOnceKeyDown('P'))
		{
			isShopOn = !isShopOn;
		}
	}
	else
	{
		isShopOn = false;
	}
	//isShopOn = true;
}