#include "SOProjectile.h"
#include "SOBaseScript.h"

SOProjectile::SOProjectile(Entity* Ent, SOBaseScript* master_ptr) : SOEnt(Ent, master_ptr) {
	ent_type = ET_PROJECTILE;
	time = 0;
	collided = false;
}

void SOProjectile::first() {
	shift = false;
	first_exec = false;
}

void SOProjectile::base_script() {
	if (first_exec)
		first();
	if (kill == 1) {
		kill_script();
		return;
	}
	if (kill == 2)
		return;

	anim();
	E.spos(E.gpos().x + velocity.x, E.gpos().y + velocity.y);
	time++;
	if (time == 25 || collided)
		kill = 1;
}

void SOProjectile::anim() {
	if (time % 5 == 0) {
		E.stoff(E.gtoff().x + 1.0f, E.gtoff().y - 1.0f);
		E.stdim(E.gtdim().x - 2.0f, E.gtdim().y - 2.0f);
	}
}

void SOProjectile::kill_script() {
	kill = 2;
}

void SOProjectile::run(Abstr_Dispatcher &dispatcher) {
	dispatcher.dispatch(*this);
}