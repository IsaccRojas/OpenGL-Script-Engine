#include "SOEnt.h"
#include "SOBaseScript.h"
#include "SOEntDataBase.h"
#include "SOPlayer.h"
#include "SOText.h"

DataTag::DataTag() {}
DataTag::DataTag(std::string tag_name, float tag_n) : name(tag_name), n(tag_n) {}

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
	//if (EData.type != ET_TEXT)
		//E = master_ptr->EBuffAlloc(EData.P, master_ptr->getTexture(EData.type), 0);
	master = master_ptr;
	E = ent_ptr;
	//data = EData.dt;
	//name = EData.name;
	//ent_type = EData.type;
	//frames = EData.frames;
	kill = 0;
}

void SOEnt::base_script() {
	//lua_pushlightuserdata(master->L, (void*)this);
	//lua_setglobal(master->L, "this");
	//lua_getglobal(master->L, name.c_str());
	//lua_pcall(master->L, 0, 0, 0);
}

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