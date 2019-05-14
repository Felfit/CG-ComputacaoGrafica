#include "Light.h"

int Light::lightCount = 0;

Light::Light() {
	lightn = lightCount++;
}

void LightDirectional::create() {
	GLenum light = GL_LIGHT0 + lightn;
	glEnable(light);

	float ldir[4] = { dirX, dirY, dirZ, 0 };
	glLightfv(light, GL_POSITION, ldir);

	float amb[4] = { 1, 1, 1, 1 };
	glLightfv(light, GL_AMBIENT, amb);
	//glLightfv(light0, GL_DIFFUSE, diff); default -> 0, 0, 0, 0
}

void LightPoint::create() {
	GLenum light = GL_LIGHT0 + lightn;
	glEnable(light);

	float lpos[4] = { posX, posY, posZ, 1 };
	glLightfv(light, GL_POSITION, lpos);

	float attenuation = 1;
	glLightf(light, GL_CONSTANT_ATTENUATION, attenuation);

	float amb[4] = { 1, 1, 1, 1 };
	glLightfv(light, GL_AMBIENT, amb);
	//glLightfv(light0, GL_DIFFUSE, diff); default -> 0, 0, 0, 0
}

void LightSpot::create() {
	GLenum light = GL_LIGHT0 + lightn;
	glEnable(light);

	float lpos[4] = { posX, posY, posZ, 1 };
	glLightfv(light, GL_POSITION, lpos);

	float spotDir[3] = { dirX, dirY, dirZ };
	glLightfv(light, GL_SPOT_DIRECTION, spotDir); 

	glLightf(light, GL_SPOT_CUTOFF, cutoff); 

	glLightf(light, GL_SPOT_EXPONENT, exponent); 

	float attenuation = 1;
	glLightf(light, GL_CONSTANT_ATTENUATION, attenuation);

	float amb[4] = { 1, 1, 1, 1 };
	glLightfv(light, GL_AMBIENT, amb);
	//glLightfv(light0, GL_DIFFUSE, diff); default -> 0, 0, 0, 0
}