#include "Group.h"

using namespace std;


Group::Group() {
}

Group::~Group() {

}




void Group::draw() {
	glPushMatrix();
	glMultMatrixf(this->matrix);
	for (auto& m : models) {
		m->draw();
	}
	for (auto& g : groups) {
		g->draw();
	}
	glPopMatrix();
}