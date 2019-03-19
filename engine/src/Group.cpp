#include "Group.h"

using namespace std;


Group::Group() {
}

Group::~Group() {

}




void Group::draw() {
	glPushMatrix();
	glMultMatrixf(this->matrix);
	for (auto& m : this->models) {
		m->draw();
	}
	for (auto& g : this->groups) {
		g->draw();
	}
	glPopMatrix();
}