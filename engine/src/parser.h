using namespace std;
using namespace tinyxml2;

typedef struct scene {
	list<Model3D> modelos;

}*Scene;

int sceneParser(char* filename);