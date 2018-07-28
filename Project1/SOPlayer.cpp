#include "SOPlayer.h"
#include "SOBaseScript.h"

SOPlayer::SOPlayer(Entity* Ent, SOBaseScript* master_ptr) : SOEnt(Ent, master_ptr) {
	ent_type = ET_PLAYER;
	spd = 4.0f;
	proj_spd = 8.0f;
	cooldown = 0.0f;
	shoot_cd_max = 10.0f;
}

void SOPlayer::base_script() {
	if (first_exec)
		first();
	if (kill == 1) {
		kill_script();
		return;
	}
	if (kill == 2)
		return;
	anim();
	auto input = SDL_GetKeyboardState(NULL);
	if (input[SDL_SCANCODE_W]) {
		E.spos(E.gpos().x, E.gpos().y + spd);
	}
	if (input[SDL_SCANCODE_A]) {
		E.spos(E.gpos().x - spd, E.gpos().y);
	}
	if (input[SDL_SCANCODE_S]) {
		E.spos(E.gpos().x, E.gpos().y - spd);
	}
	if (input[SDL_SCANCODE_D]) {
		E.spos(E.gpos().x + spd, E.gpos().y);
	}
	if (input[SDL_SCANCODE_UP]) {
		shoot(0);
	}
	if (input[SDL_SCANCODE_LEFT]) {
		shoot(1);
	}
	if (input[SDL_SCANCODE_DOWN]) {
		shoot(2);
	}
	if (input[SDL_SCANCODE_RIGHT]) {
		shoot(3);
	}
	/*
	if (SDL_BUTTON(SDL_GetMouseState(NULL, NULL)) == 1) {
		shoot();
	}
	*/
	cooldown -= 1.0f;
	if (cooldown < 0.0f)
		cooldown = 0.0f;
}

void SOPlayer::anim() {
	if (cooldown >= shoot_cd_max - 3)
		E.suvpos(0.0f, 0.0f);
	else
		E.suvpos(0.0f, 32.0f);
}

void SOPlayer::kill_script() {}

void SOPlayer::shoot(int dir) {
	if (cooldown <= 0.0f) {
		SOProjectile* projectile = dynamic_cast<SOProjectile*>(master->gen_ent("null"));
		if (projectile != nullptr) {
			auto input = SDL_GetKeyboardState(NULL);
			if (dir == 0) {
				projectile->velocity = vec2(0.0f, proj_spd);
				projectile->E.spos(E.gpos().x, E.gpos().y + 16.0f);
			}
			if (dir == 1) {
				projectile->velocity = vec2(-1.0f * proj_spd, 0.0f);
				projectile->E.spos(E.gpos().x - 16.0f, E.gpos().y);
			}
			if (dir == 2) {
				projectile->velocity = vec2(0.0f, -1.0f * proj_spd);
				projectile->E.spos(E.gpos().x, E.gpos().y - 16.0f);
			}
			if (dir == 3) {
				projectile->velocity = vec2(proj_spd, 0.0f);
				projectile->E.spos(E.gpos().x + 16.0f, E.gpos().y);
			}
			cooldown = shoot_cd_max;
		}
	}
}

void SOPlayer::run(Abstr_Dispatcher &dispatcher) {
	dispatcher.dispatch(*this);
}