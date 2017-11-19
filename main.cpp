#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
//#include "Dependencies\glui\glui.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "shader_install.h"
#include "Loading_func.h"
#include "Bind_Data.h"

using namespace std;
using glm::vec3;
using glm::mat4;
// ============================= //
// window size
int Win_w, Win_h;
float camera_fov = 45.0;

int  mainWindowID;
GLint programID;
GLint skyboxID;
//earth 
GLuint TextureEarth;


extern GLuint earthVao;
extern int drawEarthSize;

float earth_innRot_Degree = 0.0f;

//car
GLuint TextureCar;

extern GLuint carVao;
extern int drawCarSize;

float car_innRot_Degree = 0.0f;
//skybox
GLuint earth_cubemapTexture;

extern GLuint cubeVao;
extern int drawCubeSize;

float cube_innRot_Degree = 0.0f;
// view matrix
glm::mat4 common_viewM;
glm::mat4 common_projection;



// ============================= //
const float M_PI = 3.14159265;
float radius = 30.0f;
float initViewHorizontal = -90.0f;
float initViewVertical = -90.0f;

float cameraX = 0;
float cameraY = 0;
float cameraZ = radius;

float viewRotateDegree[3] = { 0.0f, 0.0f, 0.0f };

float a_brightness = 1.0f;
float d_brightness = 0.0f;
float s_brightness = 0.6f;

