#include "EBuffer.h"

Entity* EBuffer::alloc(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image imgTex, int prior) {
	if (vEnts.size() == 0) {
		highest_priority = prior;
		lowest_priority = prior;
	}
	unsigned int new_elements[] = {
		0 + index_offset, 1 + index_offset, 2 + index_offset,
		2 + index_offset, 3 + index_offset, 1 + index_offset
	};
	index_offset += 4;
	for (unsigned int e = 0; e < Entity::indicesperentitye; e++) {
		vElements.push_back(new_elements[e]);
	}
	this->count++;
	if (prior <= highest_priority) {
		highest_priority = prior;
		this->vEnts.push_back(Entity(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, imgTex, prior));
		vEnts.back().bPriorityChange = 0;
		vEnts.back().master = this;
		this->dataResizeReset(1, -1);

		return &(vEnts.back());
	}
	else {
		if (prior >= lowest_priority) {
			lowest_priority = prior;
			this->vEnts.insert(this->vEnts.begin(), Entity(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, imgTex, prior));
			(vEnts.begin())->bPriorityChange = 0;
			(vEnts.begin())->master = this;
			this->dataResizeReset(1, 0);

			return &(*vEnts.begin());
		}
		else {
			unsigned int e = 0;
			std::list<Entity>::iterator iter;
			for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
				if (iter->getPriority() <= prior) break;
				e++;
			}
			e--;
			iter--;
			this->vEnts.insert(iter, Entity(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, imgTex, prior));
			iter->bPriorityChange = 0;
			iter->master = this;
			this->dataResizeReset(1, e - 1);

			return &(*iter);
		}
	}
}
Entity* EBuffer::alloc(Packet p, Image imgTex, int prior) {
	if (vEnts.size() == 0) {
		highest_priority = prior;
		lowest_priority = prior;
	}
	unsigned int new_elements[] = {
		0 + index_offset, 1 + index_offset, 2 + index_offset,
		2 + index_offset, 3 + index_offset, 1 + index_offset
	};
	index_offset += 4;
	for (unsigned int e = 0; e < Entity::indicesperentitye; e++) {
		vElements.push_back(new_elements[e]);
	}
	this->count++;
	if (prior <= highest_priority) {
		highest_priority = prior;
		this->vEnts.push_back(Entity(p, imgTex, prior));
		vEnts.back().bPriorityChange = 0;
		vEnts.back().master = this;
		this->dataResizeReset(1, -1);

		return &(vEnts.back());
	}
	else {
		if (prior >= lowest_priority) {
			lowest_priority = prior;
			this->vEnts.insert(this->vEnts.begin(), Entity(p, imgTex, prior));
			(vEnts.begin())->bPriorityChange = 0;
			(vEnts.begin())->master = this;
			this->dataResizeReset(1, 0);

			return &(*vEnts.begin());
		}
		else {
			unsigned int e = 0;
			std::list<Entity>::iterator iter;
			for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
				if (iter->getPriority() <= prior) break;
				e++;
			}
			e--;
			iter--;
			this->vEnts.insert(iter, Entity(p, imgTex, prior));
			iter->bPriorityChange = 0;
			iter->master = this;
			this->dataResizeReset(1, e - 1);

			return &(*iter);
		}
	}
}

EBuffer::EBuffer() {
	highest_priority = 999;
	lowest_priority = -999;
	count = 0;
	EBuff_ID = count_existed;
	count_existed++;
	index_offset = 0;
}

Entity* EBuffer::allocateEntity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image imgTex, int prior) {
	return alloc(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, imgTex, prior);
}
Entity* EBuffer::allocateEntity(Packet p, Image imgTex, int prior) {
	return alloc(p, imgTex, prior);
}

void EBuffer::allocateEntity() {
	unsigned int new_elements[] = {
		0 + index_offset, 1 + index_offset, 2 + index_offset,
		2 + index_offset, 3 + index_offset, 1 + index_offset
	};
	index_offset += 4;
	for (unsigned int e = 0; e < Entity::indicesperentitye; e++) {
		vElements.push_back(new_elements[e]);
	}
	this->count++;
	this->vEnts.push_back(Entity());
	vEnts.back().bPriorityChange = 0;
	vEnts.back().bFree = 1;
	vEnts.back().master = this;
	this->vFreeEnts.push_back(&(vEnts.back()));
	this->dataResizeReset(1, -1);
}

