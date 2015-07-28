#ifndef _CMODELLOADER_H
#define _CMODELLOADER_H

#include "GameConstants.h"
#include "cImageLoader.h"
#include "objloader.hpp"

class cModelLoader
{
public:
	cModelLoader();

	void initialise(const char* mdlFilename);
	void renderMdl(glm::vec3 mdlPosition, float mdlRotationAngle);
	void renderMdl(glm::vec3 mdlPosition, float mdlRotationAngle, GLuint texture);
	mdlDimensions getModelDimensions();

	~cModelLoader();
	//WITH SHADERS================================
	void draw();
	void init(const char* mdlFilename, GLuint programID, const char* textFilename);

private:
	// Set the position of the model in world space, and set the rotation.
	GLMmodel* m_model;


	GLuint vertexArrayID;
	GLuint vertexbuffer;
	GLuint colourbuffer;

	GLuint TextureID;
	GLuint Texture;
	GLuint uvbuffer;
	GLuint normalbuffer;

	void loadVertexData(std::vector< glm::vec3 > vertices);
	void loadColourData();
	void loadTexData(std::vector< glm::vec2 > uvs);
	void loadNormals(std::vector< glm::vec3 > normals);
	GLuint loadTexture(const char* filename);

	cImageLoader * imageLoader = new cImageLoader();

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
};
#endif