#pragma once

#ifndef SOBASESCRIPT_H_
#define SOBASESCRIPT_H_

#include "ScriptObj.h"
#include "SOPlayer.h"
#include "SOProjectile.h"
#include "SONPC.h"
#include "SOEffect.h"

/*
INDEX 0: PLAYER
*/

class SOBaseScript : public ScriptObj {
	EBuffer* EBuff;
	GLBuffer<float>* VertBuf;
	GLBuffer<unsigned>* ElBuf;
	Resources* res;
	Image texture;
	Dispatcher dispatcher;
	std::vector<SOEnt*> killed_entities;
	int frame;
public:
	SOBaseScript(Resources* resources);

	Resources getResources();
	std::vector<SOEnt*> entities;

	void first() override;
	void render();
	void base_script() override;

	SOEnt* gen_ent(std::string name);
	void del_ent(SOEnt* soentity);
};

#endif