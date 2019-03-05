#include "Model3D.h"
#include<list>
#include<unordered_map>

struct scene {
	std::unordered_map<std::string, Model3D*> modelos;
	std::list<std::string> objetos;
};

typedef struct scene *Scene;




int sceneParser(char* filename, Scene s);