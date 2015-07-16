#ifndef _PLAYER_H
#define _PLAYER_H

#include "GameConstants.h"
#include "cModel.h"

class cPlayer : public cModel{
public:
	cPlayer();
	virtual void update(float elapsedTime, float newXPos);
	virtual void update(float elapsedTime);
	~cPlayer();
private:


};

#endif