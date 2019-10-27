#pragma once

#ifndef SOENT_H_
#define SOENT_H_

#include "EBuffer/Entity.h"
#include <boost/dll/config.hpp>
#include <boost/dll.hpp>

class SOEnt;

enum EntType_enum { ET_ENT, ET_PLAYER, ET_NPC, ET_TILE, ET_PROJECTILE, ET_EFFECT, ET_TEXT };
typedef enum EntType_enum EntType;

struct DataTag {
	DataTag();
	DataTag(std::string tag_name, float tag_n);
	std::string name;
	float n;
};

class EntPage {
	friend class API;
private:
	SOEnt *owner;
	bool b_empty;
public:
	EntPage();
	EntPage(Packet p, std::vector<DataTag> dt);
	bool empty();
	Packet P;
	std::vector<DataTag> DT;
};

class SOEnt {
protected:
	EntPointer E;
	std::vector<DataTag> DT;
	int index;
public:
	int kill;
	SOEnt(Entity *ent_ptr = nullptr);
	virtual void base_script();

	EntPointer getEntPointer();
	void setEnt(Entity *ent_ptr);
	
	int getIndex();
	void setIndex(int i);

	EntPage getPage();
	void setPage(EntPage pg);
};

class API {
	//structure *set_of_entities;
public:
	API();
	EntPage readPage(SOEnt* ent);
	void writePage(EntPage pg);
};

#endif