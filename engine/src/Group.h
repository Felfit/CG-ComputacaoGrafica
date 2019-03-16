#ifndef GROUP_H
#define GROUP_H

#include "Model3D.h"

#include <list>
#include <string>


class Group {
private:
	// tXYZ -> translate, sXYZ -> scale, rXYZang -> rotate
	float tX, tY, tZ, sX, sY, sZ, rX, rY, rZ, rAng;
	std::list<std::string> models;
	std::list<Group> groups;

public:
	void addModel(std::string m) { models.push_back(m); }
	void addGroup(Group g) { groups.push_back(g); }
	void setTranslate(float x, float y, float z) { tX = x; tY = y; tZ = z; }
	void setScale(float x, float y, float z) { sX = x; sY = y; sZ = z; }
	void setRotate(float angle, float x, float y, float z) { rX = x; rY = y; rZ = z; rAng = angle; }
	Group();
	~Group();
};

#endif GROUP_H
