#include "SOEnt.h"
#include "SOBaseScript.h"
#include "SOEntDataBase.h"

DataTag::DataTag() {}
DataTag::DataTag(std::string tag_name, float tag_n) : name(tag_name), n(tag_n) {}

EntPage::EntPage() { b_empty = true; }
EntPage::EntPage(Packet p, std::vector<DataTag> dt) : P(p), DT(dt) { b_empty = false; }
bool EntPage::empty() { return b_empty; }

SOEnt::SOEnt(Entity *ent_ptr) {
	E = ent_ptr;
	kill = 0;
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

EntPage SOEnt::getPage() {
	if (E.gent() == nullptr)
		return EntPage();
	return EntPage(E.gpack(), DT);
}

void SOEnt::setPage(EntPage pg) {
	if (E.gent() != nullptr) {
		E.spack(pg.P);
		DT = pg.DT;
	}
}