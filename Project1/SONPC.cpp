#include "SONPC.h"
#include "SOBaseScript.h"

SONPC::SONPC(Entity* Ent, SOBaseScript* master_ptr) : SOEnt(Ent, master_ptr) {
	ent_type = ET_NPC;
	spd = 2.0f;
	proj_spd = 8.0f;
	cooldown = 0.0f;
	shoot_cd_max = 10.0f;
	player = master->entities.at(0)->E;
}

void SONPC::base_script() {
	if (first_exec)
		first();
	if (kill == 1) {
		kill_script();
		return;
	}
	if (kill == 2)
		return;

	float mag = dist(E.gpos(), player.gpos());
	vel.x = spd * ((player.gpos().x - E.gpos().x) / mag);
	vel.y = spd * ((player.gpos().y - E.gpos().y) / mag);
	E.spos(E.gpos().x + vel.x, E.gpos().y + vel.y);

	for (unsigned n = 0; n < master->entities.size(); n++) {
		if (master->entities.at(n)->ent_type == ET_PROJECTILE) {
			if (dist(master->entities.at(n)->E.gpos(), E.gpos()) < 28.0f && !(dynamic_cast<SOProjectile*>(master->entities.at(n))->collided)) {
				kill = 1;
				dynamic_cast<SOProjectile*>(master->entities.at(n))->collided = true;
				break;
			}
		}
	}

	cooldown -= 1.0f;
	if (cooldown < 0.0f)
		cooldown = 0.0f;
}

void SONPC::anim() {}

void SONPC::kill_script() {
	kill = 2;
}

void SONPC::shoot(int dir) {
	if (cooldown <= 0.0f) {
		SOEnt* projectile = master->gen_ent("null");
		auto input = SDL_GetKeyboardState(NULL);
		


		cooldown = shoot_cd_max;
	}
}

float SONPC::dist(vec2 p1, vec2 p2) {
	return sqrtf(powf((p2.x - p1.x), 2) + powf((p2.y - p1.y), 2));
}

void SONPC::run(Abstr_Dispatcher &dispatcher) {
	dispatcher.dispatch(*this);
}