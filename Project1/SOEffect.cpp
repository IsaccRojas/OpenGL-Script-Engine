#include "SOEffect.h"
#include "SOBaseScript.h"

SOEffect::SOEffect(Entity* Ent, SOBaseScript* master_ptr) : SOEnt(Ent, master_ptr) {
	ent_type = ET_EFFECT;
	frame = 0;
}

void SOEffect::base_script() {
	anim();
	if (frame == 10)
		kill = 2;
	frame++;
}

void SOEffect::anim() {
	
}

void SOEffect::run(Abstr_Dispatcher &dispatcher) {
	dispatcher.dispatch(*this);
}