#include "SOEnt.h"
#include "SOBaseScript.h"
#include "SOProjectile.h"
#include "SONPC.h"
#include "SOPlayer.h"
#include "SOEffect.h"

Dispatcher::Dispatcher() {}

void Dispatcher::dispatch(SOEnt& soent) {
	soent.base_script();
}
void Dispatcher::dispatch(SOProjectile& soprojectile) {
	soprojectile.base_script();
}
void Dispatcher::dispatch(SONPC& sonpc) {
	sonpc.base_script();
}
void Dispatcher::dispatch(SOPlayer& soplayer) {
	soplayer.base_script();
}
void Dispatcher::dispatch(SOEffect& soeffect) {
	soeffect.base_script();
}

SOEnt::SOEnt(Entity* Ent, SOBaseScript* master_ptr) {
	E = Ent;
	master = master_ptr;
	kill = 0;
	first_exec = true;
}

void SOEnt::first() {}

void SOEnt::base_script() {
	if (kill == 1)
		kill_script();
	if (kill == 2)
		return;
}

void SOEnt::anim() {}

void SOEnt::kill_script() {}