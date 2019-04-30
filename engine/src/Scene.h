#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <unordered_map>
#include "Model3D.h"
#include "Group.h"
#include "Light.h"
#include "tinyxml2.h"

class Scene {
private:
	std::unordered_map<std::string, ModelBuffers*> models;
	std::unordered_map<std::string, Texture*> textures;
	std::list<Light*> lights;
	std::list<Group*> groups;
	void parseGroup(tinyxml2::XMLElement* parent, Group* parentGr);
	void parseLight(tinyxml2::XMLElement* el);
	void parseModel(tinyxml2::XMLElement* el, Group* group);

public:
	Scene();
	~Scene();
	int parse(char* filename);
	void draw();
};

#endif SCENE_H