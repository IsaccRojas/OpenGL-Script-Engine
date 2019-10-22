#pragma once

template<typename T>
int SOEntDataBase::get_func(std::string func_name, boost::function<T> *func) {
	for (unsigned i = 0; i < libs.size(); i++) {
		if (libs.at(i).has(func_name)) {
			*func = libs.at(i).get<T>(func_name);
			return 0;
		}
	}
	return -1;
}

/*
template<typename T>
T *SOEntDataBase::get_func(std::string func) {
	for (unsigned i = 0; i < libs.size(); i++) {
		if (libs.at(i).has(func))
			return libs.at(i).get<T>(func);
	}
	return nullptr;
}
*/