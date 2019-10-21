#include "SOEntDataBase.h"

EntData::EntData() {
	name = "null";
	P = Packet();
	type = ET_ENT;
}
EntData::EntData(std::string ent_name, Packet packet, std::vector<DataTag> datatags, EntType ent_type, std::vector<std::vector<float>> ent_frames) : name(ent_name), P(packet), dt(datatags), type(ent_type), frames(ent_frames) {}

SOEntDataBase::SOEntDataBase() {
	libs.clear();
	script_fnames.clear();
}

int SOEntDataBase::load(std::string file) {
	std::cout << "Loading script '" << file << "'... ";
	try {
		boost::dll::shared_library lib(file.c_str());
		libs.push_back(lib);
		script_fnames.push_back(file);
	}
	catch (boost::exception&) {
		std::cout << " ERROR: could not load library '" << file << "'." << std::endl;
		return -1;
	}
	std::cout << "Done." << std::endl;
	return 0;
}

int SOEntDataBase::load_all(std::string dir) {
	int errors = 0;
	//libs.clear();
	//script_fnames.clear();

	//auto for loop to attempt to load all scripts
	for (std::experimental::filesystem::path p : std::experimental::filesystem::directory_iterator(dir)) {
		std::cout << "Loading script '" << p.string().c_str() << "'... ";

		try {
			boost::dll::shared_library lib(p.string().c_str());
			libs.push_back(lib);
			script_fnames.push_back(p.string());
		}
		catch (boost::exception&) {
			std::cout << " ERROR: could not load library '" << p.string().c_str() << "'." << std::endl;
			errors++;
		}

		std::cout << "Done." << std::endl;
	}
	return errors;
}

/*
int SOEntDataBase::load_ent_data(std::string init_func) {
int sz = libs.size();
if (sz == 0) {
std::cout << "ERROR: no libraries loaded." << std::endl;
return -1;
}

for (int i = 0; i < sz; i++) {
std::cout << "Executing '" << init_func << "' in '" << (script_fnames.at(i)).c_str() << "'... ";

auto func = libs.at(i).get<EntData(void)>(init_func.c_str());
EntData ED = func();
EData.push_back(ED);

std::cout << "Done." << std::endl;
}

return 0;
}
*/

SOEnt* SOEntDataBase::exec_func(std::string func, Entity* ent_ptr, SOBaseScript* basescript_ptr) {
	for (int i = 0; i < libs.size(); i++) {
		if (libs.at(i).has(func)) {
			std::cout << "found func" << std::endl;
			auto f = libs.at(i).get<SOEnt*(Entity*,SOBaseScript*)>(func);
			std::cout << "executing func(...)" << std::endl;
			auto r = f(ent_ptr, basescript_ptr);
			std::cout << "returning" << std::endl;
			return r;
		}
	}
	return nullptr;
}