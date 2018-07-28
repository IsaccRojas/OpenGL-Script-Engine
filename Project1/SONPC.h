#pragma once

#ifndef SONPC_H_
#define SONPC_H_

#include "SOEnt.h"

class SONPC : public SOEnt {
	float spd;
	float proj_spd;
	float cooldown;
	float shoot_cd_max;
	vec2 vel;
	EntPointer player;
public:
	SONPC(Entity* Ent, SOBaseScript* master_ptr);
	void base_script() override;
	void anim() override;
	void kill_script() override;
	void shoot(int dir);
	float dist(vec2 p1, vec2 p2);
	void run(Abstr_Dispatcher &dispatcher) override;
};

#endif
