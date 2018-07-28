#pragma once

#ifndef SOPROJECTILE_H_
#define SOPROJECTILE_H_

#include "SOEnt.h"

class SOProjectile : public SOEnt {
	int time;
	bool shift;
public:
	SOProjectile(Entity* Ent, SOBaseScript* master_ptr);
	vec2 velocity;
	bool collided;
	void first() override;
	void base_script() override;
	void anim() override;
	void kill_script() override;
	void run(Abstr_Dispatcher &dispatcher) override;
};

#endif

