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

	std::vector<Entity*> vFreeEnts;
	Entity* alloc(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image imgTex, int prior);
	Entity* alloc(Packet p, Image imgTex, int prior);
public:
	static unsigned int count_existed;
	std::list<Entity> vEnts;
	std::vector<unsigned int> vElements;
	std::vector<float> vData;

	EBuffer();

	/*
	(x, y): physical center position
	(w, h): physical dimensions
	(tox, toy): visuals' offset from physical center
	(tw, th): visuals' dimensions
	(u, v): texture pixel position
	(uvw, uvh): texture pixel dimensions
	imgTex: Image object containing texture image information
	*/
	Entity* allocateEntity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image imgTex, int prior);
	Entity* allocateEntity(Packet p, Image imgTex, int prior);
	void allocateEntity();

	Entity* newEntity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image imgTex, int prior);
	Entity* newEntity(Packet p, Image imgTex, int prior);

	Entity* getEnt(unsigned int p_ID);
	unsigned int getEBuff_ID();
	void update();
	int erase(Entity* ent);
	/* sets buffer to a concatenation of all entities' data; set dir to 1:increased, -1:decreased, 0:total_reset; set position to -1 if increase/decrease occurred at end of vector (ignored if dir == 0) */
	void dataResizeReset(int dir, int position);
	
	void freeEntity(Entity* ent);
	int dataGetCount();
};

#endif