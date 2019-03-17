#include "Group.h"

using namespace std;



Group::Group() {
}

Group::~Group() {

}

void Group::draw() {
	glPushMatrix();
	glRotatef(rAng, rX, rY, rZ);
	glTranslatef(tX, tY, tZ);
	glScalef(sX, sY, sZ);
	for (auto& m : models) {
		m->draw();
	}
	for (auto& g : groups) {
		g->draw();
	}
	glPopMatrix();
}