#include "Light.h"

void LightDirectional::create() {
	float ldir[4] = { dirX, dirY, dirZ, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, ldir);

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, cor);
}

void LightPoint::create() {
	float lpos[4] = { posX, posY, posZ, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	float attenuation = 1;
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, attenuation);

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, cor);
}

void LightSpot::create() {
	float lpos[4] = { posX, posY, posZ, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	float spotDir[3] = { dirX, dirY, dirZ };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir); 

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff); 

	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, exponent); 

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, cor);
}