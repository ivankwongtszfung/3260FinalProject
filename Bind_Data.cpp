#include "Bind_Data.h"
#include "Loading_func.h"

GLuint earthVao;
int drawEarthSize = 0;

GLuint moonVao;
int drawMoonSize = 0;

GLuint carVao;
int drawCarSize = 0;

GLuint glassVao;
int drawGlassSize = 0;

void bindEarth(const char * path)
{
	//Earth data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n);

	//sending earth data
	glGenVertexArrays(1, &earthVao);
	glBindVertexArray(earthVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawEarthSize = vao5_v.size();
}

void bindGlass(const char * path)
{
	//Glass data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n);

	//sending glass data
	glGenVertexArrays(1, &glassVao);
	glBindVertexArray(glassVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawGlassSize = vao5_v.size();
}

void bindMoon(const char * path)
{
	//Moon data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n);

	//sending moon data
	glGenVertexArrays(1, &moonVao);
	glBindVertexArray(moonVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawMoonSize = vao5_v.size();
}

void bindCar(const char * path)
{
	//Car data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n);

	//sending car data
	glGenVertexArrays(1, &carVao);
	glBindVertexArray(carVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawCarSize = vao5_v.size();
}
