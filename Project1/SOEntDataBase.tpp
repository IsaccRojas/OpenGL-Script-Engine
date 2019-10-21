#pragma once

template<typename T>
T *SOEntDataBase::get_func(std::string func) {
	for (int i = 0; i < libs.size(); i++) {
		if (libs.at(i).has(func))
			return libs.at(i).get<T>(func);
	}
	return nullptr;
}