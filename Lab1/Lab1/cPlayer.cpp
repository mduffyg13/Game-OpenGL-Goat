#include "cPlayer.h"

cPlayer::cPlayer() : cModel(){

}
void cPlayer::update(float elapsedTime, float newXPos){
	
	if (g_PlayerMoving){

		float currentX = cModel::m_mdlPosition.x;

		//Set Direction
		if (dir_switch == 0){
		if (currentX < newXPos){
			cModel::setDirection(DIR_RIGHT);
			dir_switch++;
		}
		else{
			cModel::setDirection(DIR_LEFT);
			dir_switch++;
		}
	}

		if (cModel::getDirection() == DIR_RIGHT &&
			cModel::m_mdlPosition.x <= newXPos){
			cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
			
			
		}
		else if (cModel::getDirection() == DIR_LEFT &&
			cModel::m_mdlPosition.x >= newXPos){
			cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
			
		}
		else{ 
			cModel::m_mdlPosition.x = newXPos;
			g_PlayerMoving = false;
			dir_switch = 0;
		}
	}
}
void cPlayer::update(float elapsedTime){

}
cPlayer::~cPlayer(){

}