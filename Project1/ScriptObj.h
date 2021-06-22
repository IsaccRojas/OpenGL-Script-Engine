#pragma once

#ifndef BASESCRIPT_H_
#define BASESCRIPT_H_

#include "Resources.h"
#include "EBuffer/EBuffer.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <boost/dll/config.hpp>
#include <boost/dll.hpp>

//empty base class for "Scriptable Objects", which preserve a runnable lua state that can be called via base_script()
class ScriptObj {
public:
	lua_State * L;
	ScriptObj();
	virtual void base_script();
};

#endif