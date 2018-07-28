#pragma once

#ifndef SOPLAYER_H_
#define SOPLAYER_H_

#include "SOEnt.h"

class SOPlayer : public SOEnt {
	float spd;
	float proj_spd;
	float cooldown;
	float shoot_cd_max;
public:
	SOPlayer(Entity* Ent, SOBaseScript* master_ptr);
	void base_script() override;
	void anim() override;
	void kill_script() override;
	void shoot(int dir);
	void run(Abstr_Dispatcher &dispatcher) override;
};

#endif