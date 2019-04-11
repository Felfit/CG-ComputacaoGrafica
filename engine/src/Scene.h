#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <unordered_map>
#include "Model3D.h"
#include "Group.h"
#include "tinyxml2.h"

class Scene {
private:
	std::unordered_map<std::string, Model3D*> models;
	std::list<Group*> groups;
	void parseGroup(tinyxml2::XMLElement* parent, Group* parentGr);

public:
	Scene();
	~Scene();
	int parse(char *filename);
	void draw();
};

#endif SCENE_H