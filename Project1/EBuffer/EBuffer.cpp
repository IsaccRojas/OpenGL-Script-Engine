#include "EBuffer.h"

Entity* EBuffer::alloc(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image imgTex, int prior) {
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
		this->vEnts.push_back(Entity(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, script_name, imgTex, prior));
		vEnts.back().bPriorityChange = 0;
		vEnts.back().master = this;
		this->dataResizeReset(1, -1);

		return &(vEnts.back());
	}
	else {
		if (prior >= lowest_priority) {
			lowest_priority = prior;
			this->vEnts.insert(this->vEnts.begin(), Entity(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, script_name, imgTex, prior));
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
			this->vEnts.insert(iter, Entity(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, script_name, imgTex, prior));
			iter->bPriorityChange = 0;
			iter->master = this;
			this->dataResizeReset(1, e - 1);

			return &(*iter);
		}
	}
}

EBuffer::EBuffer(unsigned int position_attribute_location, unsigned int texture_attribute_location, unsigned int vb_location, unsigned int eb_location, Image image) : position_loc(position_attribute_location), tex_loc(texture_attribute_location), vb_handle(vb_location), eb_handle(eb_location) {
	highest_priority = 999;
	lowest_priority = -999;
	count = 0;
	EBuff_ID = this->count_existed;
	this->count_existed++;
	index_offset = 0;
	imgTexture = image;

#ifdef _GL
	if (vb_location == -2) {
		glGenBuffers(1, &(this->vb_handle));
	}
	if (eb_location == -2) {
		glGenBuffers(1, &(this->eb_handle));
	}
#endif
}

Entity* EBuffer::allocateEntity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image imgTex, int prior) {
	return alloc(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, script_name, imgTex, prior);
}

#ifdef ENTBASE
Entity* EBuffer::allocateEntity(const char* name, float x, float y, float w, float h, int prior) {
	Packet P = EBase->getPacket(name);
	return alloc(x, y, w, h, P.toX, P.toY, P.tW, P.tH, P.U, P.V, P.UVW, P.UVH, P.script_filename.c_str(), P.tex, prior);
}
Entity* EBuffer::allocateEntity(unsigned int ebase_index, float x, float y, float w, float h, int prior) {
	Packet P = EBase->getPacket(ebase_index);
	return alloc(x, y, w, h, P.toX, P.toY, P.tW, P.tH, P.U, P.V, P.UVW, P.UVH, P.script_filename.c_str(), P.tex, prior);
}
#endif

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

Entity* EBuffer::newEntity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image img, int prior) {
	if (this->vFreeEnts.size() > 0) {
		this->vFreeEnts.back()->dataNew(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, script_name, img, prior);
		Entity* E = this->vFreeEnts.back();
		this->vFreeEnts.pop_back();
		return E;
	}
	else
		return nullptr;
}
#

#ifdef ENTBASE
Entity* EBuffer::newEntity(const char* name, float x, float y, float w, float h, int prior) {
	if (this->vFreeEnts.size() > 0) {
		this->vFreeEnts.back()->dataNew(x, y, w, h, this->EBase->getPacket(name), prior);
		Entity* E = this->vFreeEnts.back();
		this->vFreeEnts.pop_back();
		return E;
	}
	else
		return nullptr;
}
Entity* EBuffer::newEntity(unsigned int ebase_index, float x, float y, float w, float h, int prior) {
	if (this->vFreeEnts.size() > 0) {
		this->vFreeEnts.back()->dataNew(x, y, w, h, this->EBase->getPacket(ebase_index), prior);
		Entity* E = this->vFreeEnts.back();
		this->vFreeEnts.pop_back();
		return E;
	}
	else
		return nullptr;
}
#endif

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

#ifdef ENTBASE
void EBuffer::setEntBase(EntBase* ent_base) {
	EBase = ent_base;
}

EntBase* EBuffer::getEntBasePtr() {
	return EBase;
}
#endif

void EBuffer::update() {
	std::list<Entity>::iterator iter;
	std::vector<float>::iterator iterv;
	unsigned n = 0;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		iter->index = n;
		if (iter->bChangeSinceLastCheck) {
			vData.erase(vData.begin() + n * 16, vData.begin() + n * 16 + 16);
			vData.insert(vData.begin() + n * 16, iter->vVertices.begin(), iter->vVertices.end());
		}
		n++;
	}
}

void EBuffer::erase(Entity* ent) {
	std::list<Entity>::iterator iter;
	unsigned n = 0;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++, n++)
		if (iter->ID == ent->ID) break;
	iter = vEnts.erase(iter);
	vData.erase(vData.begin() + n * 16, vData.begin() + n * 16 + 16);
}

void EBuffer::dataResizeReset(int dir, int position) {
	if (dir == 0) {
		vData.clear();
		int R = 0;
		std::list<Entity>::iterator iter;
		for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
			R += iter->vVertices.size();
		}
		vData.reserve(R);
		for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
			vData.insert(vData.end(), iter->vVertices.begin(), iter->vVertices.end());
		}
#ifdef _GL
		glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vData.size(), vData.data(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*vElements.size(), vElements.data(), GL_DYNAMIC_DRAW);
