#pragma once

#ifndef ENTITY_H_
#define ENTITY_H_

#include "../tools.h"
#include <vector>
#include <string>
#include <iostream>

class EBuffer;

struct Packet {
	Packet();
	Packet(float X, float Y, float W, float H, float toX, float toY, float tW, float tH, float U, float V, float uvW, float uvH);
	float x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh;
	Packet operator+(Packet p);
};

class Entity {
	int priority;
	float X, Y, W, H, toX, toY, tW, tH, U, V, UVW, UVH, T;
	Image tex;
	std::string ent_name;
public:
	int index;
	EBuffer* master;
	
	static unsigned const int indicesperentityv;
	static unsigned const int indicesperentitye;
	static unsigned int count_existed;
	bool bChangeSinceLastCheck;
	bool bPriorityChange;
	bool bFree;
	unsigned int ID;
	std::vector<float> vVertices;
	std::vector<unsigned int> vElements;
	/*
	(x, y): physical center position
	(w, h): physical dimensions
	(tox, toy): visuals' offset from physical center
	(tw, th): visuals' dimensions
	(u, v): texture pixel position
	(uvw, uvh): texture pixel dimensions
	*/
	Entity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image imgTex, int prior);
	Entity(Packet p, Image imgTex, int prior);
	Entity();
	~Entity();
	/* data must be size 16 */
	void dataNew(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image imgTex, int prior);
	void dataNew(Packet p, Image imgTex, int prior);

	void dataSetPos(float x, float y);
	void dataSetDimensions(float w, float h);
	void dataSetTexOffset(float tox, float toy);
	void dataSetTexDimensions(float tw, float th);
	void dataSetUV(float u, float v);
	void dataSetUVDimensions(float uvw, float uvh);
	void dataReset();
	void dataFree();
	Packet dataPacket();
	unsigned int getID();

	vec2 dataGetPos();
	vec2 dataGetDimensions();
	vec2 dataGetTexOffset();
	vec2 dataGetTexDimensions();
	vec2 dataGetUV();
	vec2 dataGetUVDimensions();
	int dataGetTexUnit();
	int getPriority();
	void setPriority(int p);
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
	Packet gpack();
	int gtu();
	int gprior();
	void sprior(int p);
};

#endif