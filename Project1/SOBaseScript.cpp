#include "SOBaseScript.h"

SOBaseScript::SOBaseScript(Resources* resources, int *err) : res(resources) {
	//store each texture's data and information in array
	textures[ET_PL] = MakeTextureImage(GL_TEXTURE_2D, "img/img.png", 1, 1);
	//textures[ET_2] = MakeTextureImage(GL_TEXTURE_2D, "img/2.png", 1, 2);
	//textures[ET_3] = MakeTextureImage(GL_TEXTURE_2D, "img/3.png", 1, 3);
	//textures[ET_4] = MakeTextureImage(GL_TEXTURE_2D, "img/4.png", 1, 4);
	//textures[ET_5] = MakeTextureImage(GL_TEXTURE_2D, "img/5.png", 1, 5);
	//textures[ET_6] = MakeTextureImage(GL_TEXTURE_2D, "img/6.png", 1, 6);
	
	if (DB.load("C:/Users/isacc/source/repos/Project1/Debug/Dll1.dll") != 0) {
		std::cout << "ERROR: could not load lib" << std::endl;
		*err = -1;
	}
	
	TBuff = new EBuffer();
	TVertBuf = new GLBuffer<float>(GL_ARRAY_BUFFER, &(TBuff->vData), GL_DYNAMIC_DRAW);
	TElBuf = new GLBuffer<unsigned>(GL_ELEMENT_ARRAY_BUFFER, &(TBuff->vElements), GL_DYNAMIC_DRAW);

	TBuff->update();
	TVertBuf->update();   
	TElBuf->update();

	EBuff = new EBuffer();
	VertBuf = new GLBuffer<float>(GL_ARRAY_BUFFER, &(EBuff->vData), GL_DYNAMIC_DRAW);
	ElBuf = new GLBuffer<unsigned>(GL_ELEMENT_ARRAY_BUFFER, &(EBuff->vElements), GL_DYNAMIC_DRAW);

	std::cout << *err << " error(s) setting up SOBaseScript." << std::endl;

	API_globals = _API_global::getInst();

	if (*err == 0) {
		gen_ent("SOEnt_Pl");

		gen.seed(unsigned(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		distr = std::uniform_real_distribution<float>(-96.0f, 96.0f);
		for (int i = 0; i < 8; i++) {
			gen_ent("SOEnt_G");
			MVEntities.at(i + 1)->getEntPointer().spos(distr(gen), distr(gen));
		}
	}

	frame = 0;
	cursorx = 0;
	cursory = 0;
}

void SOBaseScript::render(unsigned vbo, unsigned vebo) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vebo);

	glEnableVertexAttribArray(res->vAttribs.at(0).index);
	glVertexAttribPointer(res->vAttribs.at(0).index, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	glEnableVertexAttribArray(res->vAttribs.at(1).index);
	glVertexAttribPointer(res->vAttribs.at(1).index, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(res->vAttribs.at(2).index);
	glVertexAttribPointer(res->vAttribs.at(2).index, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 4));

	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)0);
}

void SOBaseScript::base_script() {
	//int oldcursx = cursorx;
	//int oldcursy = cursory;
	auto click = SDL_GetMouseState(&cursorx, &cursory);
	cursorx = int(cursorx * (res->getRenderWidth() / res->getWindowDims().x));
	cursory = int(cursory * (res->getRenderWidth() / res->getWindowDims().y));
	cursorx = int(cursorx - (res->getRenderWidth() / 2));
	cursory = int(-1 * (cursory - (res->getRenderWidth() / 2)));
	auto input = SDL_GetKeyboardState(NULL);

	API_globals->_setMouse(vec2((float)cursorx, (float)cursory));
	API_globals->_setKeyInput((uint8_t*)input);

	//if (click & SDL_BUTTON(SDL_BUTTON_LEFT))
	//	lua_pushnumber(L, 1);
	//if (click & SDL_BUTTON(SDL_BUTTON_RIGHT))
	//	lua_pushnumber(L, 1);

	for (int n = 0; n < MVEntities.size(); n++) {
		if (MVEntities.exists(n)) {
			MVEntities.at(n)->base_script();

			while (MVEntities.at(n)->API->wr_q.size() > 0) {
				EntPage pg = MVEntities.at(n)->API->wr_q.back();
				MVEntities.at(n)->API->wr_q.pop_back();
				MVEntities.at(pg.index)->setPage(pg);
			}

			if (MVEntities.at(n)->getDataTag(0) == 2)
				killed_entities.push_back(MVEntities.at(n));
		}
	}

	for (unsigned n = 0; n < killed_entities.size(); n++)
		del_ent(killed_entities.at(n)->getPage());
	killed_entities.clear();
	
	glClear(GL_COLOR_BUFFER_BIT);
	render(TVertBuf->getIndex(), TElBuf->getIndex());
	EBuff->update();
	VertBuf->update();
	ElBuf->update();
	render(VertBuf->getIndex(), ElBuf->getIndex());
	SDL_GL_SwapWindow(res->getWindow());
	//std::cout << "FPS " << get_fps() << std::endl;
	frame++;
}

