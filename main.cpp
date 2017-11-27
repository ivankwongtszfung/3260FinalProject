#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glui\glui.h"
#include "Dependencies\irrKlang\irrKlang.h"
#include "Dependencies\irrKlang\conio.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

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

// ============================= window conf =============================//
int Win_w, Win_h;
float camera_fov = 45.0;
const float ASPECT = float(16) / 9;

int  mainWindowID;
GLUI *glui;
GLint programID;
GLint skyboxID;
GLint lightID;
GLint glassID;
GLint planetID;
// view matrix
glm::mat4 common_viewM;
glm::mat4 common_projection;
// ============================= Object conf =============================//
//Planet A
extern GLuint earthVao;
extern int drawEarthSize;
GLuint TextureEarth[2];
float earth_innRot_Degree = 0.0f;
float fogD = 0.001;
float fogG = 3.5;

//Planet B
extern GLuint moonVao;
extern int drawMoonSize;
GLuint TextureMoon;
float moon_innRot_Degree = 0.0f;
float moon_outnRot_Degree = 0.0f;
float moon_orbit_radius = 8.0f;

//Planet C
extern GLuint glassVao;
extern int drawGlassSize;
GLuint TextureGlass[2];
float glass_innRot_Degree = 0.0f;
//star
const int starAmount = 5;
extern glm::mat4 starMatrices[starAmount];


//Space vehicle
extern GLuint carVao;
extern int drawCarSize;
GLuint TextureCar;
float car_outnRot_Degree = 0.0f;
float car_orbit_radius = 8.0f;
float car_orbit_speed = 2.0f;
vec3 carPos = vec3(-2.0, 10.0, 0.0); //current car position
vec3 lastCarPos = vec3(-2.0, 10.0, 0.0); //last car position
vec3 carFront; //vector that the car is facing

static vec3 carPosArr[starAmount];
static int carPosIndex = 0;
bool cameraOnCar = false;

//skybox
GLuint earth_cubemapTexture;
extern GLuint cubeVao;
extern int drawCubeSize;

float cube_innRot_Degree = 0.0f;

//cube
GLuint cubeID;
int cubeVaoSize;
//light 
float light_innRot_Degree = 0.0f;
float xLightPos = 0.0f;
float zLightPos = 0.0f;
float yLightPos = 20.0f;

//rock
extern GLuint rockVao;
extern int drawRockSize;
GLuint TextureRock[2];
float rock_innRot_Degree = 0.0f;
float rock_outnRot_Degree = 0.0f;

//asteroidRing
const int asteroidAmount = 200;
extern glm::mat4 asteroidMatrices[asteroidAmount];
glm::mat4 rockModelMat_temp;

//star
extern GLuint starVao;
extern int drawStarSize;
GLuint TextureStar;
glm::mat4 starModelMat_temp;
vec3 starPos;

// ============================= camera conf =============================//
const float M_PI = 3.14159265;
float distanceFromCenter = 30.0f;
float pitch = 0.0f;
float angleAroundCenter = 0.0f;

vec3 camera = vec3(0.0f, 0.0f, distanceFromCenter);
vec3 tempCamera = vec3(0.0f, 0.0f, distanceFromCenter);
glm::vec3 cameraLookAt = glm::vec3(0, 0, 0);
glm::vec3 cameraNorm = glm::vec3(0, 1, 0);

static float old_x = 0.0f, old_y = 0.0f;
bool allowCameraMove = true;
// ============================= lighting conf =============================//
float a_brightness = 1.0f;
float d_brightness = 0.0f;
float s_brightness = 0.6f;
// ============================= GLUI conf =============================//
GLUI_RadioGroup *viewPoint;
GLUI_RadioGroup *fogColour;
int viewpointGroup; //0 = left 1 = top 2 = front
int fogColourGroup; //0 = red 1 = yellow 2 = blue
int fogEffectOnOff = 0; //0 = off, 1 = on

// ============================= User Input Function =============================//
void cameraPosition() {
	camera.y = distanceFromCenter * sin(glm::radians(pitch));
	camera.x = - distanceFromCenter * cos(glm::radians(pitch)) * sin(glm::radians(angleAroundCenter));
	camera.z = - distanceFromCenter * cos(glm::radians(pitch)) * cos(glm::radians(angleAroundCenter));
}


