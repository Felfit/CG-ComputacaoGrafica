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
		m->draw();
	}
	for (auto& g : this->groups) {
		g->draw();
	}
	glPopMatrix();
}

void ScaleStatic::apply() {
	glScalef(x, y, z);
}

void RotateStatic::apply() {
	glRotatef(angle, x, y, z);
}

// the number of seconds to perform a full 360 degrees rotation around the specified axis
void RotateAnim::apply() {
	float mstime = time * 1000;
	float angle = fmod(glutGet(GLUT_ELAPSED_TIME), mstime) / mstime * 360;
	glRotatef(angle, x, y,z);
	glutPostRedisplay(); 
}

void TranslateStatic::apply() {
	glTranslatef(x, y, z);
}

/*
A set of points will be provided to define a Catmull-Rom cubic curve, as well as the number of seconds to run the whole curve.
Due to Catmull-Rom�s curve definition it is always required an initial point before the initial
curve segment and another point after the last segment. The minimum number of points is 4.
*/
void TranslateAnim::apply() {
	renderCatmullCurve(points, 200);
	float pos[4] = { 0 };
	float der[4] = { 0 };
	getGlobalCatmullRomCurvePoint(points, currtime, pos, der);
	glTranslatef(pos[0], pos[1], pos[2]);
	normalize(der);
	normalize(y);
	float z[3];
	cross(der, y, z);
	normalize(z);
	float m[16];
	buildRotMatrix(der, y, z, m);
	glMultMatrixf(m);
	cross(z, der, y);

	int delta = glutGet(GLUT_ELAPSED_TIME) - lastSecond;
	currtime += 1.0 / (1000 * time) * delta;
	if (currtime - floor(currtime) < 0.05) {
		printf("%d\n", glutGet(GLUT_ELAPSED_TIME)/1000);
	}
	lastSecond = glutGet(GLUT_ELAPSED_TIME);
	glutPostRedisplay();
}