Entity* SOBaseScript::EBuffAlloc(Packet p, Image imgTex, int prior) {
	if (imgTex.texture_unit == ET_1)
		return TBuff->allocateEntity(p, imgTex, prior);
	else
		return EBuff->allocateEntity(p, imgTex, prior);
}

int SOBaseScript::EBuffDealloc(Entity* Ent) {
	return EBuff->erase(Ent);
}

EntPage SOBaseScript::gen_ent(std::string name) {
	boost::function<SOEnt*(Entity*, MemVec<SOEnt*>*)> dl_genf;
	if (DB.get_func<SOEnt*(Entity*, MemVec<SOEnt*>*)>(std::string("gen_") + name, &dl_genf) != 0) {
		std::cout << "ERROR: could not get entity gen function" << std::endl;
		return EntPage();
	}

	Entity* E = EBuffAlloc(Packet{}, getTexture(1), 0);
	SOEnt* Ent = dl_genf(E, &MVEntities);
	
	Ent->setIndex(MVEntities.push(Ent));
	Ent->API->setGlobals(API_globals);
	return Ent->getPage();
}

void SOBaseScript::del_ent(EntPage pg) {
	SOEnt* Ent = MVEntities.at(pg.index);

	EBuff->erase(Ent->getEntPointer().gent());
	MVEntities.erase_at(pg.index);

	delete Ent;
}

Image SOBaseScript::getTexture(int index) {
	return textures[index];
}

float SOBaseScript::get_fps() {
	newt = std::chrono::steady_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(oldt - newt).count();
	oldt = newt;
	return fabs(1000.0f / float(time));
}

/*
void SOBaseScript::set_map(float* tilepos) {

	for (int n = 0; n < 16; n++) {
		for (int m = 0; m < 16; m++) {
			if (tilepos[(n * 16) + m]) {
				//tiles.push_back(new SOEnt(DB.get("Tile1"), this));
			}
			else {
				//tiles.push_back(new SOEnt(DB.get("Tile0"), this));
				if (n - 1 >= 0 && tilepos[((n - 1) * 16) + m])
					tiles.back()->E.suvpos(48, tiles.back()->E.guvpos().y);
			}
			tiles.back()->E.spos(float(16 * (m + 1) - 128 - 8), float(-1 * (16 * (n + 1) - 128) + 8));
			tiles.back()->index = tiles.size() - 1;
		}
	}
}
*/

Resources SOBaseScript::getResources() {
	return *res;
}

