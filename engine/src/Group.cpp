#include "Group.h"

using namespace std;


Group::Group() {
}

Group::~Group() {

}


void Group::addTransform(Transform* t) {
	for (int i = 0; i < tranformsSize; i++) {
		if (transforms[i]->getType() == t->getType()) return; // TODO: exception
	}
	transforms[tranformsSize++] = t;
}

void Group::draw() {
	glPushMatrix();
	for (int i = 0; i < tranformsSize; i++) {
		transforms[i]->apply();
	}
	for (auto& m : this->models) {
		m.draw();
	}
	for (auto& g : this->groups) {
		g->draw();
	}
	glPopMatrix();
}

