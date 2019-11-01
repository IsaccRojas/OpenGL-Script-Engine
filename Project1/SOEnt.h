#pragma once

#ifndef SOENT_H_
#define SOENT_H_

#include "EBuffer/Entity.h"
#include <boost/dll/config.hpp>
#include <boost/dll.hpp>

class SOEnt;
class _API;

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
protected:
	EntPointer E;
	std::vector<DataTag> DT;
	int index;

public:
	SOEnt(Entity *ent_ptr = nullptr, MemVec<SOEnt*> *mv = nullptr);
	~SOEnt();

	virtual void base_script();

	EntPointer getEntPointer();
	void setEnt(Entity *ent_ptr);
	
	int getIndex();
	void setIndex(int i);

	float getDataTag(int i);
	void setDataTag(int i, float v);

	EntPage getPage();
	void setPage(EntPage &pg);

	_API *API;
};

/*
namespace _API_global {
	extern uint8_t *key_input;
	extern vec2 mouse_pos;
	extern uint8_t * _getKeyInput();
	extern vec2 _getMouse();
	extern void _setKeyInput(uint8_t *keys);
	extern void _setMouse(vec2 mouse);
}
*/

class _API_global {
	static _API_global *inst;
	int d;
	_API_global();
public:
	static _API_global *getInst();
	int get();
	void set(int data);
}

class _API {
	//friend SOEnt::API_push(SOEnt*);
	friend class SOBaseScript;

	MemVec<SOEnt*> *MV;
	std::vector<EntPage> wr_q;
	std::vector<std::string> gen_q;
public:
	_API(MemVec<SOEnt*> *mv);
	vec2 getMouse();
	uint8_t *getKeyInput();
	EntPage genEnt(std::string name);
	void delEnt(SOEnt* ent);
	EntPage readPage(int i);
	void writePage(EntPage &pg);
};

#endif