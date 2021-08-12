#pragma once
#include "Cunit.h"

struct tagSkill {
	image* m_skillImage;
	RECT m_rc;
	animation* m_ani;

	float m_x, m_y;
	float m_fireX, m_fireY;
	float m_radius;
	float m_angle;
	float m_speed;

	bool m_fire;

	int m_count;
	float m_index;
};

class CenemyAttack :public Cunit
{
private:

	vector<tagSkill> m_vSkill;
	vector<tagSkill>::iterator m_viSkill;

	const char* m_ani;
	float m_range;
	int m_skillMax;

public:
	CenemyAttack();
	~CenemyAttack();

	
	HRESULT init(int skillMax, float range, const char* aniName);

	void release();
	void update();
	void render();

	void fire(float x, float y, float angle, float speed, const char* imageName, const char* aniName);

	void removeSkill(int arrNum);

	vector<tagSkill> getVSkill() { return m_vSkill; }
	vector<tagSkill>::iterator getVISkill() { return m_viSkill; }
};

