#include "Entity.h"

Entity::Entity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image img, int prior) {
	bKill = false;
	CONST_ZERO = 0;
	ent_name = "null";
	
	ID = count_existed;
	count_existed++;
	index_forced = 0;
	
#ifdef LUA_C
	data_keys = { "x", "y", "w", "h", "tox", "toy", "tw", "th", "u", "v", "uvw", "uvh", "clock", "priority" };
	data_table = { X, Y, W, H, toX, toY, tW, tH, U, V, UVW, UVH, clock, (float)priority };
#endif

	/*
	0,	1,	2,	3
	4,	5,	6,	7
	8,	9,	10,	11
	12, 13, 14, 15
	*/

	for (unsigned int e = 0; e < indicesperentityv; e++) {
		vVertices.push_back(0.0f);
	}

	unsigned int new_elements[] = {
		0, 1, 2,
		2, 3, 1
	};
	for (unsigned int e = 0; e < indicesperentitye; e++) {
		vElements.push_back(new_elements[e]);
	}

	this->dataNew(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, script_name, img, prior);
}

#ifdef LUA_C & ENTBASE
Entity::Entity(float x, float y, float w, float h, Packet pack, int prior) {
	ent_name = pack.ent_name;
	
	ID = count_existed;
	count_existed++;
	index_forced = 0;

	toX = pack.toX;
	toY = pack.toY;
	tW = pack.tW;
	tH = pack.tH;
	U = pack.U;
	V = pack.V;
	UVW = pack.UVW;
	UVH = pack.UVH;

	data_keys = { "x", "y", "w", "h", "tox", "toy", "tw", "th", "u", "v", "uvw", "uvh", "clock", "priority" };
	data_table = { X, Y, W, H, toX, toY, tW, tH, U, V, UVW, UVH, clock, (float)priority };

	/*
	0,	1,	2,	3
	4,	5,	6,	7
	8,	9,	10,	11
	12, 13, 14, 15
	*/

	for (unsigned int e = 0; e < indicesperentityv; e++) {
		vVertices.push_back(0.0f);
	}

	GLuint new_elements[] = {
		0, 1, 2,
		2, 3, 1
	};
	for (unsigned int e = 0; e < indicesperentitye; e++) {
		vElements.push_back(new_elements[e]);
	}

	this->dataNew(x, y, w, h, pack.toX, pack.toY, pack.tW, pack.tH, pack.U, pack.V, pack.UVW, pack.UVH, pack.script_filename.c_str(), pack.tex, prior);
}
#endif

#ifdef LUA_C & ENTBASE
Entity::Entity(const char* entity_name) {

}
#endif

Entity::Entity() {
	bKill = false;
	CONST_ZERO = 0;
	ent_name = "null";

	ID = count_existed;
	count_existed++;
	index_forced = 0;

#ifdef LUA_C
	data_keys = { "x", "y", "w", "h", "tox", "toy", "tw", "th", "u", "v", "uvw", "uvh", "clock", "priority" };
	data_table = { X, Y, W, H, toX, toY, tW, tH, U, V, UVW, UVH, clock, (float)priority };
#endif

	/*
	0,	1,	2,	3
	4,	5,	6,	7
	8,	9,	10,	11
	12, 13, 14, 15
	*/

	for (unsigned int e = 0; e < indicesperentityv; e++) {
		vVertices.push_back(0.0f);
	}

	unsigned int new_elements[] = {
		0, 1, 2,
		2, 3, 1
	};
	for (unsigned int e = 0; e < indicesperentitye; e++) {
		vElements.push_back(new_elements[e]);
	}

	bFree = 1;
}

Entity::~Entity() {
	count--;
	this->vVertices.clear();
	this->vElements.clear();
}

