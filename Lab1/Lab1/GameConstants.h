#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

#define GLM_FORCE_RADIANS

#include <windows.h> 
#include <stdio.h>
#include <stdlib.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "OpenAL\al.h"
#include "OpenAL\alc.h"
#include "OpenAL\alut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\geometric.hpp""
#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm-0.3.2\glm\glm.h"
#include <vector>
#include <sstream>
#include <string>
#include "shader.hpp"

//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "OpenGl Models, Lighting and Text"

struct mdlDimensions{
	float s_mdlWidth, s_mdlheight, s_mdldepth;
};

extern int drawMode;
extern float rotationAngle;
extern float translationZ;
extern int g_XPos;
extern bool g_PlayerMoving;
extern bool g_PlayerFire;

extern int camera;
extern int dir_switch;
const glm::vec3 DIR_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 DIR_LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);

const float enemy_speed_increse = 1.2f;
const float player_speed_increase = 1.0f;

extern float txtxpos;
extern float txtypos;

extern bool game;
const int gameScene[] = {0, 1, 2};
extern bool gameOver;
extern bool mute;
extern bool debug;


#endif