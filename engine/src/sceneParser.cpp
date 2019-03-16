#include "tinyxml2.h"
#include <unordered_map>
#include "Model3D.h"
#include "sceneParser.h"

using namespace tinyxml2;

void parseGroup(XMLElement* parent, Group parentGr, Scene s) {
	Group childGr;
	XMLElement* child = parent->FirstChildElement();
	// transformations -> models -> groups
	while (child) {
		if (!strcmp("group", child->Name())) {
			parseGroup(child, childGr, s);
		}
		else if (!strcmp("models", child->Name())) {
			XMLElement* model = child->FirstChildElement();
			while (model) {
				const char *filename = child->Attribute("file");
				// adicionar ao childGr
				if (!s->models[filename]) {
					Model3D* m = new Model3D(filename);
					s->models[filename] = m; //insert
				}
				model = model->NextSiblingElement();
			}
		}
		else if (!strcmp("translate", child->Name())) {
			float x = (float) atof(child->Attribute("X"));
			float y = (float) atof(child->Attribute("Y"));
			float z = (float) atof(child->Attribute("Z"));
			childGr.setTranslate(x, y, z);
		}
		else if (!strcmp("scale", child->Name())) {
			float x = (float) atof(child->Attribute("X"));
			float y = (float) atof(child->Attribute("Y"));
			float z = (float) atof(child->Attribute("Z"));
			childGr.setScale(x, y, z);
		}
		else if (!strcmp("rotate", child->Name())) {
			float a = (float) atof(child->Attribute("angle"));
			float x = (float) atof(child->Attribute("axisX"));
			float y = (float) atof(child->Attribute("axisY"));
			float z = (float) atof(child->Attribute("axisZ"));
			childGr.setRotate(a, x, y, z);
		}
		else {
			printf("Wrong file Format");
			break;
		}
		child = child->NextSiblingElement();
	}
}


int sceneParser(char* filename, Scene s) {

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
	// <scene> 
	XMLElement* el = n->ToElement();
	if (strcmp("scene", el->Name())) {
		printf("Wrong file Format");
	}
	// primeiro <group>
	el = el->FirstChildElement();
	if (strcmp("group", el->Name())) {
		printf("Wrong file Format");
	}
	Group root;
	parseGroup(el, root, s);

	return 0;
}




