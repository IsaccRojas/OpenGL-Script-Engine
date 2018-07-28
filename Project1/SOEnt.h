#pragma once

#ifndef SOENT_H_
#define SOENT_H_

#include "ScriptObj.h"

class SOBaseScript;
class SOEnt;
class SOProjectile;
class SONPC;
class SOPlayer;
class SOEffect;

enum EntType_enum { ET_ENT, ET_PROJECTILE, ET_NPC, ET_PLAYER, ET_EFFECT };
typedef enum EntType_enum EntType;

class Abstr_Dispatcher {
public:
	virtual void dispatch(SOEnt& soent) = 0;
	virtual void dispatch(SOProjectile& soprojectile) = 0;
	virtual void dispatch(SONPC& sonpc) = 0;
	virtual void dispatch(SOPlayer& soplayer) = 0;
	virtual void dispatch(SOEffect& soeffect) = 0;
};

class Dispatcher : public Abstr_Dispatcher {
public:
	Dispatcher();
	virtual void dispatch(SOEnt& soent);
	virtual void dispatch(SOProjectile& soprojectile);
	virtual void dispatch(SONPC& sonpc);
	virtual void dispatch(SOPlayer& soplayer);
	virtual void dispatch(SOEffect& soeffect);
};

class SOEnt : public ScriptObj {
protected:
	SOBaseScript* master;
public:
	EntPointer E;
	EntType ent_type;
	int index;
	int kill;
	SOEnt(Entity* Ent, SOBaseScript* master_ptr);
	virtual void first() override;
	virtual void base_script() override;
	virtual void anim();
	virtual void kill_script();
	virtual void run(Abstr_Dispatcher &dispatcher) = 0;
};

#endif