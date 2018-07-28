#pragma once
#include "SOBaseScript.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

int mloop(Resources* res);

int setup(Resources* res, float render_unit_size);

#endif MAINLOOP_H_