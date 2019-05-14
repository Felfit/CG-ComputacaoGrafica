#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <unordered_map>
#include "Model3D.h"
#include "Group.h"
#include "Light.h"
#include "tinyxml2.h"

extern bool drawCurve; // determina se as curvas de catmull rom sao desenhadas

class Scene {
private:
	int modelsN;
	std::unordered_map<std::string, ModelBuffers*> modelsBuff;
	std::unordered_map<std::string, Texture*> textures;
	std::list<Light*> lights;
	std::list<Group*> groups;
	Model3D skybox;
	bool hasSkybox = false;

	void parseSkybox(tinyxml2::XMLElement* el);
	void parseLight(tinyxml2::XMLElement* el);
	void parseGroup(tinyxml2::XMLElement* parent, Group* parentGr);
	void parseModel(tinyxml2::XMLElement* el, Group* group);
	

public:
	Scene();
	~Scene();

	int parse(char* filename);
	void draw();
	void followModel(int cameraFollow, Point3D* center);
	void drawColor();
	void drawSkybox(float camx, float camy, float camz);
	int getModelsN() { return modelsN; };
};

#endif SCENE_H