#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{

}
void cEnemy::randomise()
{
	
}

void cEnemy::update(float elapsedTime)
{
	cModel::setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;

}

cEnemy::~cEnemy()
{

}