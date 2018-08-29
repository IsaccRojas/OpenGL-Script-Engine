#include "SOText.h"
#include "SOBaseScript.h"

/*
0 - kill
1 - spacing
*/

SOText::SOText(EntData EData, SOBaseScript* master_ptr) : SOEnt(EData, master_ptr) {
	E = new Entity(EData.P, master_ptr->getTexture(EData.type), 0);
}

void SOText::base_script() {
	lua_pushlightuserdata(master->L, (void*)this);
	lua_setglobal(master->L, "this");
	lua_getglobal(master->L, name.c_str());
	lua_pcall(master->L, 0, 0, 0);
}

void SOText::run(Abstr_Dispatcher &dispatcher) {
	dispatcher.dispatch(*this);
}

void SOText::alloc_chars(int n) {

}

void SOText::set(std::string str) {
	text = str;
}

void SOText::b_set(int n, std::string str) {
	buffer_size = n;
	while (char_data.size() > 0) {
		master->EBuffDealloc(char_data.back());
		char_data.pop_back();
	}
	for (int m = 0; m < n; m++)
		char_data.push_back(
			master->EBuffAlloc(
				E.gpack() + Packet{ m * (E.guvdim().x + data.at(1).n), 0, 0, 0, 0, 0, 0, 0, (str.at(m) - ' ') * (E.guvdim().x + 1), 0, 0, 0 }, 
				master->getTexture(ET_TEXT), 
				0
			)
		);
}

std::string SOText::get() {
	return text;
}