void Mouse_Wheel_Func(int button, int state, int x, int y)
{
	if ((button == 3) || (button == 4))
	{
		if (state == GLUT_UP) return;
		if (button == 3)
		{
			radius -= 1.0f;
			cameraX = radius* cos(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraY = radius* cos(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraZ = radius* sin(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
		}
		else
		{
			radius += 1.0f;
			cameraX = radius* cos(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraY = radius* cos(glm::radians(initViewVertical + viewRotateDegree[0]));
			cameraZ = radius* sin(glm::radians(initViewHorizontal + viewRotateDegree[1]))*sin(glm::radians(initViewVertical + viewRotateDegree[0]));
		}
	}
}

void LoadAllTextures()
{
	TextureEarth = loadBMP2Texture("texture/earth.bmp");
	TextureCar = loadBMP2Texture("texture/helicopter.bmp");
	
	//skybox
	vector <const GLchar*> earth_faces;
	earth_faces.push_back("texture/aurora_skybox/right.bmp");
	earth_faces.push_back("texture/aurora_skybox/left.bmp");
	earth_faces.push_back("texture/aurora_skybox/top.bmp");
	earth_faces.push_back("texture/aurora_skybox/bottom.bmp");
	earth_faces.push_back("texture/aurora_skybox/back.bmp");
	earth_faces.push_back("texture/aurora_skybox/front.bmp");
	earth_cubemapTexture = loadCubemap(earth_faces);
}

void sendDataToOpenGL()
{
	//Load objects and bind to VAO & VBO
	bindCube();
	bindEarth("model_obj/planet.obj");
	bindCar("model_obj/Car_porsche.obj");
	// load all textures
	LoadAllTextures();
}

float zLightPos = 0.0f;
float yLightPos = 20.0f;
void set_lighting()
{
	glUseProgram(programID);

	// ambient
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	glm::vec3 ambientLight(a_brightness, a_brightness, a_brightness);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	// diffusion
	GLint kd = glGetUniformLocation(programID, "coefficient_d");
	glm::vec3 vec_kd(d_brightness, d_brightness, d_brightness);
	glUniform3fv(kd, 1, &vec_kd[0]);
	// specular
	GLint ks = glGetUniformLocation(programID, "coefficient_s");
	glm::vec3 vec_ks(s_brightness, s_brightness, s_brightness);
	glUniform3fv(ks, 1, &vec_ks[0]);
	// eye position
	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	vec3 eyePosition(cameraX, cameraY, cameraZ);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glm::vec3 lightPosition(0.0, yLightPos, zLightPos);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	// light color
	GLint lightColorUniformLocation = glGetUniformLocation(programID, "lightColor");
	glm::vec4 lightColor(1.0, 1.0, 1.0, 1.0);
	glUniform4fv(lightColorUniformLocation, 1, &lightColor[0]);
}

void set_lighting_skybox()
{
	glUseProgram(skyboxID);

	// ambient
	GLint ambientLightUniformLocation = glGetUniformLocation(skyboxID, "ambientLight");
	glm::vec3 ambientLight(a_brightness, a_brightness, a_brightness);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	// diffusion
	GLint kd = glGetUniformLocation(skyboxID, "coefficient_d");
	glm::vec3 vec_kd(d_brightness, d_brightness, d_brightness);
	glUniform3fv(kd, 1, &vec_kd[0]);
	// specular
	GLint ks = glGetUniformLocation(skyboxID, "coefficient_s");
	glm::vec3 vec_ks(s_brightness, s_brightness, s_brightness);
	glUniform3fv(ks, 1, &vec_ks[0]);
	// eye position
	GLint eyePositionUniformLocation = glGetUniformLocation(skyboxID, "eyePositionWorld");
	vec3 eyePosition(cameraX, cameraY, cameraZ);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
	GLint lightPositionUniformLocation = glGetUniformLocation(skyboxID, "lightPositionWorld");
	glm::vec3 lightPosition(0.0, yLightPos, zLightPos);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	// light color
	GLint lightColorUniformLocation = glGetUniformLocation(skyboxID, "lightColor");
	glm::vec4 lightColor(1.0, 1.0, 1.0, 1.0);
	glUniform4fv(lightColorUniformLocation, 1, &lightColor[0]);
}

void drawcube() {

	//cube
	GLfloat scale_fact = 100.0f;

	glUseProgram(skyboxID);
	//skybox cube
	glBindVertexArray(cubeVao);
	
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::mat4(1.0f);
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	// texture
	GLuint TextureID = glGetUniformLocation(skyboxID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(TextureID, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, earth_cubemapTexture);
	glActiveTexture(GL_TEXTURE1);

	glDrawArrays(GL_TRIANGLES, 0, drawCubeSize);
	glBindVertexArray(0);
}

void drawEarth(void)
{
	//earth
	GLfloat scale_fact = 3.0f;

	glUseProgram(programID);

	glBindVertexArray(earthVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(earth_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureEarth);
	glUniform1i(TextureID, 0);
	glActiveTexture(GL_TEXTURE1);

	glDrawArrays(GL_TRIANGLES, 0, drawEarthSize);
}


void drawCar(void)
{
	//car
	GLfloat scale_fact = 3.0f;

	glUseProgram(programID);

	glBindVertexArray(carVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(car_innRot_Degree), glm::vec3(0, 1, 1));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 10.0f, 10.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureCar);
	glUniform1i(TextureID, 0);
	glActiveTexture(GL_TEXTURE1);

	glDrawArrays(GL_TRIANGLES, 0, drawCarSize);
}

void paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// ================================ //
	// view matrix
	common_viewM = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	// projection matrix
	common_projection = glm::perspective(camera_fov, 1.0f, 0.1f, 200.0f);


	
	//=== draw ===//
	// set lighting parameters
	set_lighting();
	//set_lighting_skybox();
	// draw earth
	glDepthMask(GL_FALSE);
	drawcube();
	glDepthMask(GL_TRUE);
	
	drawEarth();
	drawCar();
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void Shaders_Installer()
{
	programID = installShaders("shader/Common.vs", "shader/Common.frag");
	skyboxID = installShaders("skybox/skybox.vs", "skybox/skybox.frag");
	if (!checkProgramStatus(programID))
		return;
	if (!checkProgramStatus(skyboxID))
		return;
}

void initializedGL(void)
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	

	// install all shaders
	Shaders_Installer();
	// load required obj and textures
	sendDataToOpenGL();
	
}

void timerFunction(int id)
{
	earth_innRot_Degree += 0.3;
	car_innRot_Degree += 0.3;
	
	glutPostRedisplay();
	glutTimerFunc(700.0f / 60.0f, timerFunction, 1);
}

int main(int argc, char *argv[])
{
	/*Initialization of GLUT library*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	int height = GetSystemMetrics(SM_CYSCREEN)*0.8;
	glutInitWindowSize(height, height);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	mainWindowID = glutCreateWindow("i-Navigation");

	// initialize openGL
	initializedGL();
	// draw
	glutDisplayFunc(paintGL);

	glutMouseFunc(Mouse_Wheel_Func);

	glutTimerFunc(700.0f / 60.0f, timerFunction, 1);

	/*Enter the GLUT event processing loop which never returns.*/
	glutMainLoop();

	return 0;
}