#include "SOEnt.h"

/* =============================== DataTag =============================== */

DataTag::DataTag() {}
DataTag::DataTag(std::string tag_name, float tag_n) : name(tag_name), n(tag_n) {}

/* =============================== EntPage =============================== */

EntPage::EntPage() { b_empty = true; }
EntPage::EntPage(Packet p, std::vector<DataTag> dt, int i) : P(p), DT(dt), index(i) { b_empty = false; }
bool EntPage::empty() { return b_empty; }

/* ================================ SOEnt ================================ */

SOEnt::SOEnt(Entity *ent_ptr) {
	E = ent_ptr;
	DT.push_back(DataTag("kill", 0));
}

void SOEnt::base_script() {}

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

void SOEnt::setPage(EntPage pg) {
	if (E.gent() != nullptr) {
		E.spack(pg.P);
		DT = pg.DT;
	}
}

/* ================================= API ================================= */

API::API() {}

EntPage API::genEnt(std::string name) {
	return EntPage();
}

void API::delEnt(SOEnt* ent) {}

EntPage API::readPage(SOEnt* ent) {
	return EntPage();
}

void API::writePage(EntPage pg) {
	wr_q.push_back(pg);
}