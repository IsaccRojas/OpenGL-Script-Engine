#include "SOEnt.h"

/* =============================== DataTag =============================== */

DataTag::DataTag() {}
DataTag::DataTag(std::string tag_name, float tag_n) : name(tag_name), n(tag_n) {}

/* =============================== EntPage =============================== */

EntPage::EntPage() { b_empty = true; }
EntPage::EntPage(Packet p, std::vector<DataTag> dt, int i) : P(p), DT(dt), index(i) { b_empty = false; }
bool EntPage::empty() { return b_empty; }

/* ================================ SOEnt ================================ */

SOEnt::SOEnt(Entity *ent_ptr, MemVec<SOEnt*> *mv) {
	E = ent_ptr;
	DT.push_back(DataTag("kill", 0));
	API = new _API(mv);
}

SOEnt::~SOEnt() {
	delete API;
}

void SOEnt::base_script() {}
void SOEnt::on_collide() {}

EntPointer SOEnt::getEntPointer() {
	return E;
}
void SOEnt::setEnt(Entity *ent_ptr) {
	E = ent_ptr;
}

int SOEnt::getIndex() {
	return index;
}
void SOEnt::setIndex(int i) {
	index = i;
}

float SOEnt::getDataTag(int i) {
	return DT.at(i).n;
}
void SOEnt::setDataTag(int i, float v) {
	DT.at(i).n = v;
}

EntPage SOEnt::getPage() {
	if (E.gent() == nullptr)
		return EntPage();
	return EntPage(E.gpack(), DT, index);
}

void SOEnt::setPage(EntPage &pg) {
	if (E.gent() != nullptr) {
		E.spack(pg.P);
		DT = pg.DT;
	}
}

/* ================================= API ================================= */

_API_global * _API_global::inst = 0;

_API_global::_API_global() {
	key_input = nullptr;
	mouse_pos = vec2();
}

_API_global * _API_global::getInst() {
	if (!inst)
		inst = new _API_global;
	return inst;
}

_API_global::~_API_global() {
	delete inst;
	inst = 0;
}

uint8_t * _API_global::_getKeyInput() {
	return key_input;
}
vec2 _API_global::_getMouse() {
	return mouse_pos;
}
void _API_global::_setKeyInput(uint8_t *keys) {
	key_input = keys;
}
void _API_global::_setMouse(vec2 mouse) {
	mouse_pos = mouse;
}

_API::_API(MemVec<SOEnt*> *mv) {
	MV = mv;
}

void _API::setGlobals(_API_global *global) {
	globals = global;
}

vec2 _API::getMouse() {
	return globals->_getMouse();
}

uint8_t *_API::getKeyInput() {
	return globals->_getKeyInput();
}

EntPage _API::genEnt(std::string name) {
	gen_q.push_back(name);
	return EntPage();
}

void _API::delEnt(SOEnt* ent) {}

EntPage _API::readPage(int i) {
	return this->MV->at(i)->getPage();
}

/*
EntPage API::readPage(int i) {
std::cout << "reading " << std::endl;
if (API::MV->size() > i && i >= 0) {
std::cout << "i within range" << std::endl;
return API::MV->at(i)->getPage();
}
std::cout << "ERROR: i (" << i << ") is not within valid range" << std::endl;
return EntPage();
}
*/

void _API::writePage(EntPage &pg) {
	wr_q.push_back(pg);
}