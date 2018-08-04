#include "SOBaseScript.h"

SOBaseScript::SOBaseScript(Resources* resources) : res(resources) {
	textures[0] = MakeTextureImage(GL_TEXTURE_2D, "img/sheet.png", 1, 0);
	textures[1] = MakeTextureImage(GL_TEXTURE_2D, "img/smoke.png", 1, 1);

	L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_pushcfunction(L, SOBaseScript::l_gen_ent);
	lua_setglobal(L, "gen_ent");
	lua_pushcfunction(L, SOBaseScript::l_set_kill);
	lua_setglobal(L, "set_kill");
	lua_pushcfunction(L, SOBaseScript::l_get_kill);
	lua_setglobal(L, "get_kill");
	lua_pushcfunction(L, SOBaseScript::l_get_master);
	lua_setglobal(L, "get_master");
	lua_pushcfunction(L, SOBaseScript::l_get_ent);
	lua_setglobal(L, "get_ent");
	lua_pushcfunction(L, SOBaseScript::l_get_ent_type);
	lua_setglobal(L, "get_ent_type");
	lua_pushcfunction(L, SOBaseScript::l_get_entcount);
	lua_setglobal(L, "get_entcount");
	lua_pushcfunction(L, SOBaseScript::l_get_input);
	lua_setglobal(L, "get_input");

	lua_pushcfunction(L, SOBaseScript::l_getpos);
	lua_setglobal(L, "getpos");
	lua_pushcfunction(L, SOBaseScript::l_getdim);
	lua_setglobal(L, "getdim");
	lua_pushcfunction(L, SOBaseScript::l_gettoff);
	lua_setglobal(L, "gettoff");
	lua_pushcfunction(L, SOBaseScript::l_gettdim);
	lua_setglobal(L, "gettdim");
	lua_pushcfunction(L, SOBaseScript::l_getuv);
	lua_setglobal(L, "getuv");
	lua_pushcfunction(L, SOBaseScript::l_getuvdim);
	lua_setglobal(L, "getuvdim");
	lua_pushcfunction(L, SOBaseScript::l_getdatas);
	lua_setglobal(L, "getdatas");
	lua_pushcfunction(L, SOBaseScript::l_getdatai);
	lua_setglobal(L, "getdatai");

	lua_pushcfunction(L, SOBaseScript::l_setpos);
	lua_setglobal(L, "setpos");
	lua_pushcfunction(L, SOBaseScript::l_setdim);
	lua_setglobal(L, "setdim");
	lua_pushcfunction(L, SOBaseScript::l_settoff);
	lua_setglobal(L, "settoff");
	lua_pushcfunction(L, SOBaseScript::l_settdim);
	lua_setglobal(L, "settdim");
	lua_pushcfunction(L, SOBaseScript::l_setuv);
	lua_setglobal(L, "setuv");
	lua_pushcfunction(L, SOBaseScript::l_setuvdim);
	lua_setglobal(L, "setuvdim");
	lua_pushcfunction(L, SOBaseScript::l_setdatas);
	lua_setglobal(L, "setdatas");
	lua_pushcfunction(L, SOBaseScript::l_setdatai);
	lua_setglobal(L, "setdatai");
	lua_pushcfunction(L, SOBaseScript::l_setframe);
	lua_setglobal(L, "setframe");

	std::cout << DB.load_all(L, "script") << " error(s) loading scripts." << std::endl;
	DB.load_ents(L, {
		"NPC",
		"Effect",
		"Projectile",
		"Player",
		"NPC2",
		"Projectile2"
		});

	EBuff = new EBuffer();
	VertBuf = new GLBuffer<float>(GL_ARRAY_BUFFER, &(EBuff->vData), GL_DYNAMIC_DRAW);
	ElBuf = new GLBuffer<unsigned>(GL_ELEMENT_ARRAY_BUFFER, &(EBuff->vElements), GL_DYNAMIC_DRAW);

	entities.push_back(new SOPlayer(DB.get("Player"), textures[0], this));
	entities.push_back(new SOEnt(DB.get("Effect"), textures[1], this));
	entities.at(0)->index = 0;
	entities.at(1)->index = 1;
	srand(unsigned(time(NULL))); // ex: rand() % 10 + 1 is a number in the range [1, 10]

	lua_pushlightuserdata(L, (void*)this);
	lua_setglobal(L, "master");
	lua_pushlightuserdata(L, (void*)(entities.at(0)));
	lua_setglobal(L, "player");

	frame = 0;
}

void SOBaseScript::render() {
	glClear(GL_COLOR_BUFFER_BIT);

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
	if (frame % 105 == 0) {
		int x = int(bool(rand() % 2));
		int y = int(bool(rand() % 2));
		int two = rand() % 3 + 1;
		if (x == 0) x = -1;
		if (y == 0) y = -1;
		if (two == 2)
			gen_ent("NPC2");
		else
			gen_ent("NPC");
		entities.back()->E.spos(272.0f * float(x), 272.0f * float(y));
		entities.back()->index = entities.size() - 1;
	}

	for (unsigned n = 0; n < entities.size(); n++) {
		entities.at(n)->run(dispatcher);
		if (entities.at(n)->data[0].n == 2)
			killed_entities.push_back(entities.at(n));
	}
	for (unsigned n = 0; n < killed_entities.size(); n++)
		del_ent(killed_entities.at(n));
	killed_entities.clear();
	
	EBuff->update();
	VertBuf->update();
	ElBuf->update();
	render();
	SDL_GL_SwapWindow(res->getWindow());
	//std::cout << "FPS " << get_fps() << std::endl;
	frame++;
}

Entity* SOBaseScript::EBuffAlloc(Packet p, Image imgTex, int prior) {
	return EBuff->allocateEntity(p, imgTex, prior);
}

SOEnt* SOBaseScript::create_ent(EntData ED, Image texture, int priority) {
	if (ED.type == ET_ENT)
		return new SOEnt(ED, texture, this);
	else if (ED.type == ET_PLAYER)
		return new SOPlayer(ED, texture, this);
	else
		return NULL;
}

SOEnt* SOBaseScript::gen_ent(std::string name) {
	entities.push_back(new SOEnt(DB.get(name.c_str()), DB.get(name.c_str()).type == ET_EFFECT ? textures[1] : textures[0], this));
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

float SOBaseScript::get_fps() {
	newt = std::chrono::steady_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(oldt - newt).count();
	oldt = newt;
	return fabs(1000.0f / float(time));
}

Resources SOBaseScript::getResources() {
	return *res;
}

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
	SOEnt* Ent = (((SOBaseScript*)lua_touserdata(L, 1))->entities.at((int)lua_tonumber(L, 2)));
	if (Ent != nullptr)
		lua_pushlightuserdata(L, Ent);
	else
		lua_pushlightuserdata(L, NULL);
	return 1;
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