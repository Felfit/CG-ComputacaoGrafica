#ifndef GROUP_H
#define GROUP_H

#include "Model3D.h"
#include <GL/glut.h>
#include <list>
#include <string>
#include <utility>
#include <vector> 

struct TranslateStatic {
	float x;
	float y;
	float z;
};

struct TranslateAnim {
	float time;
	vector<Point3D> points;
};

struct RotateStatic {
	float x;
	float y;
	float z;
	float angle;
};

struct RotateAnim {
	float x;
	float y;
	float z;
	float time;
};

struct Scale {
	float x;
	float y;
	float z;
};




class Group {
	private:
		std::list<Model3D*> models;
		std::list<Group*> groups;
		char transforms[3];
		int tranformsSize = 0;
		TranslateStatic ts;
		TranslateAnim ta;
		bool hasTranslateStatic = false;
		bool hasTranslateAnim = false;
		RotateStatic rs;
		RotateAnim ra;
		bool hasRotateStatic = false;
		bool hasRotateAnim = false;
		Scale s; 
		bool hasScale = false;

	public:
		void draw();
		void addModel(Model3D* m) { models.push_back(m); }
		void addGroup(Group* g) { groups.push_back(g); }
		void addRotate(RotateStatic r);
		void addTranslate(TranslateStatic t);
		void addScale(Scale s);
		void addTranslateAnim(TranslateAnim anim);
		void addRotateAnim(RotateAnim r);
		Group();
		~Group();
};

#endif GROUP_H
