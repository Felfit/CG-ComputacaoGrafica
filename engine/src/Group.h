#ifndef GROUP_H
#define GROUP_H

#include "curves.h"
#include "Model3D.h"
#include <GL/glut.h>
#include <list>
#include <string>
#include <utility>
#include <vector> 



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
		int lastSecond = 0;
		float currtime = 0;
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



class Group {
	private:
		std::list<Model3D*> models;
		std::list<Group*> groups;
		Transform* transforms[3];
		int tranformsSize = 0;
		
	public:
		void draw();
		void addTransform(Transform* t);
		void addModel(Model3D* m) { models.push_back(m); }
		void addGroup(Group* g) { groups.push_back(g); }
		Group();
		~Group();
};

#endif GROUP_H
