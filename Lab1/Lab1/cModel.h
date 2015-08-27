#ifndef _CMODEL_H
#define _CMODEL_H

#include "GameConstants.h"

class cModel
{
public:
	cModel();
	void setPosition(glm::vec3 mdlPosition);
	void setRotation(GLfloat mdlRotation);
	void setDirection(glm::vec3 mdlDirection);
	void setSpeed(float mdlSpeed);
	void setIsActive(bool mdlIsActive);
	void setMdlDimensions(mdlDimensions mdlDims);
	void setMdlRadius(float mdlRadius);
	float getMdlRadius();


	glm::vec3 getPosition();
	GLfloat getRotation();
	glm::vec3 getDirection();


	bool isActive();
	mdlDimensions getMdlDimensions();
	void setVelocity(glm::vec3 mdlVelocity);
	void increaseMdlSpeed(float speed);
	float getSpeed();
	
	glm::vec3 getVelocity();

	void initialise(glm::vec3 mdlPosition, GLfloat mdlRotation, glm::vec3 mdlVelocity);
	virtual void update(float elapsedTime) = 0;     // will be defined by inherting class
	bool SphereSphereCollision(glm::vec3 mdlPosition, float mdlRadius);
	~cModel();

	//Shaders
	glm::mat4 getModelMatrix();
private:
	float lengthSQRD(glm::vec3 mdlLength);

	
protected:
	// Set the position of the model in world space, and set the rotation.
	glm::vec3 m_mdlPosition; 
	GLfloat m_mdlRotation; 
	glm::vec3 m_mdlScale;
	glm::vec3 m_mdlDirection;
	glm::vec3 m_mdlVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	mdlDimensions m_Dimensions;
	float m_mdlSpeed;
	bool m_IsActive;
	float m_mdlRadius;

	//For shaders
	glm::vec3 transform;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 modelMatrix;
};
#endif
