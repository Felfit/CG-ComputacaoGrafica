#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glut.h>

class Light {
	public:
		virtual void create() = 0;
		Light();
		~Light();
};

class LightDirectional : public Light {
	public:
		float dirX;
		float dirY;
		float dirZ;
		void create();
};

class LightPoint : public Light {
	public:
		float posX;
		float posY;
		float posZ;
		void create();
};



class LightSpot : public Light {
	public:
		float posX;
		float posY;
		float posZ;
		void create();
};

#endif LIGHT_H
