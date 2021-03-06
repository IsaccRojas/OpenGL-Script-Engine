#pragma once

#ifndef SOENTDATABASE_H_
#define SOENTDATABASE_H_

#include "SOEnt.h"

#include <boost/function.hpp>

#include <experimental/filesystem>
#include <iostream>

class SOEntDataBase {
	std::vector<std::string> script_fnames;
	std::vector<boost::dll::shared_library> libs;
public:
	SOEntDataBase();

	//pushes file to library vector
	int load(std::string file);
	//pushes all files in dir to library vector
	int load_all(std::string dir);
	
	/*loads entity data from library scripts;
	calls EntData {init_func}(void) in all loaded libraries and populates local entity data vector*/
	//int load_ent_data(std::string init_func);

	SOEnt* exec_func(std::string func, Entity* ent_ptr = nullptr);

	//get pointer to function 'func'
	template<typename T>
	int get_func(std::string func_name, boost::function<T> *func);
};

#include "SOEntDataBase.tpp"

#endif