#include "Group.h"

using namespace std;


Group::Group() {
}

Group::~Group() {

}


void Group::addRotate(Rotate nr) {
	if (hasRotate)
		return;
	r = nr;
	transforms[tranformsSize++] = 'r';
}
void Group::addTranslate(Translate nt) {
	if (hasTranslate)
		return;
	t = nt;
	transforms[tranformsSize++] = 't';
}
void Group::addScale(Scale ns){
	if (hasScale)
		return;
	s = ns;
	transforms[tranformsSize++] = 's';
}

// TODO: anims

/* 
A set of points will be provided to define a Catmull-Rom cubic curve, as well as the number of seconds to run the whole curve.
Due to Catmull-Rom’s curve definition it is always required an initial point before the initial
curve segment and another point after the last segment. The minimum number of points is 4.
*/
void Group::addTranslateAnim(float time, vector<Point3D> points) {
	// TODO: exception tamanho minimo 4

}

// the number of seconds to perform a full 360 degrees rotation around the specified axis
void Group::addRotateAnim(float time, float x, float y, float z) {

}

// usar glutGet(GLUT_ELAPSED_TIME) para o tempo nas anims
void Group::draw() {
	glPushMatrix();
	for (int i = 0; i < tranformsSize; i++) {
		switch (transforms[i]) {
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