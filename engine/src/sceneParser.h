#include "Model3D.h"
#include<list>


struct scene {
	std::list<Model3D> modelos;
};

typedef struct scene *Scene;




int sceneParser(char* filename, Scene s);