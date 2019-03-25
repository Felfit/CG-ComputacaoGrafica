#include "Scene.h"


using namespace std;
using namespace tinyxml2;

Scene::Scene() {
}

Scene::~Scene() {

}

int Scene::parse(char *filename) {
	XMLDocument doc;
	XMLError eresult = doc.LoadFile(filename);
	if (eresult != XML_SUCCESS) {
		printf("Erro a abrir o fichero\n");
		return -1;
	}
	XMLNode* n = doc.FirstChild();
	if (!n) {
		printf("Ficheiro nao tem nodos\n");
		return -2;
	}
	// <scene> 
	XMLElement* el = n->ToElement();
	if (strcmp("scene", el->Name())) {
		printf("Wrong file format\n");
	}
	// percorre os <group> do primeiro nível
	el = el->FirstChildElement();
	while (el) {
		if (strcmp("group", el->Name())) {
			printf("Wrong file format\n");
		}
		Group *group = new Group();
		parseGroup(el, group);
		groups.push_front(group);
		el=el->NextSiblingElement();
	}
	return 0;
}

float getAttr(XMLElement *element, const char *name, float default) {
	const XMLAttribute *attr = element->FindAttribute(name);
    return attr == nullptr ? default: (float)atof(attr->Value());
}

void Scene::parseGroup(XMLElement *parent, Group *parentGr) {

	XMLElement *child = parent->FirstChildElement();
	while (child) {
		if (!strcmp("group", child->Name())) {
			Group* childGr = new Group();
			parentGr->addGroup(childGr);
			parseGroup(child, childGr);
		}
		else if (!strcmp("models", child->Name())) {
			XMLElement *model = child->FirstChildElement();
			while (model) {
				const char *filename = model->Attribute("file");
				if (!models[filename]) {
					Model3D *m = new Model3D();
					m->parse(filename);
					models[filename] = m;
				}
				parentGr->addModel(models[filename]);
				model = model->NextSiblingElement();
			}
		}
		else if (!strcmp("translate", child->Name())) {
			float x = getAttr(child, "X", 0);
			float y = getAttr(child, "Y", 0);
			float z = getAttr(child, "Z", 0);
			parentGr->addTranslate(x, y, z);
		}
		else if (!strcmp("scale", child->Name())) {
			float x = getAttr(child, "X", 1);
			float y = getAttr(child, "Y", 1);
			float z = getAttr(child, "Z", 1);
			parentGr->addScale(x, y, z);
		}
		else if (!strcmp("rotate", child->Name())) {
			float a = getAttr(child, "angle", 0);
			float x = getAttr(child, "axisX", 0);
			float y = getAttr(child, "axisY", 0);
			float z = getAttr(child, "axisZ", 0);
			parentGr->addRotate(a, x, y, z);
		}
		else {
			printf("Wrong file format\n");
			break;
		}
		child = child->NextSiblingElement();
	}
}

void Scene::draw() {
	for (auto const& group : groups) {
		group->draw();
	}
}



