#pragma once

#ifndef SOEFFECT_H_
#define SOEFFECT_H_

#include "SOEnt.h"

class SOEffect : public SOEnt {
	int frame = 0;
public:
	SOEffect(Entity* Ent, SOBaseScript* master_ptr);
	void base_script() override;
	void anim() override;
	void run(Abstr_Dispatcher &dispatcher) override;
};

#endif
