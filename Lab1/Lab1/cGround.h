#ifndef _GROUND_H
#define _GROUND_H

#include "GameConstants.h"
#include "cModel.h"

class cGround : public cModel{
public:
	cGround();
	virtual void update(float elapsedTime);
	~cGround();
private:


};

#endif