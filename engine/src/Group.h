#ifndef GROUP_H
#define GROUP_H

#include "Model3D.h"
#include <GL/glut.h>
#include <list>
#include <string>
#include <utility>

class Group {
private:
	std::list<Model3D*> models;
	std::list<Group*> groups;

public:
	GLfloat* matrix;
	void draw();
	void addModel(Model3D* m) { models.push_back(m); }
	void addGroup(Group* g) { groups.push_back(g); }
	Group();
	~Group();
};

#endif GROUP_H