void Entity::dataNew(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, const char* script_name, Image img, int prior) {
	ent_name = "null";
	X = x; Y = y; W = w; H = h; toX = tox; toY = toy; tW = tw; tH = th; U = u; V = v; UVW = uvw; UVH = uvh;

#ifdef LUA_C
	script_filename = script_name;
#endif

	clock = 0.0f;
	priority = prior;

	count++;
	index_forced = 0;
	tex.img_width = img.img_width;
	tex.img_height = img.img_height;

	/*
	0,	1,	2,	3
	4,	5,	6,	7
	8,	9,	10,	11
	12, 13, 14, 15
	*/

	float data[] = {
		(x + tox),		 (y + toy),		 (u / tex.img_width),					  (v / tex.img_height),
		(x + tox) + tw,	 (y + toy),		 (u / tex.img_width) + (uvw / tex.img_width), (v / tex.img_height),
		(x + tox),		 (y + toy) - th, (u / tex.img_width),					  (v / tex.img_height) + (uvh / tex.img_height),
		(x + tox) + tw,	 (y + toy) - th, (u / tex.img_width) + (uvw / tex.img_width), (v / tex.img_height) + (uvh / tex.img_height)
	};

	for (unsigned int e = 0; e < indicesperentityv; e++) {
		this->vVertices.at(e) = data[e];
	}

#ifdef LUA_C
	load_g_script();
#endif

	this->bChangeSinceLastCheck = 1;
	this->bPriorityChange = 1;
	this->bFree = 0;
}

#ifdef LUA_C
void Entity::dataNew(float x, float y, float w, float h, Packet pack, int prior) {
	ent_name = pack.ent_name;
	X = x; Y = y; W = w; H = h; toX = pack.toX; toY = pack.toY; tW = pack.tW; tH = pack.tH; U = pack.U; V = pack.V; UVW = pack.UVW; UVH = pack.UVH;
	script_filename = pack.script_filename;
	clock = 0.0f;
	priority = prior;

	count++;
	index_forced = 0;
	tex = pack.tex;

	/*
	0,	1,	2,	3
	4,	5,	6,	7
	8,	9,	10,	11
	12, 13, 14, 15
	*/

	float data[] = {
		(x + pack.toX),		 (y + pack.toY),		 (pack.U / tex.img_width),					  (pack.V / tex.img_height),
		(x + pack.toX) + pack.tW,	 (y + pack.toY),		 (pack.U / tex.img_width) + (pack.UVW / tex.img_width), (pack.V / tex.img_height),
		(x + pack.toX),		 (y + pack.toY) - pack.tH, (pack.U / tex.img_width),					  (pack.V / tex.img_height) + (pack.UVH / tex.img_height),
		(x + pack.toX) + pack.tW,	 (y + pack.toY) - pack.tH, (pack.U / tex.img_width) + (pack.UVW / tex.img_width), (pack.V / tex.img_height) + (pack.UVH / tex.img_height)
	};

	for (unsigned int e = 0; e < indicesperentityv; e++) {
		this->vVertices.at(e) = data[e];
	}

	load_g_script();
	this->bChangeSinceLastCheck = 1;
	this->bPriorityChange = 1;
	this->bFree = 0;
}
#endif

#ifdef ENTBASE
void Entity::dataSetPacket(Packet pack) {
	ent_name = pack.ent_name;
	tex = pack.tex;
	script_filename = pack.script_filename;
	toX = pack.toX;
	toY = pack.toY;
	tW = pack.tW;
	tH = pack.tH;
	U = pack.U;
	V = pack.V;
	UVW = pack.UVW;
	UVH = pack.UVH;
	vVertices.at(0) = X + toX;
	vVertices.at(1) = Y + toY;
	vVertices.at(2) = (U / tex.img_width);
	vVertices.at(3) = (V / tex.img_height);
	vVertices.at(4) = (X + toX) + tW;
	vVertices.at(5) = Y + toY;
	vVertices.at(6) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(7) = (V / tex.img_height);
	vVertices.at(8) = X + toX;
	vVertices.at(9) = (Y + toY) - tH;
	vVertices.at(10) = (U / tex.img_width);
	vVertices.at(11) = (V / tex.img_height) + (UVH / tex.img_height);
	vVertices.at(12) = (X + toX) + tW;
	vVertices.at(13) = (Y + toY) - tH;
	vVertices.at(14) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(15) = (V / tex.img_height) + (UVH / tex.img_height);
	this->bChangeSinceLastCheck = 1;
}
#endif