void Mouse_Wheel_Func(int button, int state, int x, int y)
{
	if (allowCameraMove && distanceFromCenter < 50.0f) {
		if ((button == 3) || (button == 4))
		{
			if (state == GLUT_UP) return;
			if (button == 3)
			{
				distanceFromCenter -= 1.0f;
				cameraPosition();
			}
			else
			{
				distanceFromCenter += 1.0f;
				cameraPosition();
			}
		}
	}
}

void keyboard(unsigned char key, int x, int y)
{
	//change viewpoint
	if (key == 'a') { //left view
		if (allowCameraMove) {
			tempCamera = camera;
		}
		camera = vec3(-50.0f, 0.0f, 0.0f);
		viewPoint->set_int_val(0);
		cameraOnCar = false;
		allowCameraMove = false;
	}
	else if (key == 's') { //top view
		if (allowCameraMove) {
			tempCamera = camera;
		}
		camera = vec3(0.0f, 30.0f, -3.5f);
		viewPoint->set_int_val(1);
		cameraOnCar = false;
		allowCameraMove = false;
	}
	else if (key == 'w') { //free view
		if (!allowCameraMove) {
			cameraOnCar = false;
			allowCameraMove = true;
			camera = tempCamera;
			viewPoint->set_int_val(3);
		}
	}

	else if (key == 'd') { //front view
		if (allowCameraMove) {
			tempCamera = camera;
		}
		camera = vec3(0.0f, 0.0f, 30.0f);
		viewPoint->set_int_val(2);
		cameraOnCar = false;
		allowCameraMove = false;
	}
	else if (key == '7') {
		a_brightness += 0.1f;
		printf("%.3lf\n", a_brightness);
	}
	else if (key == '1') {
		a_brightness -= 0.1f;
		printf("%.3lf\n", a_brightness);
	}
	else if (key == '8') {
		d_brightness += 0.1f;
		printf("%.3lf\n", d_brightness);
	}
	else if (key == '2') {
		d_brightness -= 0.1f;
		printf("%.3lf\n", d_brightness);
	}
	else if (key == '9') {
		s_brightness += 0.1f;
		printf("%.3lf\n", s_brightness);
	}
	else if (key == '3') {
		s_brightness -= 0.1f;
		printf("%.3lf\n", s_brightness);
	}
	else if (key == 'q') {
		xLightPos += 0.1f;
		printf("%.3lf\n", xLightPos);
	}
	else if (key == 'z') {
		xLightPos -= 0.1f;
		printf("%.3lf\n", xLightPos);
	}
	else if (key == 'x') {
		yLightPos -= 0.1f;
		printf("%.3lf\n", yLightPos);
	}
	else if(key=='e'){
		zLightPos += 0.1f;
		printf("%.3lf\n", zLightPos);
	}
	else if(key=='c'){
		zLightPos -= 0.1f;
		printf("%.3lf\n", zLightPos);
	}
	else if (key == '4') {
		fogD += 0.01f;
		printf("%.3lf\n", fogD);
	}
	else if (key == '0') {
		fogD -= 0.01f;
		printf("%.3lf\n", fogD);
	}
	else if (key == '5') {
		fogG += 0.1f;
		printf("%.3lf\n", fogG);
	}
	else if (key == '6') {
		fogG -= 0.1f;
		printf("%.3lf\n", fogG);
	}


}

void move(int key, int x, int y)
{
	//Real-time speed and orbit control
	//speed of vehicle
	if (key == GLUT_KEY_UP) {
		if(car_orbit_speed < 30.0)
			car_orbit_speed += 1;
	}
	else if (key == GLUT_KEY_DOWN) {
		if(car_orbit_speed > 2.0)
			car_orbit_speed -= 1;
	}
	//orbit radius size
	else if (key == GLUT_KEY_LEFT) {
		car_orbit_radius += 0.5f;
	}
	else if (key == GLUT_KEY_RIGHT) {
		if(car_orbit_radius > 5.5f)
			car_orbit_radius -= 0.5f;
	}
	
}

