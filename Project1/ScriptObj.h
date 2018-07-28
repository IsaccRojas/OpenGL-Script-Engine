#pragma once

#ifndef BASESCRIPT_H_
#define BASESCRIPT_H_

#include "Resources.h"
#include "EBuffer/EBuffer.h"

class ScriptObj {
protected:
	bool first_exec;
public:
	ScriptObj();
	virtual void first();
	virtual void base_script();
};

#endif