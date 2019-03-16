#include "Model3D.h"
#include "Group.h"
#include <list>
#include <unordered_map>

struct scene {
	std::unordered_map<std::string, Model3D*> models;
	Group* root;
};

typedef struct scene *Scene;




int sceneParser(char* filename, Scene s);