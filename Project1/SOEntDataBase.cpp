#include "SOEntDataBase.h"

EntData::EntData() {
	name = "null";
	P = Packet();
	type = ET_ENT;
}
EntData::EntData(std::string ent_name, Packet packet, std::vector<DataTag> datatags, EntType ent_type, std::vector<std::vector<float>> ent_frames) : name(ent_name), P(packet), dt(datatags), type(ent_type), frames(ent_frames) {}

SOEntDataBase::SOEntDataBase() {}

int SOEntDataBase::load_all(lua_State* L, std::string dir) {
	int code;
	int errors = 0;
	for (std::experimental::filesystem::path p : std::experimental::filesystem::directory_iterator(dir)) {
		std::cout << "Loading script '" << p.string().c_str() << "'... ";
		code = luaL_loadfile(L, p.string().c_str());
		if (code == LUA_ERRSYNTAX) {
			std::cout << "Script error: syntax error for script '" << p.string().c_str() << "'." << std::endl;
			errors++;
		}
		else if (code == LUA_ERRMEM) {
			std::cout << "Script error: memory allocation error for script '" << p.string().c_str() << "'." << std::endl;
			errors++;
		}
		else if (code == LUA_ERRFILE) {
			std::cout << "Script error: file opening error for script '" << p.string().c_str() << "'." << std::endl;
			errors++;
		}
		else {
			if (lua_pcall(L, 0, 0, 0) != 0)
				std::cout << "Error during execution: " << lua_tostring(L, -1) << std::endl;
			else {
				script_fnames.push_back(p.string());
				std::cout << "Done." << std::endl;
			}
		}
	}
	return errors;
}

int SOEntDataBase::load_ents(lua_State* L, std::vector<std::string> names) {
	for (unsigned n = 0; n < names.size(); n++) {
		std::cout << "Executing '" << (names.at(n) + "_init").c_str() << "'... ";
		lua_getglobal(L, (names.at(n) + "_init").c_str());
		// -1: [name]_init
		if (lua_pcall(L, 0, 0, 0) != 0) {
			// -1: error string, -2: [name]_init
			std::cout << "Error during execution: " << lua_tostring(L, -1) << std::endl;
			break;
		}

		Packet P{
			get_table_entry(L, "_E", "x"),
			get_table_entry(L, "_E", "y"),
			get_table_entry(L, "_E", "w"),
			get_table_entry(L, "_E", "h"),
			get_table_entry(L, "_E", "tox"),
			get_table_entry(L, "_E", "toy"),
			get_table_entry(L, "_E", "tw"),
			get_table_entry(L, "_E", "th"),
			get_table_entry(L, "_E", "u"),
			get_table_entry(L, "_E", "v"),
			get_table_entry(L, "_E", "uvw"),
			get_table_entry(L, "_E", "uvh")
		};
		
		std::vector<std::string> keys;
		std::vector<float> values;
		std::vector<DataTag> Dtable;
		lua_getglobal(L, "_D");
		size_t table_size = lua_rawlen(L, -1);
		if (table_size % 2 != 0)
			table_size--;

		lua_pushnil(L);
		int i = 0;
		while (lua_next(L, -2) != 0 && i < table_size) {
			// -1: value, -2: key, -3: table
			if (i % 2 == 0)
				keys.push_back(lua_tostring(L, -1));
			else
				values.push_back((float)lua_tonumber(L, -1));
			lua_pop(L, 1);
			// -1: key, -2: table
			i++;
		}
		for (unsigned n = 0; n < table_size / 2; n++)
			Dtable.push_back(DataTag(keys.at(n), values.at(n)));

		std::vector<std::vector<float>> sframes;
		/*
		i = 0;
		std::string num = "0";
		std::string frametable;
		while (true) {
			frametable = std::string("_F") + num;
			lua_getglobal(L, frametable.c_str());
			if (lua_istable(L, -1)) {
				sframes.push_back({});
				sframes.at(i).push_back(get_table_entry(L, frametable.c_str(), "tox"));
				sframes.at(i).push_back(get_table_entry(L, frametable.c_str(), "toy"));
				sframes.at(i).push_back(get_table_entry(L, frametable.c_str(), "tw"));
				sframes.at(i).push_back(get_table_entry(L, frametable.c_str(), "th"));
				sframes.at(i).push_back(get_table_entry(L, frametable.c_str(), "u"));
				sframes.at(i).push_back(get_table_entry(L, frametable.c_str(), "v"));
				sframes.at(i).push_back(get_table_entry(L, frametable.c_str(), "uvw"));
				sframes.at(i).push_back(get_table_entry(L, frametable.c_str(), "uvh"));
			}
			else
				break;
			i++;
			num[0] += 1;
		}
		*/
		/*
		while (true) {
			std::string s = "0";
			s[0] += i;
			lua_getglobal(L, std::string(std::string("_F") + s).c_str());
			if (lua_istable(L, -1)) {
				sframes.push_back({});
				lua_pushnil(L);
				int m = 0;
				while (lua_next(L, -2) != 0) {
					// -1: value, -2: key, -3: table
					(sframes.at(i)).push_back((float)lua_tonumber(L, -1));
					lua_pop(L, 1);
					// -1: key, -2: table
					m++;
				}
			}
			else
				break;
			i++;
		}
		*/
		lua_getglobal(L, "_T");
		EData.push_back(EntData(names.at(n), P, Dtable, (EntType)((int)lua_tonumber(L, -1)), sframes));
		std::cout << "Done." << std::endl;
	}
	return 0;
}

EntData SOEntDataBase::get(unsigned index) {
	if (index < EData.size())
		return EData.at(index);
	else
		return EntData();
}
EntData SOEntDataBase::get(std::string name) {
	for (unsigned n = 0; n < EData.size(); n++) {
		if (EData.at(n).name == name)
			return EData.at(n);
	}
	return EntData();
}

float SOEntDataBase::get_table_entry(lua_State* L, std::string table_name, std::string key) {
	lua_getglobal(L, table_name.c_str());
	lua_pushstring(L, key.c_str());
	// -1: key, -2: table
	lua_gettable(L, -2);
	// -1: value, -2: key, -3: table
	return (float)lua_tonumber(L, -1);
}