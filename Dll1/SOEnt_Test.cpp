
#include "../Project1/SOEnt.h"

extern "C" BOOST_SYMBOL_EXPORT SOEnt *gen_SOEnt_Test(Entity *ent_ptr, SOBaseScript *master_ptr);

class SOEnt_Test : public SOEnt {
public:
	SOEnt_Test(Entity *ent_ptr, SOBaseScript *master_ptr);
	void base_script() override;
	void run(Abstr_Dispatcher &dispatcher) override;
};

SOEnt_Test::SOEnt_Test(Entity *ent_ptr, SOBaseScript *master_ptr) : SOEnt(ent_ptr, master_ptr) {
	std::cout << "/t/tin SOEnt_Test constructor" << std::endl;
}

void SOEnt_Test::base_script() {
	std::cout << "test output" << std::endl;
}

void SOEnt_Test::run(Abstr_Dispatcher &dispatcher) {
	dispatcher.dispatch(*this);
}

SOEnt *gen_SOEnt_Test(Entity *ent_ptr, SOBaseScript* master_ptr) {
	std::cout << "/tallocating and returning SOEnt_Test(...)" << std::endl;
	return new SOEnt_Test(ent_ptr, master_ptr);
}
