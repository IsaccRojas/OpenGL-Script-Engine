#include "SOPlayer.h"
#include "SOBaseScript.h"

SOPlayer::SOPlayer(EntData EData, Image texture, SOBaseScript* master_ptr) : SOEnt(EData, texture, master_ptr) {}

void SOPlayer::base_script() {
	lua_pushlightuserdata(master->L, (void*)this);
	lua_setglobal(master->L, "this");
	lua_getglobal(master->L, name.c_str());
	lua_pcall(master->L, 0, 0, 0);
}

void SOPlayer::run(Abstr_Dispatcher &dispatcher) {
	dispatcher.dispatch(*this);
}