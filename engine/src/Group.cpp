#include "Group.h"

using namespace std;


Group::Group() {
}

Group::~Group() {

}


void Group::addRotate(RotateStatic nr) {
	if (hasRotateStatic || hasRotateAnim)
		return;
	hasRotateStatic = true;
	rs = nr;
	transforms[tranformsSize++] = 'r';
}
void Group::addTranslate(TranslateStatic nt) {
	if (hasTranslateAnim || hasTranslateStatic)
		return;
	hasTranslateStatic = true;
	ts = nt;
	transforms[tranformsSize++] = 't';
}
void Group::addScale(Scale ns){
	if (hasScale)
		return;
	hasScale = true;
	s = ns;
	transforms[tranformsSize++] = 's';
}

// TODO: anims

/* 
A set of points will be provided to define a Catmull-Rom cubic curve, as well as the number of seconds to run the whole curve.
Due to Catmull-Rom�s curve definition it is always required an initial point before the initial
curve segment and another point after the last segment. The minimum number of points is 4.
*/
void Group::addTranslateAnim(TranslateAnim nt) {
	// TODO: exception tamanho minimo 4
	if (hasTranslateStatic || hasTranslateAnim)
		return;
	hasTranslateAnim = true;
	ta = nt;
	transforms[tranformsSize++] = 't';
}

// the number of seconds to perform a full 360 degrees rotation around the specified axis
void Group::addRotateAnim(RotateAnim nr) {
	if (hasRotateStatic || hasRotateAnim)
		return;
	hasRotateAnim = true;
	ra = nr;
	transforms[tranformsSize++] = 'r';
}

// usar glutGet(GLUT_ELAPSED_TIME) para o tempo nas anims
void Group::draw() {
	glPushMatrix();
	for (int i = 0; i < tranformsSize; i++) {
		switch (transforms[i]) {
		case 'r':
			if (hasRotateStatic) {
				glRotatef(rs.angle, rs.x, rs.y, rs.z);
			}
			else if (hasRotateAnim) {
				// TODO: coisas
			}
			break;
		case 't':
			if (hasTranslateStatic) {
				glTranslatef(ts.x, ts.y, ts.z);
			}
			else if(hasTranslateAnim){
				applyTranslateAnim();
			}
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

void Group::applyTranslateAnim()
{
	float pos[4] = { 0 };
	float der[4] = { 0 };
	//TODO mudar curve point
	getGlobalCatmullRomCurvePoint(ta.points, ta.currtime, pos, der);
	float velocity = length(der);
	glTranslatef(pos[0], pos[1], pos[2]);
	float z[3];
	normalize(der);
	normalize(ta.y);
	cross(der, ta.y, z);
	normalize(z);
	float m[16];
	buildRotMatrix(der, ta.y, z, m);
	glMultMatrixf(m);
	cross(z, der, ta.y);
	//TODO update this
	ta.currtime += 2 / velocity;
}
