#pragma once

#ifndef SOBASESCRIPT_H_
#define SOBASESCRIPT_H_

#include "ScriptObj.h"
#include "SOEntDataBase.h"

#include <chrono>

/*
INDEX 0: PLAYER
*/

class SOBaseScript : public ScriptObj {
	std::chrono::steady_clock::time_point oldt;
	std::chrono::steady_clock::time_point newt;

	SOEntDataBase DB;

	EBuffer* TBuff;
	GLBuffer<float>* TVertBuf;
	GLBuffer<unsigned>* TElBuf;
	EBuffer* EBuff;
	GLBuffer<float>* VertBuf;
	GLBuffer<unsigned>* ElBuf;

	Resources* res;
	Image textures[16];
	
	std::vector<SOEnt*> killed_entities;
	int frame, cursorx, cursory;
public:
	int F;
	SOBaseScript(Resources* resources, int *err);

	Resources getResources();
	std::vector<SOEnt*> entities;
	std::vector<SOEnt*> tiles;

	void render(unsigned vbo, unsigned vebo);
	void base_script() override;

	Entity* EBuffAlloc(Packet p, Image imgTex, int prior);
	int EBuffDealloc(Entity* Ent);
	SOEnt* gen_ent(std::string name);
	void del_ent(SOEnt* soentity);
	Image getTexture(int index);
	float get_fps();

	void set_map(float* tilepos);

	static int l_gen_ent(lua_State* L);
	static int l_set_kill(lua_State* L);
	static int l_get_kill(lua_State* L);
	static int l_get_master(lua_State* L); /* FIXME: incomplete */
	static int l_get_ent(lua_State* L);
	static int l_get_ent_all_t(lua_State* L); //TODO
	static int l_get_ent_all_tn(lua_State* L);
	static int l_get_ent_type(lua_State* L);
	static int l_get_entcount(lua_State* L);
	static int l_get_input(lua_State* L);

	static int l_getpos(lua_State* L);
	static int l_getdim(lua_State* L);
	static int l_gettoff(lua_State* L);
	static int l_gettdim(lua_State* L);
	static int l_getuv(lua_State* L);
	static int l_getuvdim(lua_State* L);
	static int l_getdatas(lua_State* L);
	static int l_getdatai(lua_State* L);

	static int l_setpos(lua_State* L);
	static int l_setdim(lua_State* L);
	static int l_settoff(lua_State* L);
	static int l_settdim(lua_State* L);
	static int l_setuv(lua_State* L);
	static int l_setuvdim(lua_State* L);
	static int l_setdatas(lua_State* L);
	static int l_setdatai(lua_State* L);
	static int l_setframe(lua_State* L);
	static int l_settext(lua_State* L);
};

#endif