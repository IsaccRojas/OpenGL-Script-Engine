#pragma once

#ifndef SOTEXT_H_
#define SOTEXT_H_

#include "SOEnt.h"

/*
0 - kill
1 - spacing
*/

class SOText : public SOEnt {
	std::vector<Entity*> char_data;
	int buffer_size;
	std::string text;
	float t_spacing, t_au, t_av, t_uvw, t_uvh;
public:
	SOText(EntData EData, SOBaseScript* master_ptr);
	void base_script() override;
	void run(Abstr_Dispatcher &dispatcher) override;

	void alloc_chars(int n);
	void set(std::string str);
	void b_set(int n, std::string str);
	std::string get();
};

#endif