Entity* EBuffer::newEntity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image img, int prior) {
	if (this->vFreeEnts.size() > 0) {
		this->vFreeEnts.back()->dataNew(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, img, prior);
		Entity* E = this->vFreeEnts.back();
		this->vFreeEnts.pop_back();
		return E;
	}
	else
		return nullptr;
}
Entity* EBuffer::newEntity(Packet p, Image img, int prior) {
	if (this->vFreeEnts.size() > 0) {
		this->vFreeEnts.back()->dataNew(p, img, prior);
		Entity* E = this->vFreeEnts.back();
		this->vFreeEnts.pop_back();
		return E;
	}
	else
		return nullptr;
}

Entity* EBuffer::getEnt(unsigned int p_ID) {
	Entity* E;
	std::list<Entity>::iterator iter;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		if (iter->ID == p_ID) {
			E = &(*iter);
			return E;
		}
	}
	return nullptr;
}

unsigned int EBuffer::getEBuff_ID() {
	return EBuff_ID;
}

void EBuffer::update() {
	std::list<Entity>::iterator iter;
	std::vector<float>::iterator iterv;
	unsigned n = 0;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		iter->index = n;
		if (iter->bChangeSinceLastCheck) {
			vData.erase(vData.begin() + n * Entity::indicesperentityv, vData.begin() + n * Entity::indicesperentityv + Entity::indicesperentityv);
			vData.insert(vData.begin() + n * Entity::indicesperentityv, iter->vVertices.begin(), iter->vVertices.end());
		}
		n++;
	}
}

int EBuffer::erase(Entity* ent) {
	std::list<Entity>::iterator iter;
	unsigned n = 0;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++, n++)
		if (iter->ID == ent->ID) break;
	if (iter != vEnts.end()) {
		iter = vEnts.erase(iter);
		vData.erase(vData.begin() + n * Entity::indicesperentityv, vData.begin() + n * Entity::indicesperentityv + Entity::indicesperentityv);
		for (unsigned n = 0; n < 6; n++)
			vElements.pop_back();
		index_offset -= 4;
		return 0;
	}
	else
		return -1;
}

void EBuffer::dataResizeReset(int dir, int position) {
	if (dir == 0) {
		vData.clear();
		int R = 0;
		std::list<Entity>::iterator iter;
		for (iter = vEnts.begin(); iter != vEnts.end(); iter++)
			R += iter->vVertices.size();
		vData.reserve(R);
		for (iter = vEnts.begin(); iter != vEnts.end(); iter++)
			vData.insert(vData.end(), iter->vVertices.begin(), iter->vVertices.end());
	}
	else if (dir == 1) {
		if (position == -1)
			vData.insert(vData.end(), vEnts.back().vVertices.begin(), vEnts.back().vVertices.end());
		else if (position == 0)
			vData.insert(vData.begin(), vEnts.back().vVertices.begin(), vEnts.back().vVertices.end());
		else {
			std::list<Entity>::iterator iter;
			for (int e = 0; e < position; e++)
				iter++;
			vData.insert(vData.begin() + (position * (vEnts.begin())->vVertices.size()), iter->vVertices.begin(), iter->vVertices.end());
		}
	}
	else if (dir == -1) {
		if (position == -1)
			vData.erase(vData.end() - (vEnts.begin())->vVertices.size(), vData.end());
		else if (position == 0)
			vData.erase(vData.begin(), vData.begin() + (vEnts.begin())->vVertices.size());
		else
			vData.erase(vData.begin() + (position * (vEnts.begin())->vVertices.size()), vData.begin() + ((position * (vEnts.begin())->vVertices.size()) + (vEnts.begin())->vVertices.size()));
	}
}

void EBuffer::freeEntity(Entity* ent) {
	if (ent->master == this) {
		vFreeEnts.push_back(ent);
		ent->dataFree();
	}
}

int EBuffer::dataGetCount() { return this->count; }

unsigned int EBuffer::count_existed = 0;