#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\gtc\type_ptr.hpp"
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
// ============================= window conf =============================//
int Win_w, Win_h;
float camera_fov = 45.0;
const float ASPECT = float(16) / 9;

int  mainWindowID;
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

//Space vehicle
extern GLuint carVao;
extern int drawCarSize;
GLuint TextureCar;
float car_outnRot_Degree = 0.0f;
float car_orbit_radius = 8.0f;
float car_orbit_speed = 0.02f;
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
static glm::mat4 lightPositionMat;
float xLightPos = 0.0f;
float zLightPos = 0.0f;
float yLightPos = 20.0f;

//rock
extern GLuint rockVao;
extern int drawRockSize;
GLuint TextureRock[2];
float rock_innRot_Degree = 0.0f;

//asteroidRing
const int asteroidAmount = 10;
extern glm::mat4 asteroidMatrices[asteroidAmount];
glm::mat4 rockModelMat_temp;

// ============================= camera conf =============================//
const float M_PI = 3.14159265;
float distanceFromCenter = 30.0f;
float pitch = 0.0f;
float angleAroundCenter = 0.0f;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = distanceFromCenter;

static float old_x = 0.0f, old_y = 0.0f;

// ============================= lighting conf =============================//
float a_brightness = 1.0f;
float d_brightness = 0.0f;
float s_brightness = 0.6f;

// ============================= User Input Function =============================//
void cameraPosition() {
	cameraY = distanceFromCenter * sin(glm::radians(pitch));
	cameraX = -distanceFromCenter * cos(glm::radians(pitch)) * sin(glm::radians(angleAroundCenter));
	cameraZ = -distanceFromCenter * cos(glm::radians(pitch)) * cos(glm::radians(angleAroundCenter));
}


void Mouse_Wheel_Func(int button, int state, int x, int y)
{
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

void keyboard(unsigned char key, int x, int y)
{
	//change viewpoint
	if (key == 'a') {
		cameraX = -50.0f;
		cameraY = 0.0f;
		cameraZ = 0.0f;
	}
	else if (key == 's') {
		cameraX = 0.0f;
		cameraY = 49.6f;
		cameraZ = 30.0f;
	}
	else if (key == 'd') {
		printf("press d");
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
		xLightPos += 1.1f;
		printf("%.3lf\n", xLightPos);
	}
	else if (key == 'z') {
		xLightPos -= 1.1f;
		printf("%.3lf\n", xLightPos);
	}
	else if (key == 'w') {
		yLightPos += 1.1f;
		printf("%.3lf\n", yLightPos);
	}
	else if (key == 'x') {
		yLightPos -= 1.1f;
		printf("%.3lf\n", yLightPos);
	}
	else if (key == 'e') {
		zLightPos += 1.1f;
		printf("%.3lf\n", zLightPos);
	}
	else if (key == 'c') {
		zLightPos -= 1.1f;
		printf("%.3lf\n", zLightPos);
	}

}

void move(int key, int x, int y)
{
	//Real-time speed and orbit control
	//speed of vehicle
	if (key == GLUT_KEY_UP) {
		if (car_orbit_speed < 0.3)
			car_orbit_speed += 0.01;
	}
	else if (key == GLUT_KEY_DOWN) {
		if (car_orbit_speed > 0.02)
			car_orbit_speed -= 0.01;
	}
	//orbit radius size
	else if (key == GLUT_KEY_LEFT) {
		car_orbit_radius += 0.5f;
	}
	else if (key == GLUT_KEY_RIGHT) {
		if (car_orbit_radius > 5.5f)
			car_orbit_radius -= 0.5f;
	}

}

void PassiveMouse(int x, int y)
{
	float dety = y - old_y;
	float detx = x - old_x;
	pitch += dety / 10;
	angleAroundCenter -= detx / 10;
	cameraPosition();
	old_x = x;
	old_y = y;
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
	vec3 eyePosition(cameraX, cameraY, cameraZ);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
/*	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glm::vec3 lightPosition(lightPositionMat.xyz);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0])*/;

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
	vec3 eyePosition(cameraX, cameraY, cameraZ);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	// light position
	GLint lightPositionUniformLocation = glGetUniformLocation(lightID, "lightPositionWorld");
	glm::vec3 lightPosition(0.0, yLightPos, zLightPos);
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
	vec3 eyePosition(cameraX, cameraY, cameraZ);
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
	vec3 eyePosition(cameraX, cameraY, cameraZ);
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
	GLuint TextureID_1 = glGetUniformLocation(planetID, "myTextureSampler_1");

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

	trans_M *= glm::translate(glm::mat4(), glm::vec3(20.0f, 0.0f, 0.0f));
	trans_M *= glm::rotate(glm::mat4(), light_innRot_Degree, glm::vec3(0, 1, 0));
	trans_M *= glm::translate(glm::mat4(), glm::vec3(-20.0f, 0.0f, 0.0f));

	

	glm::mat4 Model = trans_M * rot_M * scale_M;

	lightPositionMat = Model;

	GLint M_ID = glGetUniformLocation(programID, "MM");
	glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
	GLint V_ID = glGetUniformLocation(programID, "VM");
	glUniformMatrix4fv(V_ID, 1, GL_FALSE, &common_viewM[0][0]);
	GLint P_ID = glGetUniformLocation(programID, "PM");
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
	glm::mat4 trans_M = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f + car_orbit_radius, 2.0f + car_orbit_radius, 0.0f));
	trans_M *= glm::translate(glm::mat4(), glm::vec3(-car_orbit_radius, -car_orbit_radius, 0.0f));
	trans_M *= glm::rotate(glm::mat4(), car_outnRot_Degree, glm::vec3(0, 0, 1));
	trans_M *= glm::translate(glm::mat4(), glm::vec3(car_orbit_radius, car_orbit_radius, 0.0f));
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

	// texture
	GLuint TextureID_0 = glGetUniformLocation(planetID, "myTextureSampler");
	GLuint TextureID_1 = glGetUniformLocation(planetID, "myTextureSampler_1");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureRock[0]);
	glUniform1i(TextureID_0, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureRock[1]);
	glUniform1i(TextureID_1, 1);

	glDrawArrays(GL_TRIANGLES, 0, drawRockSize);
}

