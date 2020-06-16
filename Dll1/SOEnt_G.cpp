
#include "../Project1/SOEnt.h"

#include <stdlib.h>
#include <time.h>

extern "C" BOOST_SYMBOL_EXPORT SOEnt *gen_SOEnt_G(Entity *ent_ptr, MemVec<SOEnt*> *mv);

class SOEnt_G : public SOEnt {
public:
	SOEnt_G(Entity *ent_ptr, MemVec<SOEnt*> *mv);
	void base_script() override;

	void wander();
	void seek();
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

	srand(time(NULL));

	DT.push_back(DataTag("seek", 0.0f));									//1
	DT.push_back(DataTag("behavior", 1.0f));								//2
	DT.push_back(DataTag("behavior_t", float(rand() % 60)));				//3
	DT.push_back(DataTag("behavior_tm", 60.0f + float(rand() % 10 - 8)));	//4
	DT.push_back(DataTag("speed", 0.2f));									//5
}

//float mag(vec2 vec) {
//	return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
//}

void SOEnt_G::base_script() {
	int sk = int(DT.at(1).n);
	
	//mode
	if (sk == 1)
		seek();
	else
		wander();
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

void SOEnt_G::seek() {

}

SOEnt *gen_SOEnt_G(Entity *ent_ptr, MemVec<SOEnt*> *mv) {
	return new SOEnt_G(ent_ptr, mv);
}