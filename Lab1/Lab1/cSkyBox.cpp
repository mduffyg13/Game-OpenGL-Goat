#include "cSkyBox.h"



cSkyBox::cSkyBox(){}
void cSkyBox::init(GLuint programID,
	const char* frontFilename,
	const char* backFilename,
	const char* topFilename,
	const char* bottomFilename,
	const char* leftFilename,
	const char* rightFilename,
	glm::vec3 cameraPosition){
	
	
	CreateSkyBox(cameraPosition);
	TextureID = glGetUniformLocation(programID, "gCubemapTexture");
	create_cube_map(frontFilename, backFilename, topFilename, bottomFilename, leftFilename, rightFilename);
	shaderProgram = programID;
}
void cSkyBox::CreateSkyBox(glm::vec3 cameraPosition){
	int scale = 50;
	//glm::vec4 q = glm::vec4((-1, -1); (1, -1); (1, 1);  (-1, 1));
	glm::vec3 v[] = {
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),

		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale),

		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),

		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale),

		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y + 1.0f * scale, cameraPosition.z - 1.0f * scale),

		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z - 1.0f * scale),
		glm::vec3(cameraPosition.x - 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale),
		glm::vec3(cameraPosition.x + 1.0f * scale, cameraPosition.y - 1.0f * scale, cameraPosition.z + 1.0f * scale)
	};

	for (int i = 0; i < 36; i++){
		vertices.push_back(v[i]);
	}

	vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	/*vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/
}

void cSkyBox::create_cube_map(
	const char* front,
	const char* back,
	const char* top,
	const char* bottom,
	const char* left,
	const char* right ) {
	//GLuint* tex_cube
	
	// generate a cube-map texture to hold all the sides
	//tex_cube = 0;
	 tex_cube = 0;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex_cube);


	imageLoader->LoadTexture(front);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);


	glTexImage2D(
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		0,
		GL_RGB,
		imageLoader->getImageWidth(),
		imageLoader->getImageHeight(),
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		imageLoader->getImageData()
		);

	// load each image and copy into a side of the cube-map texture
	//assert(
		//load_cube_map_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	//assert(
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	//assert(
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	//assert(
		load_cube_map_side( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	//assert(
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_X, left);
	//assert(
		load_cube_map_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, right);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


bool cSkyBox::load_cube_map_side(GLenum side_target, const char* file_name) {


	//glBindTexture(GL_TEXTURE_CUBE_MAP, texture);


	imageLoader->LoadTexture(file_name);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);

	
	glTexImage2D(
		side_target,
		0,
		GL_RGB,
		imageLoader->getImageWidth(),
		imageLoader->getImageHeight(),
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		imageLoader->getImageData()
		);
	//free(image_data);
	return true;
}


void cSkyBox::draw(){
	glDepthMask(GL_FALSE);
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(TextureID, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
	
	//glBindVertexArray(vao);

	//1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);
	glDepthMask(GL_TRUE);
	
}
cSkyBox::~cSkyBox(){}