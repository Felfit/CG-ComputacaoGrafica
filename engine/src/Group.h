#ifndef GROUP_H
#define GROUP_H

#include <list>
#include <string>

#include "Model3D.h"
#include "Transform.h"


class Group {
	private:
		std::list<Model3D> models;
		std::list<Group*> groups;
		Transform* transforms[3];
		int tranformsSize = 0;

	public:
		void draw();
		void addTransform(Transform* t);
		void addModel(Model3D m) { models.push_back(m); }
		void addGroup(Group* g) { groups.push_back(g); }
		Group();
		~Group();
};



#endif GROUP_H
