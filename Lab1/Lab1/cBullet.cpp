#include "cBullet.h"

cBullet::cBullet() : cModel()
{

}


void cBullet::update(float elapsedTime)
{

	cModel::setDirection(glm::vec3(0.0f, 0.0f, 1.0f));
	cModel::m_mdlRotation += elapsedTime * 2 * 3.14f;
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
}

cBullet::~cBullet()
{

}