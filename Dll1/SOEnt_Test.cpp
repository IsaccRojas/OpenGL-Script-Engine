
#include "../Project1/SOEnt.h"

extern "C" BOOST_SYMBOL_EXPORT SOEnt *gen_SOEnt_Test(Entity *ent_ptr, MemVec<SOEnt*> *mv);

class SOEnt_Test : public SOEnt {
public:
	SOEnt_Test(Entity *ent_ptr, MemVec<SOEnt*> *mv);
	void base_script() override;
	float r;
	float f;
	bool b;
	EntPage pg;
};

SOEnt_Test::SOEnt_Test(Entity *ent_ptr, MemVec<SOEnt*> *mv) : SOEnt(ent_ptr, mv) {
	//x=0, y=0, w=13, h=13, tox=-8, toy=8, tw=16, th=16, u=0, v=0, uvw=16, uvh=16
	E.spack(Packet{ 0, 0, 13, 13, -8, 8, 16, 16, 0, 0, 16, 16 });
	r = 16.0f;
	f = 0.0f;
	b = false;
	DT.push_back(DataTag("centerx", 0.0f));
	DT.push_back(DataTag("centery", 0.0f));
	DT.push_back(DataTag("speed", 0.07f));
}

void SOEnt_Test::base_script() {
	if (index != 1 && pg.empty())
		pg = API->readPage(1);

	if (!(pg.empty())) {
		pg.DT.at(1).n = E.gpos().x;
		pg.DT.at(2).n = E.gpos().y;
		pg.DT.at(3).n = -0.07f;
		API->writePage(pg);
	}

	if (index == 0) {
		float X = API->getMouse().x;
		float Y = API->getMouse().y;
		std::cout << "SOEnt_Test: " << "X " << X << ", Y " << Y << std::endl;
		//DT.at(1).n = X;
		//DT.at(2).n = Y;
	}

	E.spos(DT.at(1).n + (r * cos(f * DT.at(3).n)), DT.at(2).n + (r * sin(f * DT.at(3).n)));
	f++;
}

SOEnt *gen_SOEnt_Test(Entity *ent_ptr, MemVec<SOEnt*> *mv) {
	return new SOEnt_Test(ent_ptr, mv);
}
