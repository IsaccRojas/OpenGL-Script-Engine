#include "ScriptObj.h"

ScriptObj::ScriptObj() {
	first_exec = true;
}
void ScriptObj::first() {
	first_exec = false;
}
void ScriptObj::base_script() {
	if (first_exec)
		first();
}