#include "tinyxml2.h"
#include <unordered_map>
#include "Model3D.h"
#include "parser.h"

using namespace std;
using namespace tinyxml2;


void loadmodel(const char*s) {
	printf("model: %s\n",s);
}

int sceneParser(char* filename) {
	Scene s = new struct scene;
	XMLDocument doc;
	XMLError eresult = doc.LoadFile(filename);
	if (eresult != XML_SUCCESS)
	{
		printf("Erro a abrir o fichero");
		return -1;
	}
	XMLNode* n = doc.FirstChild();
	if (!n) {
		printf("Ficheiro não tem nodos");
		return -2;
	}
	
	XMLElement* el = n->ToElement();
	if (strcmp("scene", el->Name())) {
		printf("Wrong file Format");
	}
	XMLElement* child = el->FirstChildElement();
	while (child)
	{
		const char * f = child->Attribute("file");
		Model3D m = Model3D(f);
		s->modelos.push_front(m);
		child = child->NextSiblingElement();
	}
}