/*
int SOBaseScript::l_gen_ent(lua_State* L) {
	lua_pushlightuserdata(L, ((SOBaseScript*)lua_touserdata(L, 1))->gen_ent(lua_tostring(L, 2)));
	return 1;
}
int SOBaseScript::l_set_kill(lua_State* L) {
	((SOEnt*)lua_touserdata(L, 1))->data[0].n = (float)lua_tonumber(L, 2);
	return 0;
}
int SOBaseScript::l_get_kill(lua_State* L) {
	lua_pushnumber(L, ((SOEnt*)lua_touserdata(L, 1))->data[0].n);
	return 1;
}
int SOBaseScript::l_get_master(lua_State* L) {
	return 0;
}
int SOBaseScript::l_get_ent(lua_State* L) {
	if (lua_isnumber(L, 2)) {
		SOEnt* Ent = (((SOBaseScript*)lua_touserdata(L, 1))->entities.at((int)lua_tonumber(L, 2)));
		if (Ent != nullptr)
			lua_pushlightuserdata(L, Ent);
		else
			lua_pushlightuserdata(L, NULL);
	}
	return 1;
}
int SOBaseScript::l_get_ent_all_t(lua_State* L) {
	//...
	return 1;
}
int SOBaseScript::l_get_ent_all_tn(lua_State* L) {
	if (lua_isnumber(L, 2) && lua_isstring(L, 3)) {
		SOBaseScript* scriptptr = (SOBaseScript*)lua_touserdata(L, 1);
		unsigned size = scriptptr->entities.size();
		unsigned type = (unsigned)lua_tonumber(L, 2);
		std::string name = lua_tostring(L, 3);
		SOEnt* Ent;
		lua_newtable(L);
		unsigned ind = 0;
		for (unsigned i = 0; i < size; i++) {
			Ent = scriptptr->entities.at(i);
			if (Ent->ent_type == type && Ent->name == name) {
				lua_pushnumber(L, ind);
				lua_pushlightuserdata(L, Ent);
				lua_settable(L, -3);
				ind++;
			}
		}
		lua_pushnumber(L, ind);
	}
	return 2;
}
int SOBaseScript::l_get_ent_type(lua_State* L) {
	lua_pushnumber(L, ((SOEnt*)lua_touserdata(L, 1))->ent_type);
	return 1;
}
int SOBaseScript::l_get_entcount(lua_State* L) {
	lua_pushnumber(L, ((SOBaseScript*)lua_touserdata(L, 1))->entities.size());
	return 1;
}
int SOBaseScript::l_get_input(lua_State* L) {
	auto input = SDL_GetKeyboardState(NULL);
	lua_newtable(L);
	lua_pushstring(L, "W");
	lua_pushnumber(L, input[SDL_SCANCODE_W]);
	lua_settable(L, -3);
	lua_pushstring(L, "A");
	lua_pushnumber(L, input[SDL_SCANCODE_A]);
	lua_settable(L, -3);
	lua_pushstring(L, "S");
	lua_pushnumber(L, input[SDL_SCANCODE_S]);
	lua_settable(L, -3);
	lua_pushstring(L, "D");
	lua_pushnumber(L, input[SDL_SCANCODE_D]);
	lua_settable(L, -3);
	lua_pushstring(L, "UP");
	lua_pushnumber(L, input[SDL_SCANCODE_UP]);
	lua_settable(L, -3);
	lua_pushstring(L, "LEFT");
	lua_pushnumber(L, input[SDL_SCANCODE_LEFT]);
	lua_settable(L, -3);
	lua_pushstring(L, "DOWN");
	lua_pushnumber(L, input[SDL_SCANCODE_DOWN]);
	lua_settable(L, -3);
	lua_pushstring(L, "RIGHT");
	lua_pushnumber(L, input[SDL_SCANCODE_RIGHT]);
	lua_settable(L, -3);
	return 1;
}

int SOBaseScript::l_getpos(lua_State* L) {
	SOEnt* entp = ((SOEnt*)lua_touserdata(L, 1));
	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, entp->E.gpos().x);
	lua_settable(L, -3);
	lua_pushstring(L, "y");
	lua_pushnumber(L, entp->E.gpos().y);
	lua_settable(L, -3);
	return 1;
}
int SOBaseScript::l_getdim(lua_State* L) {
	SOEnt* entp = ((SOEnt*)lua_touserdata(L, 1));
	lua_newtable(L);
	lua_pushstring(L, "w");
	lua_pushnumber(L, entp->E.gdim().x);
	lua_settable(L, -3);
	lua_pushstring(L, "h");
	lua_pushnumber(L, entp->E.gdim().y);
	lua_settable(L, -3);
	return 1;
}
int SOBaseScript::l_gettoff(lua_State* L) {
	SOEnt* entp = ((SOEnt*)lua_touserdata(L, 1));
	lua_newtable(L);
	lua_pushstring(L, "tox");
	lua_pushnumber(L, entp->E.gtoff().x);
	lua_settable(L, -3);
	lua_pushstring(L, "toy");
	lua_pushnumber(L, entp->E.gtoff().y);
	lua_settable(L, -3);
	return 1;
}
int SOBaseScript::l_gettdim(lua_State* L) {
	SOEnt* entp = ((SOEnt*)lua_touserdata(L, 1));
	lua_newtable(L);
	lua_pushstring(L, "tw");
	lua_pushnumber(L, entp->E.gtdim().x);
	lua_settable(L, -3);
	lua_pushstring(L, "th");
	lua_pushnumber(L, entp->E.gtdim().y);
	lua_settable(L, -3);
	return 1;
}
int SOBaseScript::l_getuv(lua_State* L) {
	SOEnt* entp = ((SOEnt*)lua_touserdata(L, 1));
	lua_newtable(L);
	lua_pushstring(L, "u");
	lua_pushnumber(L, entp->E.guvpos().x);
	lua_settable(L, -3);
	lua_pushstring(L, "v");
	lua_pushnumber(L, entp->E.guvpos().y);
	lua_settable(L, -3);
	return 1;
}
int SOBaseScript::l_getuvdim(lua_State* L) {
	SOEnt* entp = ((SOEnt*)lua_touserdata(L, 1));
	lua_newtable(L);
	lua_pushstring(L, "uvw");
	lua_pushnumber(L, entp->E.guvdim().x);
	lua_settable(L, -3);
	lua_pushstring(L, "uvh");
	lua_pushnumber(L, entp->E.guvdim().y);
	lua_settable(L, -3);
	return 1;
}
int SOBaseScript::l_getdatas(lua_State* L) {
	SOEnt* entp = ((SOEnt*)lua_touserdata(L, 1));
	std::string str = lua_tostring(L, 2);
	for (unsigned n = 0; n < entp->data.size(); n++) {
		if (str == entp->data[n].name) {
			lua_pushnumber(L, entp->data[n].n);
			return 1;
		}
	}
	lua_pushnumber(L, 0.0f);
	return 1;
}
int SOBaseScript::l_getdatai(lua_State* L) {
	lua_pushnumber(L, ((SOEnt*)lua_touserdata(L, 1))->data[(int)lua_tonumber(L, 2)].n);
	return 1;
}

int SOBaseScript::l_setpos(lua_State* L) {
	((SOEnt*)lua_touserdata(L, 1))->E.spos((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
	return 0;
}
int SOBaseScript::l_setdim(lua_State* L) {
	((SOEnt*)lua_touserdata(L, 1))->E.sdim((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
	return 0;
}
int SOBaseScript::l_settoff(lua_State* L) {
	((SOEnt*)lua_touserdata(L, 1))->E.stoff((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
	return 0;
}
int SOBaseScript::l_settdim(lua_State* L) {
	((SOEnt*)lua_touserdata(L, 1))->E.stdim((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
	return 0;
}
int SOBaseScript::l_setuv(lua_State* L) {
	((SOEnt*)lua_touserdata(L, 1))->E.suvpos((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
	return 0;
}
int SOBaseScript::l_setuvdim(lua_State* L) {
	((SOEnt*)lua_touserdata(L, 1))->E.suvdim((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
	return 0;
}
int SOBaseScript::l_setdatas(lua_State* L) {
	SOEnt* entp = ((SOEnt*)lua_touserdata(L, 1));
	std::string str = lua_tostring(L, 2);
	for (unsigned n = 0; n < entp->data.size(); n++) {
		if (str == entp->data[n].name) {
			entp->data[n].n = (float)lua_tonumber(L, 3);
			return 0;
		}
	}
	return 0;
}
int SOBaseScript::l_setdatai(lua_State* L) {
	((SOEnt*)lua_touserdata(L, 1))->data[(int)lua_tonumber(L, 2)].n = (float)lua_tonumber(L, 3);
	return 0;
}
int SOBaseScript::l_setframe(lua_State* L) {
	SOEnt* entp = (SOEnt*)lua_touserdata(L, 1);
	unsigned i = (unsigned)lua_tonumber(L, 2);
	entp->E.stoff(entp->frames[i][0], entp->frames[i][1]);
	entp->E.stdim(entp->frames[i][2], entp->frames[i][3]);
	entp->E.suvpos(entp->frames[i][4], entp->frames[i][5]);
	entp->E.suvdim(entp->frames[i][6], entp->frames[i][7]);
	return 0;
}
int SOBaseScript::l_settext(lua_State* L) {
	SOText* entp = ((SOText*)lua_touserdata(L, 1));
	std::string str = lua_tostring(L, 2);
	if (entp->ent_type != ET_TEXT) {
		std::cout << "l_settext: ERROR: entity is not text" << std::endl;
		return 0;
	}
	std::cout << "l_settext: entp is " << entp << std::endl;
	entp->b_set(str.size(), str.c_str());
	return 0;
}
*/