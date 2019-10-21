#pragma once

#ifndef SOPLAYER_H_
#define SOPLAYER_H_

#include "SOEnt.h"

/*
INDEX 0 - kill
INDEX 1 - spd
INDEX 2 - proj_spd
INDEX 3 - shoot_cooldown
INDEX 4 - shoot_cooldown_max
*/

class SOPlayer : public SOEnt {
public:
	SOPlayer(Entity *ent_ptr = nullptr, SOBaseScript *master_ptr = nullptr);
	void base_script() override;
	void run(Abstr_Dispatcher &dispatcher) override;
};

#endif