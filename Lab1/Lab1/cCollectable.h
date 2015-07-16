
#ifndef _COLLECTABLE_H
#define _COLLECTABLE_H

#include "GameConstants.h"
#include "cModel.h"

class cCollectable : public cModel{
public:
	cCollectable();

	void randomise();
	virtual void update(float elapsedTime);

	~cCollectable();
private:
	const float m_EnemyMinSpeed = 3.0f;
	const float m_EnemyMaxSpeed = 10.0f;
	const float m_EnemySpeedAdjustment = 2.5f;
};

#endif