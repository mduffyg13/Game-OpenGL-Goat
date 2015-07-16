#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "GameConstants.h"
#include "cImageLoader.h"

class cSkyBox {
public:
	cSkyBox();
	void init(GLuint programID, 
		const char* frontFilename,
		const char* backFilename, 
		const char* topFilename, 
		const char* bottomFilename, 
		const char* leftFilename,
		const char* rightFilename,
		glm::vec3 cameraPosition);

	void CreateSkyBox(glm::vec3 cameraPosition);
	void draw();
	bool load_cube_map_side(GLenum side_target, const char* file_name);
	void create_cube_map(
		const char* front,
		const char* back,
		const char* top,
		const char* bottom,
		const char* left,
		const char* right);//GLuint* tex_cube
	~cSkyBox();
private:
	cImageLoader * imageLoader = new cImageLoader();
	GLuint vbo;
	GLuint vao;
	GLuint TextureID;
	GLuint shaderProgram;
	GLuint tex_cube;
	std::vector< glm::vec3 > vertices;
};

#endif