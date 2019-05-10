#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <unordered_map>
#include "Model3D.h"
#include "Group.h"
#include "Light.h"
#include "tinyxml2.h"

extern bool drawCurve;

class Scene {
private:
	std::unordered_map<std::string, ModelBuffers*> models;
	std::unordered_map<std::string, Texture*> textures;
	std::list<Light*> lights;
	std::list<Group*> groups;
	void parseGroup(tinyxml2::XMLElement* parent, Group* parentGr);
	void parseLight(tinyxml2::XMLElement* el);
	void parseModel(tinyxml2::XMLElement* el, Group* group);
	bool hasSkybox = false;
	Model3D skybox;

public:
	Scene();
	~Scene();
	int parse(char* filename);
	void draw();
	void followModel();
	void drawColor();
	void drawSkybox(int camx, int camy, int camz);
};

#endif SCENE_H