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
			
			//cModel::modelMatrix = glm::mat4(1.0f);
			//cModel::modelMatrix = glm::translate(cModel::modelMatrix, cModel::m_mdlPosition);
		}
		else if (cModel::getDirection() == DIR_LEFT &&
			cModel::m_mdlPosition.x >= newXPos){
			cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
			
			//cModel::modelMatrix = glm::mat4(1.0f);
			//cModel::modelMatrix = glm::translate(cModel::modelMatrix, cModel::m_mdlPosition);
		}
		else{ 
			cModel::m_mdlPosition.x = newXPos;
			g_PlayerMoving = false;
			dir_switch = 0;
		}

		//cModel::modelMatrix = glm::translate(cModel::modelMatrix, cModel::m_mdlPosition);

	}
}
void cPlayer::update(float elapsedTime){

}
cPlayer::~cPlayer(){

}