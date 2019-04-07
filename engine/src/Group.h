#ifndef GROUP_H
#define GROUP_H

#include "Model3D.h"
#include <GL/glut.h>
#include <list>
#include <string>
#include <utility>
#include <vector> 

struct Translate {
	float x;
	float y;
	float z;
};

struct Scale {
	float x;
	float y;
	float z;
};

struct Rotate {
	float x;
	float y;
	float z;
	float angle;
};

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
		void draw();
		void addModel(Model3D* m) { models.push_back(m); }
		void addGroup(Group* g) { groups.push_back(g); }
		void addRotate(Rotate r);
		void addTranslate(Translate t);
		void addScale(Scale s);
		void addTranslateAnim(float time, std::vector<Point3D> points);
		void addRotateAnim(float time, float x, float y, float z);
		Group();
		~Group();
};

#endif GROUP_H