void PassiveMouse(int x, int y)
{
	if (allowCameraMove) {
		float dety = y - old_y;
		float detx = x - old_x;
		pitch += dety / 10;
		angleAroundCenter -= detx / 10;
		cameraPosition();
		old_x = x;
		old_y = y;
	}
}

// ============================= Function =============================//

void LoadAllTextures()
{
	TextureEarth[0] = loadBMP2Texture("texture/earth.bmp");
	TextureEarth[1] = loadBMP2Texture("normal_map/earth_normal.bmp");
	//skybox
	vector <const GLchar*> earth_faces;
	earth_faces.push_back("texture/aurora_skybox/right.bmp");
	earth_faces.push_back("texture/aurora_skybox/left.bmp");
	earth_faces.push_back("texture/aurora_skybox/top.bmp");
	earth_faces.push_back("texture/aurora_skybox/bottom.bmp");
	earth_faces.push_back("texture/aurora_skybox/back.bmp");
	earth_faces.push_back("texture/aurora_skybox/front.bmp");
	earth_cubemapTexture = loadCubemap(earth_faces);
	//glass
	TextureGlass[0] = loadBMP2Texture("texture/glass_a.bmp");
	TextureGlass[1] = loadBMP2Texture("texture/grass.bmp");
	//moon
	TextureMoon = loadBMP2Texture("texture/apple.bmp");
	//car
	TextureCar = loadBMP2Texture("texture/helicopter.bmp");
	//rock
	TextureRock[0] = loadBMP2Texture("texture/stone.bmp");
	TextureRock[1] = loadBMP2Texture("normal_map/stone_normal.bmp");
	//star
	TextureStar = loadBMP2Texture("texture/starfy.bmp");
}

void sendDataToOpenGL()
{
	//Load objects and bind to VAO & VBO
	bindCube();
	bindEarth("model_obj/planet.obj");
	bindGlass("model_obj/planet.obj");
	bindMoon("model_obj/planet.obj");
	bindCar("model_obj/helicopter2.obj");
	bindRock("model_obj/rock.obj");
	bindAsteroidRing();
	bindStar("model_obj/starfy.obj");
	// load all textures
	LoadAllTextures();
}

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
	vec3 eyePosition(camera.x, camera.y, camera.z);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glm::vec3 lightPosition(xLightPos, yLightPos, zLightPos);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	// light color
	GLint lightColorUniformLocation = glGetUniformLocation(programID, "lightColor");
	glm::vec4 lightColor(1.0, 1.0, 1.0, 1.0);
	glUniform4fv(lightColorUniformLocation, 1, &lightColor[0]);
}

void set_lighting_light()
{
	glUseProgram(lightID);

	// ambient
	GLint ambientLightUniformLocation = glGetUniformLocation(lightID, "ambientLight");
	glm::vec3 ambientLight(a_brightness, a_brightness, a_brightness);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	// diffusion
	GLint kd = glGetUniformLocation(lightID, "coefficient_d");
	glm::vec3 vec_kd(d_brightness, d_brightness, d_brightness);
	glUniform3fv(kd, 1, &vec_kd[0]);
	// specular
	GLint ks = glGetUniformLocation(lightID, "coefficient_s");
	glm::vec3 vec_ks(s_brightness, s_brightness, s_brightness);
	glUniform3fv(ks, 1, &vec_ks[0]);
	// eye position
	GLint eyePositionUniformLocation = glGetUniformLocation(lightID, "eyePositionWorld");
	vec3 eyePosition(camera.x, camera.y, camera.z);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
	GLint lightPositionUniformLocation = glGetUniformLocation(lightID, "lightPositionWorld");
	glm::vec3 lightPosition(xLightPos, yLightPos, zLightPos);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	// light color
	GLint lightColorUniformLocation = glGetUniformLocation(lightID, "lightColor");
	glm::vec4 lightColor(1.0, 1.0, 1.0, 1.0);
	glUniform4fv(lightColorUniformLocation, 1, &lightColor[0]);
}

