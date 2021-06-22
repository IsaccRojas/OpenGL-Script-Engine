#pragma once
#include "SOBaseScript.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

//main loop of program, using allocated Resources object
int mloop(Resources* res);

//main initialization of program to be called before mloop(), using allocated Resources object
int setup(Resources* res);

#endif MAINLOOP_H_