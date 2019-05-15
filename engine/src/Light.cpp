#include "Light.h"

// luz ambiente branca
float amb[4] = { 1, 1, 1, 1 };

int Light::lightCount = 0;

Light::Light() {
	lightn = lightCount++;
}

void LightDirectional::create() {
	GLenum light = GL_LIGHT0 + lightn;
	glEnable(light);

	float ldir[4] = { dirX, dirY, dirZ, 0 };
	glLightfv(light, GL_POSITION, ldir);

	glLightfv(light, GL_AMBIENT, amb);
	//glLightfv(light0, GL_DIFFUSE, diff); default -> 0, 0, 0, 0
}

void LightPoint::create() {
	GLenum light = GL_LIGHT0 + lightn;
	glEnable(light);

	float lpos[4] = { posX, posY, posZ, 1 };
	glLightfv(light, GL_POSITION, lpos);

	glLightf(light, GL_CONSTANT_ATTENUATION, conAtt);
	glLightf(light, GL_LINEAR_ATTENUATION, linAtt);
	glLightf(light, GL_QUADRATIC_ATTENUATION, quaAtt);

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

	glLightf(light, GL_CONSTANT_ATTENUATION, conAtt);
	glLightf(light, GL_LINEAR_ATTENUATION, linAtt);
	glLightf(light, GL_QUADRATIC_ATTENUATION, quaAtt);

	glLightfv(light, GL_AMBIENT, amb);
	//glLightfv(light0, GL_DIFFUSE, diff); default -> 0, 0, 0, 0
}