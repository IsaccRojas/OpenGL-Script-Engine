#pragma once

#ifndef SOENTDATABASE_H_
#define SOENTDATABASE_H_

#include "SOEnt.h"

#include <experimental/filesystem>
#include <iostream>

class SOEntDataBase {
	std::vector<std::string> script_fnames;
	std::vector<boost::dll::shared_library> libs;
public:
	std::vector<EntData> EData;

	SOEntDataBase();

	//clears passed library vector and populates it with sources
	int load_all(std::string dir);
	
	/*loads entity data from library scripts;
	calls EntData {init_func}(void) in all loaded libraries and populates local entity data vector*/
	//int load_ent_data(std::string init_func);

	//get pointer to function 'func'
	template<typename T>
	T *get_func(std::string func);
};

#endif