#ifndef GROUP_H
#define GROUP_H

#include "Model3D.h"
#include <GL/glut.h>
#include <list>
#include <string>


class Group {
private:
	// tXYZ -> translate, sXYZ -> scale, rXYZang -> rotate
	float tX, tY, tZ;
	float sX = 1, sY = 1, sZ = 1;
	float rX, rY, rZ, rAng;
	std::list<Model3D*> models;
	std::list<Group*> groups;

public:
	void draw();
	void addModel(Model3D* m) { models.push_back(m); }
	void addGroup(Group* g) { groups.push_back(g); }
	void setTranslate(float x, float y, float z) { tX = x; tY = y; tZ = z; }
	float getTX() { return tX; }
	void setScale(float x, float y, float z) { sX = x; sY = y; sZ = z; }
	void setRotate(float angle, float x, float y, float z) { rX = x; rY = y; rZ = z; rAng = angle; }
	Group();
	~Group();
};

#endif GROUP_H
