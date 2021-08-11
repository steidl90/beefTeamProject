#include "framework.h"
#include "CsceneTown.h"

CsceneTown::CsceneTown()
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
	m_town->init();
	m_town->setCameraMemory(m_camera);

	m_player = new CplayerManager;
	m_player->init();

	m_aStar = new CaStar;
	m_aStar->init();
	m_aStar->setCameraMemory(m_camera);

	m_changeRect = RectMake(100, 100, 100, 100);

	return S_OK;
}

void CsceneTown::release()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_town);
	SAFE_DELETE(m_player);
	SAFE_DELETE(m_aStar);
}

void CsceneTown::update()
{
	m_camera->update();
	m_town->update();
	m_player->update();
	m_camera->setTargetPoint(PointMake(m_player->getplayerRect()->left, m_player->getplayerRect()->top));

	// 에이스타 추가
	m_aStar->setPlayerIndex(PointMake(m_player->getplayerRect()->left / TILESIZE, m_player->getplayerRect()->top / TILESIZE));
	m_aStar->update();
	m_town->setFastLoadIndex(m_aStar->getFastLoad());
	m_player->setFastLoadLocation(m_aStar->getFastLoadLocation());

	sceneChange();
}

void CsceneTown::render()
{
	this->getMapBuffer()->render(getMemDC(), 0, 0, m_camera->getCameraPoint().x, m_camera->getCameraPoint().y, m_camera->getCameraWidth(), m_camera->getCameraHeight());
	this->getMapBuffer()->mapRender(getMemDC(), WINSIZEX - IMAGE->findImage("MiniMapUi")->getWidth(), 70);

	m_camera->render();
	m_town->render();
	m_player->render();

	Rectangle(getMapDC(), m_changeRect.left, m_changeRect.top, m_changeRect.right, m_changeRect.bottom);
}

void CsceneTown::sceneChange()
{
	RECT temp;

	if (IntersectRect(&temp, m_player->getplayerRect(), &m_changeRect))
	{
		SCENE->changeScene("던전");
	}
}
