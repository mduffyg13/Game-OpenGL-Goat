#include "cCollectable.h"

cCollectable::cCollectable() : cModel()
{

}
void cCollectable::randomise()
{

}

void cCollectable::update(float elapsedTime)
{
	cModel::setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;

}

cCollectable::~cCollectable()
{

}