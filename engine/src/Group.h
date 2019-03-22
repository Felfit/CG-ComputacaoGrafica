#ifndef GROUP_H
#define GROUP_H

#include "Model3D.h"
#include <GL/glut.h>
#include <list>
#include <string>
#include <utility>

typedef struct translate {
	float x;
	float y;
	float z;
} Translate;

typedef struct scale {
	float x;
	float y;
	float z;
} Scale;

typedef struct rotate {
	float x;
	float y;
	float z;
	float angle;
} Rotate;

class Group {
private:
	std::list<Model3D*> models;
	std::list<Group*> groups;
	char transforms[3];
	int tranformsSize = 0;
	Translate t; 
	bool hasTranslate = false;
	Rotate r; 
	bool hasRotate = false;
	Scale s; 
	bool hasScale = false;

public:
	GLfloat* matrix;
	void draw();
	void addModel(Model3D* m) { models.push_back(m); }
	void addGroup(Group* g) { groups.push_back(g); }
	void addRotate(float angle,float x,float y,float z);
	void addTranslate(float x,float y,float z);
	void addScale(float x,float y,float z);
	Group();
	~Group();
};

#endif GROUP_H