void set_lighting_glass()
{
	glUseProgram(glassID);

	// ambient
	GLint ambientLightUniformLocation = glGetUniformLocation(glassID, "ambientLight");
	glm::vec3 ambientLight(a_brightness, a_brightness, a_brightness);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	// diffusion
	GLint kd = glGetUniformLocation(glassID, "coefficient_d");
	glm::vec3 vec_kd(d_brightness, d_brightness, d_brightness);
	glUniform3fv(kd, 1, &vec_kd[0]);
	// specular
	GLint ks = glGetUniformLocation(glassID, "coefficient_s");
	glm::vec3 vec_ks(s_brightness, s_brightness, s_brightness);
	glUniform3fv(ks, 1, &vec_ks[0]);
	// eye position
	GLint eyePositionUniformLocation = glGetUniformLocation(glassID, "eyePositionWorld");
	vec3 eyePosition(camera.x, camera.y, camera.z);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
	GLint lightPositionUniformLocation = glGetUniformLocation(glassID, "lightPositionWorld");
	glm::vec3 lightPosition(xLightPos, yLightPos, zLightPos);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	// light color
	GLint lightColorUniformLocation = glGetUniformLocation(glassID, "lightColor");
	glm::vec4 lightColor(1.0, 1.0, 1.0, 1.0);
	glUniform4fv(lightColorUniformLocation, 1, &lightColor[0]);
}


void set_lighting_planet()
{
	glUseProgram(planetID);

	// ambient
	GLint ambientLightUniformLocation = glGetUniformLocation(planetID, "ambientLight");
	glm::vec3 ambientLight(a_brightness, a_brightness, a_brightness);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	// diffusion
	GLint kd = glGetUniformLocation(planetID, "coefficient_d");
	glm::vec3 vec_kd(d_brightness, d_brightness, d_brightness);
	glUniform3fv(kd, 1, &vec_kd[0]);
	// specular
	GLint ks = glGetUniformLocation(planetID, "coefficient_s");
	glm::vec3 vec_ks(s_brightness, s_brightness, s_brightness);
	glUniform3fv(ks, 1, &vec_ks[0]);
	// eye position
	GLint eyePositionUniformLocation = glGetUniformLocation(planetID, "eyePositionWorld");
	vec3 eyePosition(camera.x, camera.y, camera.z);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
	GLint lightPositionUniformLocation = glGetUniformLocation(planetID, "lightPositionWorld");
	glm::vec3 lightPosition(xLightPos, yLightPos, zLightPos);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	// light color
	GLint lightColorUniformLocation = glGetUniformLocation(planetID, "lightColor");
	glm::vec4 lightColor(1.0, 1.0, 1.0, 1.0);
	glUniform4fv(lightColorUniformLocation, 1, &lightColor[0]);
}

void set_Fog() {
	glUseProgram(programID);
	GLint programFog = glGetUniformLocation(programID, "FogFlag");
	GLint FogDensity = glGetUniformLocation(programID, "FogDensity");
	GLint FogGradient = glGetUniformLocation(programID, "FogGradient");
	glUniform1f(programFog, 1);
	glUniform1f(FogDensity,fogD);
	glUniform1f(FogGradient,fogG);

	glUseProgram(glassID);
	GLint glassFog = glGetUniformLocation(glassID, "FogFlag");
	FogDensity = glGetUniformLocation(glassID, "FogDensity");
	FogGradient = glGetUniformLocation(glassID, "FogGradient");
	glUniform1f(glassFog, 1);
	glUniform1f(FogDensity, 0.05);
	glUniform1f(FogGradient, 3.5);

	glUseProgram(planetID);
	GLint planetFog = glGetUniformLocation(planetID, "FogFlag");
	FogDensity = glGetUniformLocation(planetID, "FogDensity");
	FogGradient = glGetUniformLocation(planetID, "FogGradient");
	glUniform1f(planetFog, 1);
	glUniform1f(FogDensity, 0.05);
	glUniform1f(FogGradient, 3.5);

	glUseProgram(skyboxID);
	GLint skyboxFog = glGetUniformLocation(skyboxID, "FogFlag");
	FogDensity = glGetUniformLocation(skyboxID, "FogDensity");
	FogGradient = glGetUniformLocation(skyboxID, "FogGradient");
	glUniform1f(skyboxFog, 0);
	glUniform1f(FogDensity, 0.05);
	glUniform1f(FogGradient, 3.5);


}

