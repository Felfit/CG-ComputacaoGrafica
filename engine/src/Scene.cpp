#include "Scene.h"


using namespace std;
using namespace tinyxml2;

Scene::Scene() {
}

Scene::~Scene() {

}

int Scene::parse(char* filename) {
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
	root = new Group();
	parseGroup(el, root);
	return 0;
}

void Scene::parseGroup(XMLElement* parent, Group* parentGr) {

	XMLElement* child = parent->FirstChildElement();
	printf("<group>\n");
	// transformations -> models -> groups
	while (child) {
		if (!strcmp("group", child->Name())) {
			Group* childGr = new Group();
			parentGr->addGroup(childGr);
			parseGroup(child, childGr);
		}
		else if (!strcmp("models", child->Name())) {
			printf("<models>\n");
			XMLElement* model = child->FirstChildElement();
			while (model) {
				const char *filename = model->Attribute("file");
				printf("%s\n", filename);
				if (!models[filename]) {
					Model3D* m = new Model3D();
					m->parse(filename);
					parentGr->addModel(m);
					models[filename] = m;
				}
				model = model->NextSiblingElement();
			}
			printf("</models>\n");
		}
		else if (!strcmp("translate", child->Name())) {
			printf("translate\n");
			float x = (float)atof(child->Attribute("X"));
			float y = (float)atof(child->Attribute("Y"));
			float z = (float)atof(child->Attribute("Z"));
			parentGr->setTranslate(x, y, z);
		}
		else if (!strcmp("scale", child->Name())) {
			printf("scale\n");
			float x = (float)atof(child->Attribute("X"));
			float y = (float)atof(child->Attribute("Y"));
			float z = (float)atof(child->Attribute("Z"));
			parentGr->setScale(x, y, z);
		}
		else if (!strcmp("rotate", child->Name())) {
			printf("rotate\n");
			float a = (float)atof(child->Attribute("angle"));
			float x = (float)atof(child->Attribute("axisX"));
			float y = (float)atof(child->Attribute("axisY"));
			float z = (float)atof(child->Attribute("axisZ"));
			parentGr->setRotate(a, x, y, z);
		}
		else {
			printf("Wrong file Format");
			break;
		}
		child = child->NextSiblingElement();
	}
	printf("</group>\n");
}

void Scene::draw() {
	root->draw();
}



