#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector> 


#include "curves.h"
#include "Model3D.h"

#include <GL/glut.h>

class Transform {
public:
	virtual void apply() = 0;
	virtual int getType() = 0;
};

class TranslateAnim : public Transform {
public:
	int getType() { return 0; }
	void apply();
	float time = 0;
	std::vector<Point3D> points;

private:
	float y[3] = { 0,1,0 };
};


class TranslateStatic : public Transform {
public:
	int getType() { return 0; }
	void apply();
	float x;
	float y;
	float z;
};

class RotateAnim : public Transform {
public:
	int getType() { return 1; }
	void apply();
	float x;
	float y;
	float z;
	float time;
};


class RotateStatic : public Transform {
public:
	int getType() { return 1; }
	void apply();
	float x;
	float y;
	float z;
	float angle;
};

class ScaleStatic : public Transform {
public:
	int getType() { return 2; }
	void apply();
	float x;
	float y;
	float z;
};

#endif TRANSFORM_H