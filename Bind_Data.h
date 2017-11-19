#ifndef  __BIND_DATA_H__
#define  __BIND_DATA_H__

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <vector>

void bindEarth(const char * path);
void bindGlass(const char * path);
void bindMoon(const char * path);
void bindCar(const char * path);
void bindCube();

/*** END OF FILE ***/

#endif /* __BIND_DATA_H__ */