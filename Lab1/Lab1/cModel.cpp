#include "cModel.h"

cModel::cModel()
{
	m_mdlPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_mdlRotation = 0.0f;
	m_mdlVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Dimensions.s_mdldepth = 0.0f;
	m_Dimensions.s_mdlheight = 0.0f;
	m_Dimensions.s_mdlWidth = 0.0f;
	m_mdlRotation = 0.0f;
	transform = glm::vec3(0.0f, 0.0f, 0.0f);
	modelMatrix = glm::mat4(1.0f);   //identity
}
glm::mat4 cModel::getModelMatrix(){
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, m_mdlPosition);
	modelMatrix = glm::rotate(modelMatrix, m_mdlRotation, glm::vec3(1.0,0.0,0.0));

	return modelMatrix; 

}
float cModel::getSpeed(){
	return m_mdlSpeed;
}
glm::vec3 cModel::getPosition()
{
	return m_mdlPosition;
}
void cModel::setPosition(glm::vec3 mdlPosition)
{
	m_mdlPosition = mdlPosition;

	//shader
	modelMatrix = glm::translate(modelMatrix, mdlPosition);
}
GLfloat cModel::getRotation()
{
	return m_mdlRotation;
}
void cModel::setRotation(GLfloat mdlRotation)
{
	m_mdlRotation = mdlRotation;
}
glm::vec3 cModel::getVelocity()
{
	return m_mdlVelocity;
}
void cModel::setVelocity(glm::vec3 mdlVelocity)
{
	m_mdlVelocity = mdlVelocity;
}
mdlDimensions cModel::getMdlDimensions()
{
	return m_Dimensions;
}
void cModel::setMdlDimensions(mdlDimensions mdlDims)
{
	m_Dimensions = mdlDims;
	//m_mdlRadius = m_Dimensions.s_mdldepth / 2;
	m_mdlRadius = m_Dimensions.s_mdlWidth / 2;
}
void cModel::setDirection(glm::vec3 mdlDirection){
	m_mdlDirection = mdlDirection;
}
glm::vec3 cModel::getDirection(){
	return m_mdlDirection;
}
void cModel::setIsActive(bool mdlIsActive){
	m_IsActive = mdlIsActive;
}
bool cModel::isActive(){
	return m_IsActive;
}
float cModel::getMdlRadius()
{
	return m_mdlRadius;
}
void cModel::setMdlRadius(float mdlRadius)
{
	m_mdlRadius = mdlRadius;
}
void cModel::increaseMdlSpeed(float speed){
	m_mdlSpeed += speed;
}
void cModel::setSpeed(float mdlSpeed){
	m_mdlSpeed = mdlSpeed;
}
void cModel::initialise(glm::vec3 mdlPosition, GLfloat mdlRotation, glm::vec3 mdlVelocity)
{
	setPosition(mdlPosition);
	setRotation(mdlRotation);
	setVelocity(mdlVelocity);
	glm::vec3 mdlPos = getPosition();
	GLfloat mdlRot = getRotation();
	glRotatef(mdlRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(mdlPos.x,mdlPos.y,mdlPos.z);
	m_Dimensions.s_mdldepth = 0.0f;
	m_Dimensions.s_mdlheight = 0.0f;
	m_Dimensions.s_mdlWidth = 0.0f;
	m_mdlSpeed = 5.0f;

	//shader
	modelMatrix = glm::translate(modelMatrix, mdlPosition);
	
}
void cModel::update(float elapsedTime)
{
	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = -(float)glm::sin(m_mdlRotation);
	mdlVelocityAdd.z = -(float)glm::cos(m_mdlRotation);
	mdlVelocityAdd *= elapsedTime;

	//m_mdlVelocity *=  mdlVelocityAdd;// elapsedTime;

	m_mdlPosition += m_mdlVelocity;
	
	glm::vec3 mdlPos = getVelocity();
	//glTranslatef(mdlPos.x, mdlPos.y, mdlPos.z);

}
bool cModel::SphereSphereCollision(glm::vec3 mdlPosition, float mdlRadius){

	const float distSq = lengthSQRD(m_mdlPosition - mdlPosition);

	const float sumRadius = (m_mdlRadius + mdlRadius);

	if (distSq < sumRadius * sumRadius)
	{
		return true; // Collision
	}
	return false; // No Collision

}
float cModel::lengthSQRD(glm::vec3 mdlLength){

	return (mdlLength.x * mdlLength.x) + (mdlLength.y * mdlLength.y) + (mdlLength.z * mdlLength.z);
}
cModel::~cModel()
{

}
