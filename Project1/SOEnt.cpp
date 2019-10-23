#include "SOEnt.h"
#include "SOBaseScript.h"
#include "SOEntDataBase.h"
#include "SOPlayer.h"
#include "SOText.h"

DataTag::DataTag() {}
DataTag::DataTag(std::string tag_name, float tag_n) : name(tag_name), n(tag_n) {}

EntPage::EntPage() {}
EntPage::EntPage(Packet p, std::vector<DataTag> dt) : P(p), DT(dt) {}

Dispatcher::Dispatcher() {}

void Dispatcher::dispatch(SOEnt& soent) {
	soent.base_script();
}
void Dispatcher::dispatch(SOPlayer& soplayer) {
	soplayer.base_script();
}
void Dispatcher::dispatch(SOText& sotext) {
	sotext.base_script();
}

SOEnt::SOEnt(Entity *ent_ptr, SOBaseScript* master_ptr) {
	master = master_ptr;
	E = ent_ptr;
	kill = 0;
}

void SOEnt::base_script() {}

void SOEnt::run(Abstr_Dispatcher &dispatcher) {
	dispatcher.dispatch(*this);
}

Packet SOEnt::getPacket() {
	return Packet();
}

void SOEnt::setEnt(Entity *ent_ptr) {
	E = ent_ptr;
}

EntPointer SOEnt::getEntPointer() {
	return E;
}

int SOEnt::getIndex() {
	return index;
}

void SOEnt::setIndex(int i) {
	index = i;
}

int SOEnt::getData() {
	return kill;
}