#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glut.h>

class Light {
	public:
		virtual void create() = 0;
		Light();
		~Light();
};


class LightPoint : public Light {
	public:
		float posX;
		float posY;
		float posZ;
		void create();
		
};

class LightDirectional : public Light {
	public:

		void create();

};

class LightSpot : public Light {
	public:
		void create();

};

#endif LIGHT_H
