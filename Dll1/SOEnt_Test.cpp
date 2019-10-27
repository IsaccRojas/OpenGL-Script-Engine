
#include "../Project1/SOEnt.h"

extern "C" BOOST_SYMBOL_EXPORT SOEnt *gen_SOEnt_Test(Entity *ent_ptr);

class SOEnt_Test : public SOEnt {
public:
	SOEnt_Test(Entity *ent_ptr);
	void base_script() override;
	float r;
	float f;
	float s;
	vec2 center;
};

SOEnt_Test::SOEnt_Test(Entity *ent_ptr) : SOEnt(ent_ptr) {
	//x=0, y=0, w=13, h=13, tox=-8, toy=8, tw=16, th=16, u=0, v=0, uvw=16, uvh=16
	E.spack(Packet{ 0, 0, 13, 13, -8, 8, 16, 16, 0, 0, 16, 16 });
	r = 16.0f;
	f = 0.0f;
	s = 0.07f;
	center.x = 0.0f;
	center.y = 0.0f;
}

void SOEnt_Test::base_script() {
	E.spos(center.x + (r * cos(f * s)), center.y + (r * sin(f * s)));
	f++;
}

SOEnt *gen_SOEnt_Test(Entity *ent_ptr) {
	return new SOEnt_Test(ent_ptr);
}
