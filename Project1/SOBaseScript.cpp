#include "SOBaseScript.h"

SOBaseScript::SOBaseScript(Resources* resources) : res(resources) {
	first_exec = true;
	texture = MakeTextureImage(GL_TEXTURE_2D, "img/sheet.png", 1);
	EBuff = new EBuffer(0, 0, 0, 0, texture);
	VertBuf = new GLBuffer<float>(GL_ARRAY_BUFFER, &(EBuff->vData), GL_DYNAMIC_DRAW);
	ElBuf = new GLBuffer<unsigned>(GL_ELEMENT_ARRAY_BUFFER, &(EBuff->vElements), GL_DYNAMIC_DRAW);
	entities.push_back(new SOPlayer(EBuff->allocateEntity(0.0f, 0.0f, 32.0f, 32.0f, -16.0f, 16.0f, 32.0f, 32.0f, 0.0f, 32.0f, 32.0f, 32.0f, "null", texture, 0), this));
	entities.push_back(new SONPC(EBuff->allocateEntity(128.0f, 128.0f, 32.0f, 32.0f, -16.0f, 16.0f, 32.0f, 32.0f, 32.0f, 0.0f, 32.0f, 32.0f, "null", texture, 0), this));
	entities.at(0)->index = 0;
	entities.at(1)->index = 1;
	srand(time(NULL)); // ex: rand() % 10 + 1 is a number in the range [1, 10]
}

void SOBaseScript::first() {
	frame = 0;
	first_exec = false;
}

void SOBaseScript::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(res->vAttribs.at(0).index);
	glVertexAttribPointer(res->vAttribs.at(0).index, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	glEnableVertexAttribArray(res->vAttribs.at(1).index);
	glVertexAttribPointer(res->vAttribs.at(1).index, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)(sizeof(GLfloat) * 2));

	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)0);

	SDL_GL_SwapWindow(res->getWindow());
}

void SOBaseScript::base_script() {
	if (first_exec)
		first();

	if (frame % 120 == 0) {
		int x = int(bool(rand() % 2));
		int y = int(bool(rand() % 2));
		if (x == 0) x = -1;
		if (y == 0) y = -1;
		entities.push_back(new SONPC(EBuff->allocateEntity(272.0f * float(x), 272.0f * float(y), 32.0f, 32.0f, -16.0f, 16.0f, 32.0f, 32.0f, 32.0f, 0.0f, 32.0f, 32.0f, "null", texture, 0), this));
		entities.back()->index = entities.size() - 1;
	}

	for (unsigned n = 0; n < entities.size(); n++) {
		entities.at(n)->run(dispatcher);
		if (entities.at(n)->kill)
			killed_entities.push_back(entities.at(n));
	}

	for (unsigned n = 0; n < killed_entities.size(); n++)
		del_ent(killed_entities.at(n));
	killed_entities.clear();
	
	EBuff->update();
	VertBuf->update();
	ElBuf->update();
	render();
	frame++;
}

SOEnt* SOBaseScript::gen_ent(std::string name) {
	//entities.push_back(EDatabase.get(name), this));
	entities.push_back(new SOProjectile(EBuff->allocateEntity(0.0f, 0.0f, 0.0f, 0.0f, -8.0f, 8.0f, 16.0f, 16.0f, 0.0f, 0.0f, 32.0f, 32.0f, "null", texture, 1), this));
	entities.back()->index = entities.size() - 1;
	return entities.back();
}

void SOBaseScript::del_ent(SOEnt* soentity) {
	int ind = soentity->index;
	EBuff->erase(soentity->E.gent());
	entities.erase(entities.begin() + ind);
	for (unsigned n = ind; n < entities.size(); n++)
		(entities.at(n)->index)--;
}

Resources SOBaseScript::getResources() {
	return *res;
}