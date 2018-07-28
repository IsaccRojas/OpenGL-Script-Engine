#pragma once

#ifndef ENTITY_H_
#define ENTITY_H_

#include "../tools.h"
#include <vector>
#include <string>

#ifdef LUA_C
#include <lua.c>
#endif

#ifdef ENTBASE
#include "EntBase.h"
#endif



class EBuffer;

class Entity {
	float clock;
	int priority;
	float X, Y, W, H, toX, toY, tW, tH, U, V, UVW, UVH;
#ifdef LUA_C
	std::string script_filename;
	lua_State* L;
#endif
	Image tex;
	std::string ent_name;
	int num_globals;
public:
	int CONST_ZERO;
	int index;
	EBuffer* master;
#ifdef LUA_C
	std::vector<std::string> g_keys;
	std::vector<std::string> data_keys;
	std::vector<float> data_table;
	std::vector<float> g_vals;
#endif
	
	static unsigned const int indicesperentityv;
	static unsigned const int indicesperentitye;
	static unsigned int count;
	static unsigned int count_existed;
	bool index_forced;
	bool bChangeSinceLastCheck;
	bool bPriorityChange;
	bool bFree;
	bool bTextured;
	bool bKill;
	unsigned int ID;
	std::vector<float> vVertices;
	std::vector<unsigned int> vElements;
	/*
	(x, y): physical center position
	(w, h): physical dimensions
	(tox, toy): visuals' offset from physical center
	(tw, th): visuals' dimensions
	(u, v): texture position
	(uvw, uvh): texture pixel dimensions
	*/
	Entity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image imgTex, int prior);
#ifdef ENTBASE
	Entity(float x, float y, float w, float h, Packet pack, int prior);
	Entity(const char* entity_name);
#endif
	Entity();
	~Entity();
	/* data must be size 16 */
	void dataNew(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image imgTex, int prior);
	//void dataNew(float x, float y, float w, float h, Packet pack, int prior);
#ifdef ENTBASE
	void dataSetPacket(Packet pack);
#endif
#ifdef LUA_C
	void dataSetScript(const char* filename);
#endif
	void dataSetPos(float x, float y);
	void dataSetDimensions(float w, float h);
	void dataSetTexOffset(float tox, float toy);
	void dataSetTexDimensions(float tw, float th);
	void dataSetUV(float u, float v);
	void dataSetUVDimensions(float uvw, float uvh);
	void dataReset();
	void dataFree();
	unsigned int getID();

	vec2 dataGetPos();
	vec2 dataGetDimensions();
	vec2 dataGetTexOffset();
	vec2 dataGetTexDimensions();
	vec2 dataGetUV();
	vec2 dataGetUVDimensions();
	float getClock();
	void incClock();
	void setClock(float t);
	int getPriority();
	void setPriority(int p);
	void Draw(unsigned int vbuff_handle, unsigned int vebuff_handle, bool reset_data);
#ifdef LUA_C
	int script();
	int load_g_script();
	int routine();
#endif
};

class EntPointer {
	Entity* ent;
public:
	EntPointer(Entity* entity);
	EntPointer();
	void operator=(Entity* entity);
	void operator=(EntPointer entptr);

	Entity* gent();
	unsigned int gid();
	vec2 gpos();
	void spos(float x, float y);
	vec2 gdim();
	void sdim(float w, float h);
	vec2 gtoff();
	void stoff(float tox, float toy);
	vec2 gtdim();
	void stdim(float tw, float th);
	vec2 guvpos();
	void suvpos(float u, float v);
	vec2 guvdim();
	void suvdim(float uvw, float uvh);
	float gclock();
	void sclock(float t);
	int gprior();
	void sprior(int p);
};

#endif