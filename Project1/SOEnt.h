#pragma once

#ifndef SOENT_H_
#define SOENT_H_

#include "EBuffer/Entity.h"
#include <boost/dll/config.hpp>
#include <boost/dll.hpp>

class SOEnt;

template <class S>
struct MVNode {
	MVNode(S x) : data(x) {
		exists = true;
	};
	bool exists;
	S data;
};

template <class T>
class MemVec {
	std::vector<MVNode<T>> mainvec;
	std::vector<int> freevec;
	int sz;
public:
	MemVec() {
		sz = 0;
	}
	~MemVec() {
		this->clear();
	}
	int push(T x) {
		if (freevec.empty()) {
			mainvec.push_back(x);
			sz++;
			return sz - 1;
		}
		int index = freevec.back();
		freevec.pop_back();
		mainvec.at(index).data = x;
		mainvec.at(index).exists = true;
		return index;
	}
	void erase_at(int i) {
		if (mainvec.at(i).exists) {
			mainvec.at(i).exists = false;
			freevec.push_back(i);
		}
	}
	bool exists(int i) {
		return mainvec.at(i).exists;
	}
	T at(int i) {
		return mainvec.at(i).data;
	}
	bool empty() {
		return (sz == 0);
	}
	void clear() {
		mainvec.clear();
		freevec.clear();
		sz = 0;
	}
	int size() {
		return sz;
	}
};

struct DataTag {
	DataTag();
	DataTag(std::string tag_name, float tag_n);
	std::string name;
	float n;
};

class EntPage {
	friend class API;
private:
	SOEnt *owner;
	bool b_empty;
public:
	EntPage();
	EntPage(Packet p, std::vector<DataTag> dt, int i);
	bool empty();
	Packet P;
	std::vector<DataTag> DT;
	int index;
};

class SOEnt {
	//API_push(SOEnt *ent);
protected:
	EntPointer E;
	std::vector<DataTag> DT;
	int index;
public:
	int kill;
	SOEnt(Entity *ent_ptr = nullptr);
	virtual void base_script();

	EntPointer getEntPointer();
	void setEnt(Entity *ent_ptr);
	
	int getIndex();
	void setIndex(int i);

	float getDataTag(int i);
	void setDataTag(int i, float v);

	EntPage getPage();
	void setPage(EntPage pg);
};

class API {
	//friend SOEnt::API_push(SOEnt*);
	//static structure
	std::vector<EntPage> wr_q;
public:
	API();
	EntPage genEnt(std::string name);
	void delEnt(SOEnt* ent);
	EntPage readPage(SOEnt* ent);
	void writePage(EntPage pg);
};

#endif