void drawRing() {
	GLint modelTransformMatrixUniformLocation;
	glm::mat4 rockOrbitIni = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -7.0f, 0.0f));
	glm::mat4 rockOrbit_M = glm::rotate(rockOrbitIni, 89.0f, glm::vec3(0, 1, 0));
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
	}
}

void paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	// ================================ //
	// view matrix
	common_viewM = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	// projection matrix
	common_projection = glm::perspective(camera_fov, 16.0f / 9.0f, 0.1f, 200.0f);

	//=== draw ===//
	// set lighting parameters
	set_lighting();
	set_lighting_glass();
	set_lighting_planet();
	//set_lighting_light();
	glEnable(GL_LIGHTING);
	//skybox

	drawcube();
	glDepthMask(GL_TRUE);
	// draw Planet A
	set_lighting();
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
	if (!checkProgramStatus(programID)) {
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

void timerFunction(int id)
{
	//earth_innRot_Degree += 0.3;
	glass_innRot_Degree += 0.5;
	moon_innRot_Degree += 1.0;
	light_innRot_Degree += 0.05;
	rock_innRot_Degree += 0.3;
	car_outnRot_Degree += car_orbit_speed;
	moon_outnRot_Degree += 0.01; //moon rotation speed
								 //moon orbit along earth
								 //moonOrbitFunc();
								 //space vehicle orbit along earth


	glutPostRedisplay();
	glutTimerFunc(700.0f / 60.0f, timerFunction, 1);
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

int main(int argc, char *argv[])
{
	/*Initialization of GLUT library*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	int height = GetSystemMetrics(SM_CYSCREEN)*0.8;
	int width = GetSystemMetrics(SM_CYSCREEN)*1.42;

	glutInitWindowSize(width, height);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	mainWindowID = glutCreateWindow("i-Navigation");

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

	//keep aspect ratio
	glutReshapeFunc(WindowSize);

	/*Enter the GLUT event processing loop which never returns.*/
	glutMainLoop();

	return 0;
}