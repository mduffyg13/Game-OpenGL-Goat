
#ifndef _BULLET_H
#define _BULLET_H

#include "GameConstants.h"
#include "cModel.h"

class cBullet : public cModel{
public:
	cBullet();

	virtual void update(float elapsedTime);

	~cBullet();
private:
	
};

#endif