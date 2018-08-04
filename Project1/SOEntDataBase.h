#pragma once

#ifndef SOENTDATABASE_H_
#define SOENTDATABASE_H_

#include "SOEnt.h"

#include <experimental/filesystem>
#include <iostream>

struct EntData {
	EntData();
	EntData(std::string ent_name, Packet packet, std::vector<DataTag> datatags, EntType ent_type, std::vector<std::vector<float>> ent_frames);
	std::string name;
	Packet P;
	std::vector<DataTag> dt;
	EntType type;
	std::vector<std::vector<float>> frames;
};

class SOEntDataBase {
	std::vector<std::string> script_fnames;
public:
	std::vector<EntData> EData;
	SOEntDataBase();
	int load_all(lua_State* L, std::string dir);
	int load_ents(lua_State* L, std::vector<std::string> names);
	EntData get(unsigned index);
	EntData get(std::string name);
	
	static float get_table_entry(lua_State* L, std::string table_name, std::string key);
};

#endif