#include "Light.h"

Light::Light() {

}

Light::~Light() {

}

// TODO: acabar

void LightPoint::create() {
	float lpos[4] = { posX, posY, posZ, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
}

void LightDirectional::create() {
}

void LightSpot::create() {
}