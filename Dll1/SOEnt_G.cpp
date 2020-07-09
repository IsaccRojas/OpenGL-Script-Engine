
#include "../Project1/SOEnt.h"

#include <stdlib.h>
#include <time.h>

extern "C" BOOST_SYMBOL_EXPORT SOEnt *gen_SOEnt_G(Entity *ent_ptr, MemVec<SOEnt*> *mv);

class SOEnt_G : public SOEnt {
public:
	SOEnt_G(Entity *ent_ptr, MemVec<SOEnt*> *mv);
	void base_script() override;
	void on_collide() override;

	void wander();
	void run();
	vec2 dir;

	//EntPage pg;
};

SOEnt_G::SOEnt_G(Entity *ent_ptr, MemVec<SOEnt*> *mv) : SOEnt(ent_ptr, mv) {
	//x, y, w, h, tox, toy=8, tw=16, th=16, u=0, v=0, uvw=16, uvh=16
	E.spack(Packet{ 
		0, 0,	//x, y
		13, 13, //w, h
		-8, 8,  //tox, toy,
		16, 16, //tw, th
		64, 0,	//u, v
		16, 16	//uvw, uvh
	});

	std::default_random_engine gen;
	gen.seed(unsigned(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> distr(0.0f, 60.0f);

	DT.push_back(DataTag("mode", 0.0f));							//1
	DT.push_back(DataTag("behavior", 1.0f));						//2
	DT.push_back(DataTag("behavior_t", distr(gen)));				//3

	distr = std::uniform_real_distribution<float>(-10.0f, 10.0f);

	DT.push_back(DataTag("behavior_t_max", 60.0f + distr(gen)));	//4
	DT.push_back(DataTag("speed", 0.2f));							//5
}

void SOEnt_G::base_script() {
	//check for collision
	int mode = 0;
	EntPage pg = API->readPage(0);
	int d = int(dist(E.gpos(), vec2(pg.P.x, pg.P.y)));
	if (d < 36)
		mode = 1;
	
	//mode
	if (mode)
		run();
	else
		wander();

	DT.at(1).n = float(mode);
}

void SOEnt_G::wander() {
	int b = int(DT.at(2).n);
	int t = int(DT.at(3).n);
	float s = DT.at(5).n;

	//behavior timer
	if (--t < 0)
		t = 0;

	//change behavior
	if (t == 0) {
		//always idle if last behavior was moving
		if (3 <= b && b <= 4)
			b = 1;
		else
			b = rand() % 4 + 1;

		//get rand direction of 16
		if (3 <= b && b <= 4) {
			float pi_r = 6.28318530718f * (float(rand() % 16) / 16.0f);
			dir = vec2(cosf(pi_r), sinf(pi_r));
			if (b == 3)
				s = 0.2f;
			else
				s = 0.4f;
		};

		t = int(DT.at(4).n);
	}

	if (1 <= b && b <= 2) { //behavior 1: idle (50%)
		//do nothing
	}

	else if (3 <= b && b <= 4) { //behavior 2: slow or fast move (50%)
		vec2 pos = E.gpos();
		E.spos(pos.x + dir.x * s, pos.y + dir.y * s);
	}

	DT.at(2).n = float(b);
	DT.at(3).n = float(t);
	DT.at(5).n = s;
}

void SOEnt_G::run() {
	float rs = DT.at(5).n;
	rs = 1.0f;

	EntPage pg = API->readPage(0);
	vec2 pos = E.gpos();
	vec2 p_pos = vec2(pg.P.x, pg.P.y);
	vec2 run_dir = vec2(pos.x - p_pos.x, pos.y - p_pos.y);
	run_dir /= mag(run_dir);

	E.spos(pos.x + run_dir.x * rs, pos.y + run_dir.y * rs);

	DT.at(5).n = rs;
}

void SOEnt_G::on_collide() {}

SOEnt *gen_SOEnt_G(Entity *ent_ptr, MemVec<SOEnt*> *mv) {
	return new SOEnt_G(ent_ptr, mv);
}