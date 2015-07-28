#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define _CRT_SECURE_NO_DEPRECATE
#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one

#include <time.h>
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cGround.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cBullet.h"
#include "cSound.h"
#include "cCollectable.h"
#include"text2D.hpp"
#include "cSkyBox.h"




int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{


	//Set our window settings.
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;

	//This is our window.
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	//The OpenGL code window.
	windowOGL theOGLWnd;

	//Attach our opengl to our window.
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);


	//Attempt to create the window.
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP)){

		//If it fails.
		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND();
		//Reset the display and exit .
		return 1;
	}

	if (!theOGLWnd.initOGL()) //Initialize our example 
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit 
		return 1;
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	//Camera vectors.
	glm::vec3 cameraPostition = glm::vec3(0.0, 15.0, -70.0);
	glm::vec3 cameraTarget = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

	//Second camera
	glm::vec3 cameraPostition2 = glm::vec3(-40.0, 70.0, -75.0);
	glm::vec3 cameraTarget2 = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 cameraUp2 = glm::vec3(0.0, 1.0, 0.0);

	// Create and compile our GLSL program from the shaders (LIGHT)
	GLuint programID = LoadShaders("Shaders/StandardShading.vertexshader", "Shaders/StandardShading.fragmentshader");
	

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	//Skybox shaders.
	GLuint programID_skybox2 = LoadShaders("Shaders/SkyBoxVertexShader2.vertexshader", "Shaders/SkyBoxFragmentShader2.fragmentshader");
	GLuint p = glGetUniformLocation(programID_skybox2, "P");
	GLuint v = glGetUniformLocation(programID_skybox2, "V");
	
	//Load skybox textures.
	cSkyBox skyBox;
	skyBox.init(programID_skybox2, "Skybox/Front.bmp", "Skybox/Back.bmp",
		"Skybox/Top.bmp", "Skybox/Bottom.bmp",
		"Skybox/Left.bmp", "Skybox/Right.bmp",
		cameraPostition);
		
	//Load Audio
	cSound gameMusic;
	gameMusic.createContext();
	gameMusic.loadWAVFile("Audio/GameMusic.wav");
	cSound shotFX;
	shotFX.loadWAVFile("Audio/Shotgun.wav");
	cSound sound;
	sound.loadWAVFile("Audio/cow4.wav");
	cSound titleMusic;
	titleMusic.loadWAVFile("Audio/TitleMusic.wav");

	//Load Font
	initText2D("Fonts/Holstein.DDS");

	//Load models
	cModelLoader sGround;
	sGround.init("Models/Ground3.OBJ", programID, "Models/Textures/Grass1.jpg");
	cModelLoader sPlayer;
	sPlayer.init("Models/Goat2.OBJ", programID, "Models/Textures/Goat_D.tga");
	cModelLoader smEnemy;
	smEnemy.init("Models/Cow.OBJ", programID, "Models/Textures/COWSKIN.jpg");
	cModelLoader smBullet;
	smBullet.init("Models/Hammer.OBJ", programID, "Models/Textures/checker.bmp");
	cModelLoader smCollectable;
	smCollectable.init("Models/cane.OBJ", programID, "Models/Textures/checker.bmp");


	cModelLoader mdlGround;
	mdlGround.initialise("Models/Ground.OBJ");
	cModelLoader mdlPlayer;
	mdlPlayer.initialise("Models/Goat.OBJ");
	cModelLoader mdlEnemy;
	mdlEnemy.initialise("Models/Cow.OBJ");
	cModelLoader mdlBullet;
	mdlBullet.initialise("Models/Hammer.OBJ");
	cModelLoader mdlCollectable;
	mdlCollectable.initialise("Models/Collectable.OBJ");

	//Seed random number generator
	srand(time(NULL));

	cGround * ground = new cGround();
	ground->setMdlDimensions(mdlGround.getModelDimensions());
	ground->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	
	cPlayer oPlayer;
	oPlayer.initialise(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	oPlayer.setMdlDimensions(mdlPlayer.getModelDimensions());

	//Array of horizontal lane positions
	float aXPos[] = {
		-ground->getMdlDimensions().s_mdlWidth / 5 * 2,
		-ground->getMdlDimensions().s_mdlWidth / 5 * 1,
		ground->getMdlDimensions().s_mdlWidth / 5 * 0,
		ground->getMdlDimensions().s_mdlWidth / 5 * 1,
		ground->getMdlDimensions().s_mdlWidth / 5 * 2 };

	//Set initial player position
	oPlayer.setPosition(glm::vec3(aXPos[g_XPos],
		mdlPlayer.getModelDimensions().s_mdlheight / 2,
		-mdlGround.getModelDimensions().s_mdldepth / 2 + mdlPlayer.getModelDimensions().s_mdlheight / 2));


	//Enemy/Collectable Spawn Points
	glm::vec3 aEnemySpawn[] = {
		glm::vec3(aXPos[0], mdlEnemy.getModelDimensions().s_mdlheight / 2, mdlGround.getModelDimensions().s_mdldepth / 2 - mdlEnemy.getModelDimensions().s_mdlheight / 2),
		glm::vec3(aXPos[1], mdlEnemy.getModelDimensions().s_mdlheight / 2, mdlGround.getModelDimensions().s_mdldepth / 2 - mdlEnemy.getModelDimensions().s_mdlheight / 2),
		glm::vec3(aXPos[2], mdlEnemy.getModelDimensions().s_mdlheight / 2, mdlGround.getModelDimensions().s_mdldepth / 2 - mdlEnemy.getModelDimensions().s_mdlheight / 2),
		glm::vec3(aXPos[3], mdlEnemy.getModelDimensions().s_mdlheight / 2, mdlGround.getModelDimensions().s_mdldepth / 2 - mdlEnemy.getModelDimensions().s_mdlheight / 2),
		glm::vec3(aXPos[4], mdlEnemy.getModelDimensions().s_mdlheight / 2, mdlGround.getModelDimensions().s_mdldepth / 2 - mdlEnemy.getModelDimensions().s_mdlheight / 2) };

	//List of enemies
	std::vector<cModel*> vEnemyList;
	std::vector<cModel*>::iterator enemyIndex;

	//Pre calculate possible bullet spawn points
	glm::vec3 aBulletSpawn[] = {
		glm::vec3(aXPos[0], mdlPlayer.getModelDimensions().s_mdlheight / 2, oPlayer.getPosition().z + mdlPlayer.getModelDimensions().s_mdlWidth / 2 + mdlBullet.getModelDimensions().s_mdlWidth / 2),
		glm::vec3(aXPos[1], mdlPlayer.getModelDimensions().s_mdlheight / 2, oPlayer.getPosition().z + mdlPlayer.getModelDimensions().s_mdlWidth / 2 + mdlBullet.getModelDimensions().s_mdlWidth / 2),
		glm::vec3(aXPos[2], mdlPlayer.getModelDimensions().s_mdlheight / 2, oPlayer.getPosition().z + mdlPlayer.getModelDimensions().s_mdlWidth / 2 + mdlBullet.getModelDimensions().s_mdlWidth / 2),
		glm::vec3(aXPos[3], mdlPlayer.getModelDimensions().s_mdlheight / 2, oPlayer.getPosition().z + mdlPlayer.getModelDimensions().s_mdlWidth / 2 + mdlBullet.getModelDimensions().s_mdlWidth / 2),
		glm::vec3(aXPos[4], mdlPlayer.getModelDimensions().s_mdlheight / 2, oPlayer.getPosition().z + mdlPlayer.getModelDimensions().s_mdlWidth / 2 + mdlBullet.getModelDimensions().s_mdlWidth / 2)
	};



	//List of bullets
	std::vector<cBullet*> vBulletList;
	std::vector<cBullet*>::iterator bulletIndex;

	float bullet_cooldown = 1.0f;


	//Timers
	float spawnTime = 5.0f;
	float spawn_timer = 0.0f;
	float bullet_cooldown_timer = bullet_cooldown;
	float time = 0.0f;

	//Score data
	int enemiesKilled = 0;
	int wave_counter = 0;
	int collectable_counter = 0;
	int score = 000;
	int combo = 1;
	int wave = 0;

	//triggers
	bool collectable_spawn = false;
	bool titleMusicPlaying = false;
	bool gameMusicPlaying = false;
	

	float move = 0;

	float player_speed = 0.0f;
	float enemy_speed = 0.0f;

	const int new_wave_trigger = 2;
	const int combo_chance = 3;

	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning()) {

		//Process any window events
		pgmWNDMgr->processWNDEvents(); 

		//Time that passed since the last frame 
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);



		//if in game state
		if (game){
			//start timers
			spawn_timer += elapsedTime;
			bullet_cooldown_timer -= elapsedTime;
			player_speed = oPlayer.getSpeed();

			//mute/unmute music
			if (!mute){
				if (titleMusicPlaying){
					titleMusic.stopAudio();
					titleMusicPlaying = false;

				}
				if (!gameMusicPlaying){
					gameMusic.playAudio(AL_FALSE);
					gameMusicPlaying = true;
				}
			}
			else{
				sound.stopAudio();
				gameMusic.stopAudio();
				gameMusicPlaying = false;
			}


			//Game
			//=====================================================================================================================
			//=====================================================================================================================




			//===========================shaders
		//	glUseProgram(programID);
			/////=================================================
			// Get a handle for our "MVP" uniform
			//GLuint MatrixID = glGetUniformLocation(programID, "MVP");
			// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

		///////////////////////	glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 1.0f, 150.0f);
			
			//Define camera mitrices
			glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 1.0f, 150.0f);
			glm::mat4 View = glm::lookAt(cameraPostition, cameraTarget, cameraUp);
			
			// Change camera matrix
			if (camera == 0){
				Projection = glm::perspective(45.0f, 1.0f, 1.0f, 150.0f);
				 View = glm::lookAt(cameraPostition, cameraTarget, cameraUp);
			}
			else{
				Projection = glm::perspective(45.0f, 1.0f, 1.0f, 150.0f);
				//Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
			 View = glm::lookAt(cameraPostition2, cameraTarget2, cameraUp2);
				//	cameraPostition = glm::vec3(0.0, 50.0, 50.0);
			}
		///////////////////////////	glm::mat4 View = glm::lookAt(cameraPostition, cameraTarget, cameraUp);


			//Make skybox shader current
			glUseProgram(programID_skybox2);
			
			glm::mat4 MVP = Projection * View * glm::mat4(1.0f);
			
			//Pass view and projection matrices
			glUniformMatrix4fv(p, 1, GL_FALSE, &Projection[0][0]);
			glUniformMatrix4fv(v, 1, GL_FALSE, &View[0][0]);

			//Draw sky box
			skyBox.draw(); 

			



			//Make model shader current
			glUseProgram(programID);

			
			
			//Send MVP to pipeline - update shader info for each model
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ground->getModelMatrix()[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);

			//Define light position, send to shader
			glm::vec3 lightPos = glm::vec3(0, 10, -20);
			glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

			//Draw ground
			sGround.draw();

			//Update & Draw player
			if (g_PlayerMoving){
				oPlayer.update(elapsedTime, aXPos[g_XPos]);
			}
			

			//Recalculate MVP
			MVP = Projection * View * oPlayer.getModelMatrix();

			//Send updated matrices to shader
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &oPlayer.getModelMatrix()[0][0]);
			
			//Draw player
			sPlayer.draw();

			//When spawn time triggered
			if (spawn_timer > spawnTime){

				//if collectable trigger reached
				//roll for collectable
				//if true spawn
				//if false spawn enemy
				if (collectable_counter >= new_wave_trigger){


					int collectable_chance = rand() % combo_chance;

					//If random chance matches 1
					if (collectable_chance == 1){
						//collectable bool - disables enemy spawn when collectable spawned
						collectable_spawn = true;
						//spawn collectable
						cCollectable* collectable = new cCollectable();

						//random spawn point
						int spawn = rand() % 5;

						collectable->initialise(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
						collectable->setPosition(aEnemySpawn[spawn]);
						collectable->setIsActive(true);
						collectable->setMdlDimensions(mdlCollectable.getModelDimensions());
						//increase speed for advance in waves
						collectable->increaseMdlSpeed(enemy_speed_increse * wave);
						//add to cModel vector
						vEnemyList.push_back(collectable);

						//reset collectable trigger
						collectable_counter = 0;
						
					}
				}

				//if no collectable spawned this time spawn enemy
				if (!collectable_spawn){
					cEnemy* enemy = new cEnemy();
					int spawn = rand() % 5;

					enemy->initialise(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
					enemy->setPosition(aEnemySpawn[spawn]);
					enemy->setIsActive(true);
					enemy->setMdlDimensions(mdlEnemy.getModelDimensions());
					enemy->increaseMdlSpeed(enemy_speed_increse * wave);
					enemy_speed = enemy->getSpeed();
					vEnemyList.push_back(enemy);
					if (!mute){
						//play enemy spawn audio
						sound.playAudio(AL_FALSE);
					}
				}
				//reset spawn trigger
				spawn_timer = 0;
				//reset collectable spawn bool
				collectable_spawn = false;
			}
			//if player presses fire key
			if (g_PlayerFire){
				
				//if enough time has passed since last bullet
				if (bullet_cooldown_timer < 0.0f){
					cBullet* bullet = new cBullet();
					bullet->initialise(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
					bullet->setPosition(aBulletSpawn[g_XPos]);
					bullet->setSpeed(17.0f);
					bullet->setIsActive(true);
					bullet->setMdlDimensions(mdlBullet.getModelDimensions());
					vBulletList.push_back(bullet);
					if (!mute){
						//play shot sound
						shotFX.playAudio(AL_FALSE);
					}
					//set bullet cooldown
					bullet_cooldown_timer = bullet_cooldown;
				}

				//enable other shot
				g_PlayerFire = false;
			}

			///=============================================================================ob render

			//for each enemy/ collectable in list
			for (enemyIndex = vEnemyList.begin(); enemyIndex != vEnemyList.end(); ++enemyIndex){
				//if active
				if ((*enemyIndex)->isActive()){

					//if enemy reaches end of board
					if ((*enemyIndex)->getPosition().z <= oPlayer.getPosition().z){
						
						//If not collectable
						if (cEnemy * e = dynamic_cast<cEnemy*>((*enemyIndex))){
							
							//remove enemy
							(*enemyIndex)->setIsActive(false);
							
							//set to game over state
							game = false;
							gameOver = true;
						}
					}



					//If enemy
					if (cEnemy * e = dynamic_cast<cEnemy*>((*enemyIndex))){
						//reset matrices and draw
					MVP = Projection * View * (*enemyIndex)->getModelMatrix();
						glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*enemyIndex)->getModelMatrix()[0][0]);
						smEnemy.draw();

					}
					//if collectable
					if (cCollectable * c = dynamic_cast<cCollectable*>((*enemyIndex))){
						//reset matrices and draw
						MVP = Projection * View * (*enemyIndex)->getModelMatrix();
						glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*enemyIndex)->getModelMatrix()[0][0]);
						smCollectable.draw();

						//if collectable hits player
						if ((*enemyIndex)->isActive() && (*enemyIndex)->SphereSphereCollision(oPlayer.getPosition(), oPlayer.getMdlRadius())){
						
							//increase combp
							combo++;
							//remove collectable
							(*enemyIndex)->setIsActive(false);
						
						}
						

					}
					//update enemy/collectable
					(*enemyIndex)->update(elapsedTime);
				}


			}
			//for each active bullet
			for (bulletIndex = vBulletList.begin(); bulletIndex != vBulletList.end(); ++bulletIndex){
				if ((*bulletIndex)->isActive()){


					//reset matrices and draw
					MVP = Projection * View * (*bulletIndex)->getModelMatrix();
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
					glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(*bulletIndex)->getModelMatrix()[0][0]);
					smBullet.draw();

					//update bullet
					(*bulletIndex)->update(elapsedTime);

					//for each active enemy
					for (enemyIndex = vEnemyList.begin(); enemyIndex != vEnemyList.end(); ++enemyIndex){

						if ((*enemyIndex)->isActive()){
							//check for collision
							if ((*bulletIndex)->SphereSphereCollision((*enemyIndex)->getPosition(), (*enemyIndex)->getMdlRadius())){

								//if collision update counters
								enemiesKilled += 1;
								wave_counter++;
								collectable_counter++;

								//new wave reached
								if (wave_counter == new_wave_trigger){
									oPlayer.increaseMdlSpeed(player_speed_increase);	//increase player movement speed
									wave++;												//increase wave count
									wave_counter = 0;									//reset enemies killed counter
								}

								score += 100 * combo;									//update score
								(*bulletIndex)->setIsActive(false);						//remove bullet
								(*enemyIndex)->setIsActive(false);						//remove enemy
								break;
							}
						}
					}
				}
			}



			//==========================================================

			
		}//end of game loop


		//if in game state
		if (game){

			//draw score and combo txt
			time += elapsedTime;
			char text[256];

			sprintf(text, "Combo x%i", combo);
			printText2D(text, 12, 556, 24);

			sprintf(text, "Score %i", score); //.2f sec
			printText2D(text, 550, 556, 24);

			


		}
		//if in game over state
		else if (gameOver){

			char text[256];
			//display game over
			sprintf(text, "GAME OVER");
			printText2D(text, 230, 315, 50);
		
		}
		//if in title screen state
		else{
			glClearColor(0.2f, 0.5f, 0.4f, 0.0f);

			//play title music
			if (!mute){
				if (gameMusicPlaying){
					gameMusic.stopAudio();
					gameMusicPlaying = false;
				}
				if (!titleMusicPlaying){
					titleMusic.playAudio(AL_FALSE);
					titleMusicPlaying = true;
				}
			}


			char text[256];

			//display title text
			sprintf(text, "GOATZ");
			printText2D(text, 250, 315, 50);
			sprintf(text, "Press Fire");
			printText2D(text, 250, 315-50, 25);
			
		}
		//if in debug mode
		if (debug){

			time += elapsedTime;
			char text[256];

			int fontsz = 14;
			sprintf(text, "Time - %.2f", time);
			printText2D(text, 10, 300, fontsz);
			sprintf(text, "Score - %i", score);
			printText2D(text, 10, 300 - fontsz, fontsz);
			sprintf(text, "Combo - %i", combo);
			printText2D(text, 10, 300 - fontsz * 2, fontsz);
			sprintf(text, "Enemies killed - %i", enemiesKilled);
			printText2D(text, 10, 300 - fontsz * 3, fontsz);
			sprintf(text, "Wave counter - %i", wave_counter);
			printText2D(text, 10, 300 - fontsz * 4, fontsz);
			sprintf(text, "Wave - %i", wave);
			printText2D(text, 10, 300 - fontsz * 5, fontsz);

			if (bullet_cooldown_timer > 0){sprintf(text, "Bullet timer - %.2f", bullet_cooldown_timer);}
			else{ sprintf(text, "Bullet timer - 0.0"); }
			printText2D(text, 10, 300 - fontsz * 6, fontsz);


			sprintf(text, "Player speed - %.2f", player_speed);
			printText2D(text, 10, 300 - fontsz * 7, fontsz);
			sprintf(text, "Enemy speed - %.2f", enemy_speed);
			printText2D(text, 10, 300 - fontsz * 8, fontsz);
			sprintf(text, "Spawn timer - %.2f", spawn_timer);
			printText2D(text, 10, 300 - fontsz * 9, fontsz);
			sprintf(text, "Spawn speed - %.2f", spawnTime);
			printText2D(text, 10, 300 - fontsz * 10, fontsz);
			sprintf(text, "Collectable counter - %i", collectable_counter);
			printText2D(text, 10, 300 - fontsz * 11, fontsz);
			
		}


		//display current frame
		pgmWNDMgr->swapBuffers();
	}




	theOGLWnd.shutdown();//Free any resources
	pgmWNDMgr->destroyWND();//Destroy the program window

	return 0; //Return success

}
