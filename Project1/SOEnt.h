#pragma once

#ifndef SOENT_H_
#define SOENT_H_

#include "ScriptObj.h"

struct EntData;

struct DataTag {
	DataTag();
	DataTag(std::string tag_name, float tag_n);
	std::string name;
	float n;
};

class SOBaseScript;
class SOEnt;
class SOPlayer;
class SOText;

enum EntType_enum { ET_ENT, ET_PLAYER, ET_NPC, ET_TILE, ET_PROJECTILE, ET_EFFECT, ET_TEXT };
typedef enum EntType_enum EntType;

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

class SOEnt : public ScriptObj {
protected:
	SOBaseScript* master;
public:
	std::vector<DataTag> data;
	std::string name;
	EntPointer E;
	EntType ent_type;
	std::vector<std::vector<float>> frames;
	int index;
	SOEnt(EntData EData, SOBaseScript* master_ptr);
	virtual void base_script() override;
	virtual void run(Abstr_Dispatcher &dispatcher);
};

#endif