void drawcube() {

	//cube
	GLfloat scale_fact = 50.0f;

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
	GLfloat scale_fact = 2.0f;

	glUseProgram(planetID);

	glBindVertexArray(earthVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(earth_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(planetID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(planetID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(planetID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID_0 = glGetUniformLocation(planetID, "myTextureSampler");
	GLuint TextureID_1 = glGetUniformLocation(planetID, "myTextureSampler_normal");
	//enable texture
	GLuint TextureEnable = glGetUniformLocation(planetID, "normalEnable");
	glUniform1i(TextureEnable,1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureEarth[0]);
	glUniform1i(TextureID_0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureEarth[1]);
	glUniform1i(TextureID_1, 1);

	glDrawArrays(GL_TRIANGLES, 0, drawEarthSize);
}

void drawGlass(void)
{
	GLfloat scale_fact = 0.75f;

	glUseProgram(glassID);

	glBindVertexArray(glassVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(glass_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -7.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(glassID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(glassID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(glassID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);

	// texture
	GLuint TextureID_0 = glGetUniformLocation(glassID, "myTextureSampler");
	GLuint TextureID_1 = glGetUniformLocation(glassID, "myTextureSampler_1");
	//disable texture
	GLuint TextureEnable = glGetUniformLocation(glassID, "normalEnable");
	glUniform1i(TextureEnable, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureGlass[0]);
	glUniform1i(TextureID_0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureGlass[1]);
	glUniform1i(TextureID_1, 1);

	glDrawArrays(GL_TRIANGLES, 0, drawGlassSize);
}

void drawLightCube() {

	//cube
	GLfloat scale_fact = 2.0f;

	glUseProgram(lightID);
	//skybox cube
	glBindVertexArray(cubeVao);

	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(light_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(lightID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(lightID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(lightID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);
	// texture

	glDrawArrays(GL_TRIANGLES, 0, drawCubeSize);
	glBindVertexArray(0);
}
void drawMoon(void)
{
	GLfloat scale_fact = 0.3f;

	glUseProgram(programID);

	glBindVertexArray(moonVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	//local rotation
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(moon_innRot_Degree), glm::vec3(0, 1, 0)); 
	//glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-19.0f + moon_tranX, 0.75f + moon_tranY, 0.0f));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-19.0f, 0.75f, 0.0f)); //moon init position
	//global rotation
	trans_M *= glm::translate(glm::mat4(), glm::vec3(moon_orbit_radius, 0.0f, 0.0f)); 
	trans_M *= glm::rotate(glm::mat4(), moon_outnRot_Degree, glm::vec3(0, 1, 0));
	trans_M *= glm::translate(glm::mat4(), glm::vec3(-moon_orbit_radius, 0.0f, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);
	//enable texture
	GLuint TextureEnable = glGetUniformLocation(programID, "normalEnable");
	glUniform1i(TextureEnable, 0);
	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureMoon);
	glUniform1i(TextureID, 0);
	glActiveTexture(GL_TEXTURE1);

	glDrawArrays(GL_TRIANGLES, 0, drawMoonSize);
}

void drawCar(void)
{
	//car
	GLfloat scale_fact = 0.1f;

	glUseProgram(programID);

	glBindVertexArray(carVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), 89.0f, glm::vec3(0, 1, 1));
	//init position
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f + car_orbit_radius, 2.0f+car_orbit_radius, 0.0f));
	trans_M *= glm::translate(glm::mat4(), glm::vec3(-car_orbit_radius, -car_orbit_radius, 0.0f));
	trans_M *= glm::rotate(glm::mat4(), car_outnRot_Degree, glm::vec3(0, 0, 1));
	trans_M *= glm::translate(glm::mat4(), glm::vec3(car_orbit_radius, car_orbit_radius, 0.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;
	carPos.x = trans_M[3][0]+1.0;
	carPos.y = trans_M[3][1];
	carPos.z = trans_M[3][2];
	carFront = carPos - lastCarPos;
	lastCarPos = carPos;


	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);
	//enable texture
	GLuint TextureEnable = glGetUniformLocation(programID, "normalEnable");
	glUniform1i(TextureEnable, 0);
	// texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureCar);
	glUniform1i(TextureID, 0);
	glActiveTexture(GL_TEXTURE1);

	glDrawArrays(GL_TRIANGLES, 0, drawCarSize);
}

void drawRock(void) {
	GLfloat scale_fact = 0.75f;
	glUseProgram(planetID);

	glBindVertexArray(rockVao);
	glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact));
	glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(rock_innRot_Degree), glm::vec3(0, 1, 0));
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 Model = trans_M * rot_M * scale_M;

	GLint M_ID = glGetUniformLocation(planetID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(planetID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(planetID, "PM");
	glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);
	//enable texture
	GLuint TextureEnable = glGetUniformLocation(programID, "normalEnable");
	glUniform1i(TextureEnable, 1);
	// texture
	GLuint TextureID_0 = glGetUniformLocation(planetID, "myTextureSampler");
	GLuint TextureID_1 = glGetUniformLocation(planetID, "myTextureSampler_normal");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureRock[0]);
	glUniform1i(TextureID_0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureRock[1]);
	glUniform1i(TextureID_1, 1);

	//glDrawArrays(GL_TRIANGLES, 0, drawRockSize);
}

void drawRing() {
	GLint modelTransformMatrixUniformLocation;
	glm::mat4 rockOrbitIni = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -8.0f, 0.0f));
	glm::mat4 rockOrbit_M = glm::rotate(rockOrbitIni, rock_outnRot_Degree, glm::vec3(0, 1, 0));
	for (GLuint i = 0; i < asteroidAmount; i++) {
		rockModelMat_temp = asteroidMatrices[i];
		rockModelMat_temp = rockOrbit_M * rockModelMat_temp;
		GLint M_ID = glGetUniformLocation(planetID, "MM");
		glUniformMatrix4fv(M_ID, 1, GL_FALSE, &rockModelMat_temp[0][0]);
		GLint V_ID = glGetUniformLocation(planetID, "VM");
		glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
		GLint P_ID = glGetUniformLocation(planetID, "PM");
		glUniformMatrix4fv(P_ID, 1, GL_FALSE, &common_projection[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, drawRockSize);
		//glDrawArraysInstanced(GL_TRIANGLES, 0, drawRockSize, asteroidAmount);
	}
	//glDrawArraysInstanced(GL_TRIANGLES, 0, drawRockSize, asteroidAmount);
}

void drawStar() {
	GLfloat scale_fact[starAmount] = { 0.2f,0.18f,0.16f,0.14f,0.12f };

	glUseProgram(programID);

	glBindVertexArray(starVao);
	for (GLuint i = 0; i < starAmount; i++) {
		glm::mat4 scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale_fact[i]));
		//local rotation
		glm::mat4 rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0, 1, 0));
		glm::mat4 trans_M;
		trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(carPosArr[i]));
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
		glBindTexture(GL_TEXTURE_2D, TextureStar);
		glUniform1i(TextureID, 0);
		glActiveTexture(GL_TEXTURE1);
		glDrawArrays(GL_TRIANGLES, 0, drawStarSize);
	}
}

void paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// ================================ //
	// view matrix
	common_viewM = glm::lookAt(camera, cameraLookAt, cameraNorm);
	// projection matrix
	common_projection = glm::perspective(camera_fov, 16.0f/9.0f, 0.1f, 200.0f);
	
	//=== draw ===//
	// set lighting parameters
	set_Fog();
	set_lighting();
	set_lighting_light();
	set_lighting_planet();
	set_lighting_glass();
	glEnable(GL_LIGHTING);
	//skybox
	glDepthMask(GL_FALSE);
	drawcube();
	glDepthMask(GL_TRUE);
	// draw Planet A
	drawEarth();
	// draw Planet B
	drawMoon();
	//draw Planet C
	drawGlass();
	//draw space vehicle
	drawCar();
	//draw light cube
	drawLightCube();
	//draw rock
	drawRock();
	drawRing();
	//draw trajectory
	drawStar();
	drawLightCube();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Shaders_Installer()
{
	programID = installShaders("shader/Common.vs", "shader/Common.frag");
	skyboxID = installShaders("skybox/skybox.vs", "skybox/skybox.frag");
	lightID = installShaders("light/lightShader.vs", "light/lightShader.frag");
	glassID = installShaders("shader/Common.vs", "shader/Glass.frag");
	planetID = installShaders("shader/Common.vs", "shader/Planet.frag");
	if (!checkProgramStatus(programID)){
		return;
	}
	if (!checkProgramStatus(skyboxID)) {
		return;
	}
	if (!checkProgramStatus(lightID)) {
		return;
	}
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

/*
void moonOrbitFunc() {
	float radius = 0.8f;
	moon_outnRot_Degree += 0.1f;
	if (moon_outnRot_Degree > 360.0f)
	{
		moon_outnRot_Degree = 0.0f;
	}
	moon_tranX += radius * sinf(moon_outnRot_Degree);
	moon_tranY -= radius * cosf(moon_outnRot_Degree);
}
*/

void recordCarPos(vec3 carPos) {
	for (carPosIndex = 0; carPosIndex < starAmount; carPosIndex++) {
		carPosArr[carPosIndex] = carPos;
	}
}

void timerFunction(int id)
{
	earth_innRot_Degree += 0.3;
	glass_innRot_Degree += 0.5;
	moon_innRot_Degree += 1.0;
	//light_innRot_Degree += 0.3;
	rock_outnRot_Degree -= 0.01;
	car_outnRot_Degree += car_orbit_speed/100;
	light_innRot_Degree += 0.07;
	rock_innRot_Degree += 0.3;
	moon_outnRot_Degree += 0.01; //moon rotation speed
	//moon orbit along earth
	//moonOrbitFunc();
	//space vehicle orbit along earth
	if (cameraOnCar) {
		camera = carPos;
		cameraLookAt = carFront;
		cameraNorm = vec3(0, glm::normalize(carPos.y), 0);
	}
	glutPostRedisplay();
	glutTimerFunc(700.0f / 60.0f, timerFunction, 1);
}

void starTimeFunction(int id) {
	starPos = lastCarPos;
	if (starPos.y <= 0) {
		starPos.x -= 1.0f;
	}
		
	if (starPos.x >= 0) {
		starPos.y -= 2.0f;
	}
	else {
		starPos.y -= 1.0f;
	}
	//recordCarPos(starPos);
	carPosArr[4] = carPosArr[3];
	carPosArr[3] = carPosArr[2];
	carPosArr[2] = carPosArr[1];
	carPosArr[1] = carPosArr[0];
	carPosArr[0] = starPos;

	glutPostRedisplay();
	glutTimerFunc(10000.0f / 60.0f, starTimeFunction, 2);

}

void WindowSize(GLint width, GLint height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	int w = height * ASPECT;           // w is width adjusted for aspect ratio
	int left = (width - w) / 2;
	glViewport(left, 0, w, height);       // fix up the viewport to maintain aspect ratio
	gluOrtho2D(0, 600, 800, 0);   // only the window is changing, not the camera
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}


void controlCb(int control){
	if (control == 1) { //viewpoint control
		if (viewpointGroup == 0) { // left 
			if (allowCameraMove) {
				tempCamera = camera;
			}
			allowCameraMove = false;
			camera = vec3(-50.0f, 0.0f, 0.0f);
			cameraLookAt = glm::vec3(0, 0, 0);
			cameraNorm = glm::vec3(0, 1, 0);
			cameraOnCar = false;
		}
		else if (viewpointGroup == 1) { // top
			if (allowCameraMove) {
				tempCamera = camera;
			}
			allowCameraMove = false;
			camera = vec3(0.0f, 30.0f, -3.5f);
			cameraLookAt = glm::vec3(0, 0, 0);
			cameraNorm = glm::vec3(0, 1, 0);
			cameraOnCar = false;
		}
		else if (viewpointGroup == 2) { // front
			if (allowCameraMove) {
				tempCamera = camera;
			}
			allowCameraMove = false;
			camera = vec3(0.0f, 0.0f, 30.0f);
			cameraLookAt = glm::vec3(0, 0, 0);
			cameraNorm = glm::vec3(0, 1, 0);
			cameraOnCar = false;
		}
		else if (viewpointGroup == 3) { // free
			if (!allowCameraMove) {
				camera = tempCamera;
				cameraLookAt = glm::vec3(0, 0, 0);
				cameraNorm = glm::vec3(0, 1, 0);
			}
			allowCameraMove = true;
			cameraOnCar = false;
		}
		else if (viewpointGroup == 4) { // car
			if (allowCameraMove) {
				tempCamera = camera;
			}
			allowCameraMove = false;
			cameraOnCar = true;
		}
	}
	else if (control == 2 && fogEffectOnOff == 1) { //fog control
		if (fogColourGroup == 0) {

		}
	}
}

void gluiInt(GLuint mainWindow) {
	glui = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_RIGHT);
	glui->set_main_gfx_window(mainWindow);
	glui->add_separator();
	GLUI_StaticText *infoText = glui->add_statictext("Universe Exploration");
	infoText->set_alignment(GLUI_ALIGN_CENTER);
	glui->add_separator();
	//control vehicle speed
	GLUI_Spinner *vehicleSpeed = glui->add_spinner("Vehicle Speed", GLUI_SPINNER_FLOAT, &car_orbit_speed);
	vehicleSpeed->set_float_limits(2.0f,30.0f, GLUI_LIMIT_CLAMP);
	vehicleSpeed->set_speed(0.5f);
	glui->add_separator();
	//define panel
	GLUI_Rollout *renderControl = glui->add_rollout("Render Control");
	GLUI_Panel *vpp = glui->add_panel_to_panel(renderControl, "View Point");
	GLUI_Panel *fcp = glui->add_panel_to_panel(renderControl, "Fog Colour");
	GLUI_Panel *fp = glui->add_panel_to_panel(renderControl, "Fog On/Off");
	//radio group for viewpoint
	viewPoint = glui->add_radiogroup_to_panel(vpp, &viewpointGroup,1,controlCb);
		glui->add_radiobutton_to_group(viewPoint, "left"); //0
		glui->add_radiobutton_to_group(viewPoint, "top"); //1
		glui->add_radiobutton_to_group(viewPoint, "front"); //2
		glui->add_radiobutton_to_group(viewPoint, "free"); //3
		glui->add_radiobutton_to_group(viewPoint, "car"); //4

		viewPoint->set_int_val(3);//initial free view
	fogColour = glui->add_radiogroup_to_panel(fcp, &fogColourGroup,2,controlCb);
		glui->add_radiobutton_to_group(fogColour, "red");
		glui->add_radiobutton_to_group(fogColour, "yellow");
		glui->add_radiobutton_to_group(fogColour, "blue");
	GLUI_Checkbox *fogOnOff = glui->add_checkbox_to_panel(fp, "Fog",&fogEffectOnOff);
}

int main(int argc, char *argv[])
{
	//background music
	ISoundEngine* engine = createIrrKlangDevice(); //can ignore the error
	engine->play2D("media/getout.ogg", true);

	/*Initialization of GLUT library*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	int height = GetSystemMetrics(SM_CYSCREEN)*0.8;
	int width = GetSystemMetrics(SM_CYSCREEN)*1.42;

	glutInitWindowSize(width,height);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	GLuint mainWindow = glutCreateWindow(argv[0]);
	gluiInt(mainWindow);
	// initialize openGL
	initializedGL();
	// draw
	glutDisplayFunc(paintGL);
	//user input function
	glutMouseFunc(Mouse_Wheel_Func);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(move);
	glutPassiveMotionFunc(PassiveMouse);

	glutTimerFunc(700.0f / 60.0f, timerFunction, 1);
	glutTimerFunc(10000.0f / 60.0f, starTimeFunction, 2);

	//keep aspect ratio
	glutReshapeFunc(WindowSize);

	/*Enter the GLUT event processing loop which never returns.*/
	glutMainLoop();
	engine->drop(); // delete engine

	return 0;
}