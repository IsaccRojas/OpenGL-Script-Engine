#pragma once

#ifndef EBUFFER_H_
#define EBUFFER_H_

#include "Entity.h"
#include <list>
#include <iostream>

class EBuffer {
	unsigned int count, index_offset;
	int highest_priority, lowest_priority;
	unsigned int EBuff_ID;
#ifdef ENTBASE
	EntBase* EBase;
#endif

	std::vector<Entity*> vFreeEnts;
	Entity* alloc(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image imgTex, int prior);
public:
	static unsigned int count_existed;
	unsigned int vb_handle, eb_handle, position_loc, tex_loc;
	std::list<Entity> vEnts;
	std::vector<unsigned int> vElements;
	std::vector<float> vData;
	Image imgTexture;

	/* set vb_location and/or eb_location to -2 to generate a buffer */
	EBuffer(unsigned int position_attribute_location, unsigned int texture_attribute_location, unsigned int vb_location, unsigned int eb_location, Image image);

	/*
	(x, y): physical center position
	(w, h): physical dimensions
	(tox, toy): visuals' offset from physical center
	(tw, th): visuals' dimensions
	(u, v): texture position
	(uvw, uvh): texture pixel dimensions
	script_name: name of script entity will execute
	imgTex: Image object containing texture image information
	*/
	Entity* allocateEntity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image imgTex, int prior);
#ifdef ENTBASE
	Entity* allocateEntity(const char* name, float x, float y, float w, float h, int prior);
	Entity* allocateEntity(unsigned int ebase_index, float x, float y, float w, float h, int prior);
#endif
	void allocateEntity();

	Entity* newEntity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image imgTex, int prior);
#ifdef ENTBASE
	Entity* newEntity(const char* name, float x, float y, float w, float h, int prior);
	Entity* newEntity(unsigned int ebase_index, float x, float y, float w, float h, int prior);
#endif

	Entity* getEnt(unsigned int p_ID);
	unsigned int getEBuff_ID();
#ifdef ENTBASE
	void setEntBase(EntBase* ent_base);
	EntBase* getEntBasePtr();
#endif
	void update();
	void erase(Entity* ent);
	/* sets buffer to a concatenation of all entities' data; set dir to 1:increased, -1:decreased, 0:total_reset; set position to -1 if increase/decrease occurred at end of vector (ignored if dir == 0) */
	void dataResizeReset(int dir, int position);
	/* executes all entities' routines */
#ifdef LUA_C
	void dataRoutine();
#endif
	/* sets buffer sub data based on whether calls were made to the dataSet* methods of internal entities or not */
	void dataCheck();
	void dataOrder();

	void Draw(bool reset_data);

	void deallocateEntity(unsigned int p_ID);
	void deallocateEntity(Entity* ent); /* FIXME: incomplete */
	void freeEntity(Entity* ent);
	void reportIDs();
	int dataGetCount();
};

#endif