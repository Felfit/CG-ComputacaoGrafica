#include "Light.h"

Light::Light() {

}

Light::~Light() {

}

// TODO: acabar

void LightDirectional::create() {
	float lpos[4] = { dirX, dirY, dirZ, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lpos);
}

void LightPoint::create() {
	float lpos[4] = { posX, posY, posZ, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	float attenuation = 1;
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, attenuation);

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lpos);
}



void LightSpot::create() {
	float lpos[4] = { posX, posY, posZ, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	float spotDir[3] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir); 

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0); // [0,90] or 180

	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, 0.0); // [0,128]

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lpos);
}