#endif
	}
	else if (dir == 1) {
		if (position == -1) {
			vData.insert(vData.end(), vEnts.back().vVertices.begin(), vEnts.back().vVertices.end());
#ifdef _GL
			glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb_handle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vData.size(), vData.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*vElements.size(), vElements.data(), GL_DYNAMIC_DRAW);
#endif
		}
		else if (position == 0) {
			vData.insert(vData.begin(), vEnts.back().vVertices.begin(), vEnts.back().vVertices.end());
#ifdef _GL
			glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb_handle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vData.size(), vData.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*vElements.size(), vElements.data(), GL_DYNAMIC_DRAW);
#endif
		}
		else {
			std::list<Entity>::iterator iter;
			for (int e = 0; e < position; e++) {
				iter++;
			}
			vData.insert(vData.begin() + (position * (vEnts.begin())->vVertices.size()), iter->vVertices.begin(), iter->vVertices.end());
#ifdef _GL
			glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb_handle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vData.size(), vData.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*vElements.size(), vElements.data(), GL_DYNAMIC_DRAW);
#endif
		}
	}
	else if (dir == -1) {
		if (position == -1) {
			vData.erase(vData.end() - (vEnts.begin())->vVertices.size(), vData.end());
#ifdef _GL
			glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb_handle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vData.size(), vData.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*vElements.size(), vElements.data(), GL_DYNAMIC_DRAW);
#endif
		}
		else if (position == 0) {
			vData.erase(vData.begin(), vData.begin() + (vEnts.begin())->vVertices.size());
#ifdef _GL
			glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb_handle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vData.size(), vData.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*vElements.size(), vElements.data(), GL_DYNAMIC_DRAW);
#endif
		}
		else {
			vData.erase(vData.begin() + (position * (vEnts.begin())->vVertices.size()), vData.begin() + ((position * (vEnts.begin())->vVertices.size()) + (vEnts.begin())->vVertices.size()));
#ifdef _GL
			glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb_handle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vData.size(), vData.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*vElements.size(), vElements.data(), GL_DYNAMIC_DRAW);
#endif
		}
	}
}

#ifdef LUA_C
void EBuffer::dataRoutine() {
	std::list<Entity>::iterator iter;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		if (!(iter->bFree)) iter->routine();
	}
}
#endif

void EBuffer::dataCheck() {
	std::list<Entity>::iterator iter;
	/*
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		if (iter->bPriorityChange && !(iter->bFree)) {
			dataOrder();
			break;
		}
	}
	*/
#ifdef _GL
	glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
	glBindTexture(GL_TEXTURE_2D, this->imgTexture.tex);
#endif
	unsigned int e = 0;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		if (!(iter->bFree)) {
			iter->incClock();
			if (iter->bChangeSinceLastCheck) {
#ifdef _GL
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(e*(iter->vVertices.size())), sizeof(GLfloat)*(iter->vVertices.size()), iter->vVertices.data());
#endif
				iter->bChangeSinceLastCheck = 0;
			}
		}
		e++;
	}
}

void EBuffer::dataOrder() {
	/*
	std::list<Entity> E_temp;
	std::list<Entity> E_new;
	std::list<Entity>::iterator iter;
	std::list<Entity>::iterator iter2;
	std::vector<int> priorities;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		if (!(iter->bFree)) {
			E_temp.push_back((*iter));
		}
	}
	for (iter = E_temp.begin(); iter != E_temp.end(); iter++) {
		priorities.push_back(iter->getPriority());
	}
	std::sort(priorities.begin(), priorities.end());
	std::reverse(priorities.begin(), priorities.end());
	while (E_temp.size() > 0 && iter != E_temp.end()) {
		if (*priorities.begin() <= iter->getPriority()) {
			E_new.push_back(*iter);
			E_temp.erase(iter);
			priorities.erase(priorities.begin());
			iter = E_temp.begin();
			continue;
		}
		iter++;
	}
	for (iter = vEnts.begin(), iter2 = E_temp.begin(); iter2 != E_temp.end(); iter++, iter2++) {
		*iter = *iter2;
	}
	//this->vEnts = E_new;
	this->dataResizeReset(0, 0);
	for (iter = E_temp.begin(); iter != E_temp.end(); iter++) iter->bPriorityChange = 0;
	*/
}

void EBuffer::Draw(bool reset_data) {
#ifdef _GL
	glBindBuffer(GL_ARRAY_BUFFER, this->vb_handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eb_handle);
	glEnableVertexAttribArray(position_loc);
	glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	glEnableVertexAttribArray(tex_loc);
	glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)(sizeof(GLfloat) * 2));

	glDrawElements(GL_TRIANGLES, this->vElements.size(), GL_UNSIGNED_INT, 0);
#endif
}

void EBuffer::deallocateEntity(unsigned int p_ID) {
	unsigned int e = 0;
	std::list<Entity>::iterator iter;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		if (iter->ID == p_ID) {
			this->vEnts.erase(iter);
			break;
		}
	}
	for (unsigned int f = 0; f < Entity::indicesperentitye; f++) {
		this->vElements.pop_back();
	}
	index_offset -= 4;
	this->count--;
	this->dataResizeReset(-1, e);
}

void EBuffer::freeEntity(Entity* ent) {
	if (ent->master == this) {
		vFreeEnts.push_back(ent);
		ent->dataFree();
	}
}

void EBuffer::reportIDs() {
	std::list<Entity>::iterator iter;
	unsigned int e = 0;
	for (iter = vEnts.begin(); iter != vEnts.end(); iter++) {
		std::cout << e << ":" << iter->ID << " ";
		e++;
	}
	std::cout << std::endl;
}

int EBuffer::dataGetCount() { return this->count; }

unsigned int EBuffer::count_existed = 0;