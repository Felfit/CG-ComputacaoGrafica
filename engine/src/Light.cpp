#include "Light.h"

Light::Light() {

}

Light::~Light() {

}

void LightPoint::create() {
	GLfloat dir[4] = { 0.0, 0.0 ,1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, dir);
}

void LightDirectional::create() {
}

void LightSpot::create() {
}