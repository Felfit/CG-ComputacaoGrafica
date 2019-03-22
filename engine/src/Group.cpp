#include "Group.h"

using namespace std;


Group::Group() {
}

Group::~Group() {

}


void Group::addRotate(float angle, float x, float y, float z) {
	if (hasRotate)
		return;
	r.x = x;
	r.y = y;
	r.z = z;
	r.angle = angle;
	transforms[tranformsSize++] = 'r';
}
void Group::addTranslate(float x, float y, float z) {
	if (hasTranslate)
		return;
	t.x = x;
	t.y = y;
	t.z = z;
	transforms[tranformsSize++] = 't';
}
void Group::addScale(float x, float y, float z){
	if (hasScale)
		return;
	s.x = x;
	s.y = y;
	s.z = z;
	transforms[tranformsSize++] = 's';
}

void Group::draw() {
	glPushMatrix();
	for (int i = 0; i < tranformsSize; i++)
	{
		switch (transforms[i])
		{
		case 'r':
			glRotatef(r.angle,r.x,r.y,r.z);
			break;
		case 't':
			glTranslatef(t.x, t.y, t.z);
			break;
		case 's':
			glScalef(s.x, s.y, s.z);
			break;
		default:
			break;
		}
	}
	for (auto& m : this->models) {
		m->draw();
	}
	for (auto& g : this->groups) {
		g->draw();
	}
	glPopMatrix();
}