#ifdef LUA_C
void Entity::dataSetScript(const char* filename) {
	script_filename = filename;
}
#endif

void Entity::dataSetPos(float x, float y) {
	X = x;
	Y = y;
	vVertices.at(0) = X + toX;
	vVertices.at(1) = Y + toY;
	vVertices.at(4) = (X + toX) + tW;
	vVertices.at(5) = Y + toY;
	vVertices.at(8) = X + toX;
	vVertices.at(9) = (Y + toY) - tH;
	vVertices.at(12) = (X + toX) + tW;
	vVertices.at(13) = (Y + toY) - tH;
	bChangeSinceLastCheck = 1;
}
void Entity::dataSetDimensions(float w, float h) {
	W = w;
	H = h;
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataSetTexOffset(float tox, float toy) {
	toX = tox;
	toY = toy;
	vVertices.at(0) = X + toX;
	vVertices.at(1) = Y + toY;
	vVertices.at(4) = (X + toX) + tW;
	vVertices.at(5) = Y + toY;
	vVertices.at(8) = X + toX;
	vVertices.at(9) = (Y + toY) - tH;
	vVertices.at(12) = (X + toX) + tW;
	vVertices.at(13) = (Y + toY) - tH;
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataSetTexDimensions(float tw, float th) {
	tW = tw;
	tH = th;
	vVertices.at(4) = (X + toX) + tW;
	vVertices.at(9) = (Y + toY) - tH;
	vVertices.at(12) = (X + toX) + tW;
	vVertices.at(13) = (Y + toY) - tH;
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataSetUV(float u, float v) {
	U = u;
	V = v;
	vVertices.at(2) = (U / tex.img_width);
	vVertices.at(3) = (V / tex.img_height);
	vVertices.at(6) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(7) = (V / tex.img_height);
	vVertices.at(10) = (U / tex.img_width);
	vVertices.at(11) = (V / tex.img_height) + (UVH / tex.img_height);
	vVertices.at(14) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(15) = (V / tex.img_height) + (UVH / tex.img_height);
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataSetUVDimensions(float uvw, float uvh) {
	UVW = uvw;
	UVH = uvh;
	vVertices.at(6) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(11) = (V / tex.img_height) + (UVH / tex.img_height);
	vVertices.at(14) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(15) = (V / tex.img_height) + (UVH / tex.img_height);
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataReset() {
	/*
	(x + tox),
	(y + toy),
	(u / tex_img_w),
	(v / tex_img_h),
	(x + tox) + tw,
	(y + toy),
	(u / tex_img_w) + (uvw / tex_img_w),
	(v / tex_img_h),
	(x + tox),
	(y + toy) - th,
	(u / tex_img_w),
	(v / tex_img_h) + (uvh / tex_img_h),
	(x + tox) + tw,
	(y + toy) - th,
	(u / tex_img_w) + (uvw / tex_img_w),
	(v / tex_img_h) + (uvh / tex_img_h)
	*/
	vVertices.at(0) = (X + toX);
	vVertices.at(1) = (Y + toY);
	vVertices.at(2) = (U / tex.img_width);
	vVertices.at(3) = (V / tex.img_height);
	vVertices.at(4) = (X + toX) + tW;
	vVertices.at(5) = (Y + toY);
	vVertices.at(6) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(7) = (V / tex.img_height);
	vVertices.at(8) = (X + toX);
	vVertices.at(9) = (Y + toY) - tH;
	vVertices.at(10) = (U / tex.img_width);
	vVertices.at(11) = (V / tex.img_height) + (UVH / tex.img_height);
	vVertices.at(12) = (X + toX) + tW;
	vVertices.at(13) = (Y + toY) - tH;
	vVertices.at(14) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(15) = (V / tex.img_height) + (UVH / tex.img_height);
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataFree() {
	for (unsigned int e = 0; e < indicesperentityv; e++) {
		this->vVertices.at(e) = 0.0f;
	}
	this->bChangeSinceLastCheck = 1;
	this->bFree = 1;
}

unsigned int Entity::getID() {
	return ID;
}
vec2 Entity::dataGetPos() {
	return vec2(X, Y);
}
vec2 Entity::dataGetDimensions() {
	return vec2(W, H);
}
vec2 Entity::dataGetTexOffset() {
	return vec2(toX, toY);
}
vec2 Entity::dataGetTexDimensions() {
	return vec2(tW, tH);
}
vec2 Entity::dataGetUV() {
	return vec2(U, V);
}
vec2 Entity::dataGetUVDimensions() {
	return vec2(UVW, UVH);
}
float Entity::getClock() { return this->clock; }
void Entity::incClock() { (this->clock)++; }
void Entity::setClock(float t) { this->clock = t; }
int Entity::getPriority() { return this->priority; }
void Entity::setPriority(int p) {
	this->priority = p;
	bPriorityChange = 1;
}
// FIXME: incomplete
void Entity::Draw(unsigned int vbuff_handle, unsigned int vebuff_handle, bool reset_data) {

	this->bChangeSinceLastCheck = 0;
}

#ifdef LUA_C
int Entity::script() {
	L = luaL_newstate();
	luaL_openlibs(L);
	setup_libraries(L);

	int code = luaL_loadfile(L, (EntBase::script_root + script_filename).c_str());
	if (code == LUA_ERRSYNTAX) {
		std::cout << "Script error: syntax error for script of entity ID " << ID << "." << std::endl;
		return -1;
	}
	else if (code == LUA_ERRMEM) {
		std::cout << "Script error: memory allocation error for script of entity ID " << ID << "." << std::endl;
		return -1;
	}
	else if (code == LUA_ERRFILE) {
		std::cout << "Script error: file opening error for script of entity ID " << ID << "." << std::endl;
		return -1;
	}
	else {
		data_table = { X, Y, W, H, toX, toY, tW, tH, U, V, UVW, UVH, clock, (float)priority };
		stackPushSet_table_floats(L, data_table, data_keys, "this");
		stackPushSet_table_floats(L, g_vals, g_keys, "g");
		stackPushSet_maindata(L);
		if (master != NULL) {
			lua_pushlightuserdata(L, master);
			lua_setglobal(L, "master");
		}
		luaL_dofile(L, (EntBase::script_root + script_filename).c_str());
		data_table = stackGet_table_floats(L, data_keys, "this");
		g_vals = stackGet_table_floats(L, g_keys, "g");
		lua_close(L);
		if (X != data_table.at(0) ||
			Y != data_table.at(1) ||
			W != data_table.at(2) ||
			H != data_table.at(3) ||
			toX != data_table.at(4) ||
			toY != data_table.at(5) ||
			tW != data_table.at(6) ||
			tH != data_table.at(7) ||
			U != data_table.at(8) ||
			V != data_table.at(9) ||
			UVW != data_table.at(10) ||
			UVH != data_table.at(11) ||
			clock != data_table.at(12)) {
			this->bChangeSinceLastCheck = 1;
		}
		if (priority != (int)data_table.at(13)) bPriorityChange = 1;
		X = data_table.at(0);
		Y = data_table.at(1);
		W = data_table.at(2);
		H = data_table.at(3);
		toX = data_table.at(4);
		toY = data_table.at(5);
		tW = data_table.at(6);
		tH = data_table.at(7);
		U = data_table.at(8);
		V = data_table.at(9);
		UVW = data_table.at(10);
		UVH = data_table.at(11);
		clock = data_table.at(12);
		priority = (int)data_table.at(13);
		dataReset();
		return 0;
	}
	lua_close(L);
}

int Entity::load_g_script() {
	L = luaL_newstate();
	int len = 0;
	int code = luaL_loadfile(L, (EntBase::script_root + std::string("g_") + script_filename).c_str());
	if (code == LUA_ERRSYNTAX) {
		std::cout << "Script error: syntax error for script of entity ID " << ID << "." << std::endl;
		return -1;
	}
	else if (code == LUA_ERRMEM) {
		std::cout << "Script error: memory allocation error for script of entity ID " << ID << "." << std::endl;
		return -1;
	}
	else if (code == LUA_ERRFILE) {
		std::cout << "Script error: file opening error for script of entity ID " << ID << "." << std::endl;
		return -1;
	}
	else {
		data_table = { X, Y, W, H, toX, toY, tW, tH, U, V, UVW, UVH, clock, (float)priority };
		stackPushSet_table_floats(L, data_table, data_keys, "this");
		luaL_dofile(L, (EntBase::script_root + std::string("g_") + script_filename).c_str());
		g_keys = lua_parse(getFileSource((EntBase::script_root + std::string("g_") + script_filename).c_str(), &len));
		if (g_keys.size() > 0) {
			if (g_keys.at(0) == "g") g_keys.erase(g_keys.begin());
			g_vals = stackGet_table_floats(L, g_keys, "g");
		}
		return 0;
	}
	lua_close(L);
}


int Entity::routine() {
	script();
	return 0;
}
#endif

unsigned int Entity::count = 0;
unsigned int Entity::count_existed = 0;
const unsigned int Entity::indicesperentityv = 16;
const unsigned int Entity::indicesperentitye = 6;

//__________________________________________________________________________________________________
//EntPointerID
//__________________________________________________________________________________________________

EntPointer::EntPointer(Entity* entity) : ent(entity) {}
EntPointer::EntPointer() : ent(NULL) {}

void EntPointer::operator=(Entity* entity) { ent = entity; }
void EntPointer::operator=(EntPointer entptr) { ent = entptr.gent(); }

//FIXME: add conditional for NULL pointer
Entity* EntPointer::gent() { return ent; }
unsigned int EntPointer::gid() { return ent->ID; }
vec2 EntPointer::gpos() { return vec2(ent->dataGetPos().x, ent->dataGetPos().y); }
void EntPointer::spos(float x, float y) { if (ent != NULL) ent->dataSetPos(x, y); }
vec2 EntPointer::gdim() { return vec2(ent->dataGetDimensions().x, ent->dataGetDimensions().y); }
void EntPointer::sdim(float w, float h) { if (ent != NULL) ent->dataSetDimensions(w, h); }
vec2 EntPointer::gtoff() { return vec2(ent->dataGetTexOffset().x, ent->dataGetTexOffset().y); }
void EntPointer::stoff(float tox, float toy) { if (ent != NULL) ent->dataSetTexOffset(tox, toy); }
vec2 EntPointer::gtdim() { return vec2(ent->dataGetTexDimensions().x, ent->dataGetTexDimensions().y); }
void EntPointer::stdim(float tw, float th) { if (ent != NULL) ent->dataSetTexDimensions(tw, th); }
vec2 EntPointer::guvpos() { return vec2(ent->dataGetUV().x, ent->dataGetUV().y); }
void EntPointer::suvpos(float u, float v) { if (ent != NULL) ent->dataSetUV(u, v); }
vec2 EntPointer::guvdim() { return vec2(ent->dataGetUVDimensions().x, ent->dataGetUVDimensions().y); }
void EntPointer::suvdim(float uvw, float uvh) { if (ent != NULL) ent->dataSetUVDimensions(uvw, uvh); }
float EntPointer::gclock() { return ent->getClock(); }
void EntPointer::sclock(float t) { if (ent != NULL) ent->setClock(t); }
int EntPointer::gprior() { return ent->getPriority(); }
void EntPointer::sprior(int p) { if (ent != NULL) ent->setPriority(p); }