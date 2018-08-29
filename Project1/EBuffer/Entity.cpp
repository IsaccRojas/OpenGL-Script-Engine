#include "Entity.h"

Packet::Packet() {
	x = 0; y = 0; w = 0; h = 0; tox = 0; toy = 0; tw = 0; th = 0; u = 0; v = 0; uvw = 0; uvh = 0;
}
Packet::Packet(float X, float Y, float W, float H, float toX, float toY, float tW, float tH, float U, float V, float uvW, float uvH) : x(X), y(Y), w(W), h(H), tox(toX), toy(toY), tw(tW), th(tH), u(U), v(V), uvw(uvW), uvh(uvH) {}
Packet Packet::operator+(Packet p) {
	return Packet{this->x + p.x, this->y + p.y, this->w + p.w, this->h + p.h, this->tox + p.tox, this->toy + p.toy, this->tw + p.tw, this->th + p.th, this->u + p.u, this->v + p.v, this->uvw + p.uvw, this->uvh + p.uvh};
}

Entity::Entity(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image img, int prior) {
	ID = count_existed;
	count_existed++;

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

	this->dataNew(x, y, w, h, tox, toy, tw, th, u, v, uvw, uvh, img, prior);
}
Entity::Entity(Packet p, Image img, int prior) {
	ID = count_existed;
	count_existed++;

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

	this->dataNew(p, img, prior);
}

Entity::Entity() {
	ent_name = "null";
	ID = count_existed;
	count_existed++;

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
	this->vVertices.clear();
	this->vElements.clear();
}

void Entity::dataNew(float x, float y, float w, float h, float tox, float toy, float tw, float th, float u, float v, float uvw, float uvh, Image img, int prior) {
	X = x; Y = y; W = w; H = h; toX = tox; toY = toy; tW = tw; tH = th; U = u; V = v; UVW = uvw; UVH = uvh; T = float(img.texture_unit);
	priority = prior;
	tex.img_width = img.img_width;
	tex.img_height = img.img_height;

	float data[] = {
		(x + tox),		 (y + toy),		 (u / tex.img_width),							(v / tex.img_height),							T,		
		(x + tox) + tw,	 (y + toy),		 (u / tex.img_width) + (uvw / tex.img_width),	(v / tex.img_height),							T,
		(x + tox),		 (y + toy) - th, (u / tex.img_width),							(v / tex.img_height) + (uvh / tex.img_height),	T,
		(x + tox) + tw,	 (y + toy) - th, (u / tex.img_width) + (uvw / tex.img_width),	(v / tex.img_height) + (uvh / tex.img_height),	T
	};
	for (unsigned int e = 0; e < indicesperentityv; e++) {
		this->vVertices.at(e) = data[e];
	}

	this->bChangeSinceLastCheck = 1;
	this->bPriorityChange = 1;
	this->bFree = 0;
}
void Entity::dataNew(Packet p, Image img, int prior) {
	X = p.x; Y = p.y; W = p.w; H = p.h; toX = p.tox; toY = p.toy; tW = p.tw; tH = p.th; U = p.u; V = p.v; UVW = p.uvw; UVH = p.uvh; T = float(img.texture_unit);
	priority = prior;
	tex.img_width = img.img_width;
	tex.img_height = img.img_height;

	float data[] = {
		(p.x + p.tox),			(p.y + p.toy),			(p.u / tex.img_width),								(p.v / tex.img_height),								T,
		(p.x + p.tox) + p.tw,	(p.y + p.toy),			(p.u / tex.img_width) + (p.uvw / tex.img_width),	(p.v / tex.img_height),								T,
		(p.x + p.tox),			(p.y + p.toy) - p.th,	(p.u / tex.img_width),								(p.v / tex.img_height) + (p.uvh / tex.img_height),	T,
		(p.x + p.tox) + p.tw,	(p.y + p.toy) - p.th,	(p.u / tex.img_width) + (p.uvw / tex.img_width),	(p.v / tex.img_height) + (p.uvh / tex.img_height),	T
	};
	for (unsigned int e = 0; e < indicesperentityv; e++) {
		this->vVertices.at(e) = data[e];
	}

	this->bChangeSinceLastCheck = 1;
	this->bPriorityChange = 1;
	this->bFree = 0;
}

/*
setpos - 0, 1, 5, 6, 10, 11, 15, 16
settoff - 0, 1, 5, 6, 10, 11, 15, 16
settdim - 5, 11, 15, 16
setuv - 2, 3, 7, 8, 12, 13, 17, 18
setuvdim - 7, 13, 17, 18
*/

void Entity::dataSetPos(float x, float y) {
	X = x;
	Y = y;
	vVertices.at(0) = X + toX;
	vVertices.at(1) = Y + toY;
	vVertices.at(5) = (X + toX) + tW;
	vVertices.at(6) = Y + toY;
	vVertices.at(10) = X + toX;
	vVertices.at(11) = (Y + toY) - tH;
	vVertices.at(15) = (X + toX) + tW;
	vVertices.at(16) = (Y + toY) - tH;
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
	vVertices.at(5) = (X + toX) + tW;
	vVertices.at(6) = Y + toY;
	vVertices.at(10) = X + toX;
	vVertices.at(11) = (Y + toY) - tH;
	vVertices.at(15) = (X + toX) + tW;
	vVertices.at(16) = (Y + toY) - tH;
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataSetTexDimensions(float tw, float th) {
	tW = tw;
	tH = th;
	vVertices.at(5) = (X + toX) + tW;
	vVertices.at(11) = (Y + toY) - tH;
	vVertices.at(15) = (X + toX) + tW;
	vVertices.at(16) = (Y + toY) - tH;
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataSetUV(float u, float v) {
	U = u;
	V = v;
	vVertices.at(2) = (U / tex.img_width);
	vVertices.at(3) = (V / tex.img_height);
	vVertices.at(7) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(8) = (V / tex.img_height);
	vVertices.at(12) = (U / tex.img_width);
	vVertices.at(13) = (V / tex.img_height) + (UVH / tex.img_height);
	vVertices.at(17) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(18) = (V / tex.img_height) + (UVH / tex.img_height);
	this->bChangeSinceLastCheck = 1;
}
void Entity::dataSetUVDimensions(float uvw, float uvh) {
	UVW = uvw;
	UVH = uvh;
	vVertices.at(7) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(13) = (V / tex.img_height) + (UVH / tex.img_height);
	vVertices.at(17) = (U / tex.img_width) + (UVW / tex.img_width);
	vVertices.at(18) = (V / tex.img_height) + (UVH / tex.img_height);
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
Packet Entity::dataPacket() {
	return Packet{ X, Y, W, H, toX, toY, tW, tH, U, V, UVW, UVH };
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
int Entity::dataGetTexUnit() {
	return int(T);
}
int Entity::getPriority() { return this->priority; }
void Entity::setPriority(int p) {
	this->priority = p;
	bPriorityChange = 1;
}

unsigned int Entity::count_existed = 0;
const unsigned int Entity::indicesperentityv = 20;
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
Packet EntPointer::gpack() { return ent->dataPacket(); }
int EntPointer::gtu() { return ent->dataGetTexUnit(); }
int EntPointer::gprior() { return ent->getPriority(); }
void EntPointer::sprior(int p) { if (ent != NULL) ent->setPriority(p); }