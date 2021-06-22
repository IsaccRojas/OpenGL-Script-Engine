#pragma once

#undef INCLUDES_H_
#ifndef INCLUDES_H_
#define INCLUDES_H_

//collection of STL, OpenGL, SDL2, and Lua headers

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <GL/glew.h>
#include <SOIL.h>
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include <ctime>

#endif