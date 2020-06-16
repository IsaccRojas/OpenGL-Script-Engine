
#include "../Project1/SOEnt.h"

extern "C" BOOST_SYMBOL_EXPORT SOEnt *gen_SOEnt_Pl(Entity *ent_ptr, MemVec<SOEnt*> *mv);

class SOEnt_Pl : public SOEnt {
public:
	SOEnt_Pl(Entity *ent_ptr, MemVec<SOEnt*> *mv);
	void base_script() override;
	vec2 vel;
	EntPage pg;
};

SOEnt_Pl::SOEnt_Pl(Entity *ent_ptr, MemVec<SOEnt*> *mv) : SOEnt(ent_ptr, mv) {
	//x=0, y=0, w=13, h=13, tox=-8, toy=8, tw=16, th=16, u=0, v=0, uvw=16, uvh=16
	E.spack(Packet{ 0, 0, 13, 13, -8, 8, 16, 16, 0, 0, 16, 16 });
	DT.push_back(DataTag("accel", 0.3f));	//1
	DT.push_back(DataTag("deccel", 0.15f));	//2
	DT.push_back(DataTag("maxspd", 2.0f));	//3
}

float mag(vec2 vec) {
	return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

void SOEnt_Pl::base_script() {
	uint8_t *in = API->getKeyInput();

	vec2 vel_i = vel;
	float spd_i = mag(vel);
	float dec_factor;
	if (spd_i != 0)
		dec_factor = DT.at(2).n / mag(vel);
	else
		dec_factor = 0;
	vel -= vel * dec_factor;

	if ((vel_i.x > 0 && vel.x < 0) || (vel_i.x < 0 && vel.x > 0))
		vel.x = 0;
	if ((vel_i.y > 0 && vel.y < 0) || (vel_i.y < 0 && vel.y > 0))
		vel.y = 0;

	if (in[26]) //26 = W (y+)
		vel.y += DT.at(1).n;
	if (in[4]) //26 = A (x-)
		vel.x -= DT.at(1).n;
	if (in[22]) //26 = S (y-)
		vel.y -= DT.at(1).n;
	if (in[7]) //26 = D (x+)
		vel.x += DT.at(1).n;

	float spd = mag(vel);
	if (spd > DT.at(3).n) {
		float max_factor = DT.at(3).n / spd;
		vel = vel * max_factor;
	}

	vec2 pos = E.gpos();
	E.spos(pos.x + vel.x, pos.y + vel.y);
}

SOEnt *gen_SOEnt_Pl(Entity *ent_ptr, MemVec<SOEnt*> *mv) {
	return new SOEnt_Pl(ent_ptr, mv);
}