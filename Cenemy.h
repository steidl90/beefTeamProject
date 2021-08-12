#pragma once
#include "Cunit.h"

enum class STATE
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	IDLE
};

class Cenemy : public Cunit
{
protected:
	class Cplayer* m_player;

	image* m_walkImage;
	animation* m_walkAni;
	RECT m_walkRc;

	image* m_attackImage;
	animation* m_attackAni;
	RECT m_attackRc;

	image* m_dieImage;
	animation* m_dieAni;
	RECT m_dieRc;

	effect* m_effect;
	STATE m_state;
	RECT m_traceRc;		

	float m_x, m_y;
	float m_returnX, m_returnY;
	float m_speed;
<<<<<<< HEAD
	float m_distance;
=======
>>>>>>> parent of 35abb8d (몬스터 최종)
	float m_hp;
	float m_maxHp;
	float m_damage;
<<<<<<< HEAD
	int m_exp;
=======
	float m_distance;
	
>>>>>>> parent of 35abb8d (몬스터 최종)

	bool m_isIdle;
	bool m_isWalking;
	bool m_isAttack;
	bool m_isDie;

	int m_cooltimeCount;
	int m_rndskillCount;

public:
		class CenemyAttack* m_enemyAttack;
	Cenemy();
	~Cenemy();

	virtual HRESULT init(POINT position,int HP);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void move();
	virtual void attack();

	virtual void die();
	virtual void animation();
	virtual bool enemyCooltime();

<<<<<<< HEAD
	virtual void ReturnIdleAnimation();

	float getHp() { return m_hp; }
	void setHp(float hp) { m_hp = hp; }

	int getExp() { return m_exp; }
	void setExp(int exp) { m_exp = exp; }

=======
>>>>>>> parent of 35abb8d (몬스터 최종)
	virtual void setPlayer(Cplayer* player) { m_player = player; }
	virtual RECT getRect() { return m_walkRc; }
};

