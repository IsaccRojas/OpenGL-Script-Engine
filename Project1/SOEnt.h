#pragma once

#ifndef SOENT_H_
#define SOENT_H_

//#include "ScriptObj.h"
#include "EBuffer/Entity.h"
#include <boost/dll/config.hpp>
#include <boost/dll.hpp>

enum EntType_enum { ET_ENT, ET_PLAYER, ET_NPC, ET_TILE, ET_PROJECTILE, ET_EFFECT, ET_TEXT };
typedef enum EntType_enum EntType;

struct DataTag {
	DataTag();
	DataTag(std::string tag_name, float tag_n);
	std::string name;
	float n;
};

struct EntPage {
	EntPage();
	EntPage(Packet p, std::vector<DataTag> dt);
	Packet P;
	std::vector<DataTag> DT;
};

class SOBaseScript;
class SOEnt;
class SOPlayer;
class SOText;

class Abstr_Dispatcher {
public:
	virtual void dispatch(SOEnt& soent) = 0;
	virtual void dispatch(SOPlayer& soplayer) = 0;
	virtual void dispatch(SOText& sotext) = 0;
};

class Dispatcher : public Abstr_Dispatcher {
public:
	Dispatcher();
	virtual void dispatch(SOEnt& soent);
	virtual void dispatch(SOPlayer& soplayer);
	virtual void dispatch(SOText& sotext);
};

/*
INDEX 0 - kill
*/

class SOEnt {
protected:
	SOBaseScript* master;
	//EntData ED;
	EntPointer E;
	int index;
	int kill;
public:
	SOEnt(Entity *ent_ptr = nullptr, SOBaseScript *master_ptr = nullptr);
	virtual void base_script();
	virtual void run(Abstr_Dispatcher &dispatcher);
	virtual Packet getPacket();

	void setEnt(Entity *ent_ptr);

	int getIndex();
	void setIndex(int i);

	EntPointer getEntPointer();

	//TODO: currently hardcoded to return kill variable
	int getData();
};

#endif