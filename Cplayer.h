#pragma once
#include "Cunit.h"

enum class  DIRECTIONS
{
	DIRECTIONS_LEFT,
	DIRECTIONS_UP,
	DIRECTIONS_RIGHT,
	DIRECTIONS_DOWN
};

//enum class STATE
//{
//	STATE_IDLE,
//	STATE_LEFT,
//	STATE_RIGHT,
//	STATE_UP,
//	STATE_DOWN
//};

class Cplayer :public Cunit
{
private:
	DIRECTIONS direction;
	DIRECTIONS beforeDirection;

	//대기
	image* playerDown;
	image* playerUp;
	image* playerLeft;
	image* playerRight;

	//무브
	image* playerMoveDown;
	image* playerMoveUp;
	image* playerMoveLeft;
	image* playerMoveRight;

	//공격
	image* playerAttackDown;
	image* playerAttackUp;
	image* playerAttackLeft;
	image* playerAttackRight;

	animation* playerIdleAni;
	animation* playerAttackAni;
	animation* playerMoveAni;

	RECT playerAttackRc;
	RECT playerMoveRc;

	float m_playerX, m_playerY;

	float m_speed;

	bool isMoving;
	bool isAttack;
	bool isLive;
	bool isIdle;

public:
	Cplayer();
	~Cplayer();

	HRESULT init();
	void release();
	void update();
	void render();
	void moveControl();
	void moveAnimation();
	void playerStateRender();

	RECT* getplayerMoveRC() { return &playerMoveRc; }
};

