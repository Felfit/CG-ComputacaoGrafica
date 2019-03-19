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
		printf("Erro a abrir o fichero\n");
		return -1;
	}
	XMLNode* n = doc.FirstChild();
	if (!n) {
		printf("Ficheiro n�o tem nodos\n");
		return -2;
	}
	// <scene> 
	XMLElement* el = n->ToElement();
	if (strcmp("scene", el->Name())) {
		printf("Wrong file Format\n");
	}
	// primeiro <group>
	el = el->FirstChildElement();
	while (el)
	{
		if (strcmp("group", el->Name())) {
			printf("Wrong file Format\n");
		}
		Group *group = new Group();
		parseGroup(el, group);
		groups.push_front(group);
		el=el->NextSiblingElement();
	}
	return 0;
}

float getAttr(XMLElement *element, const char* name, float default) {
	const XMLAttribute *attr = element->FindAttribute(name);
    return attr == nullptr ? default: (float)atof(attr->Value());
}

void Scene::parseGroup(XMLElement* parent, Group* parentGr) {

	XMLElement* child = parent->FirstChildElement();
	printf("<group>\n");
	// transformations -> models -> groups
	//Inicialização
	glPushMatrix();
	glLoadIdentity();
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
					models[filename] = m;
				}
				parentGr->addModel(models[filename]);
				model = model->NextSiblingElement();
			}
			printf("</models>\n");
		}
		else if (!strcmp("translate", child->Name())) {
			float x = getAttr(child, "X", 0);
			float y = getAttr(child, "Y", 0);
			float z = getAttr(child, "Z", 0);
			printf("translate %f %f %f\n", x, y, z);
			glTranslatef(x, y, z);
		}
		else if (!strcmp("scale", child->Name())) {
			float x = getAttr(child, "X", 1);
			float y = getAttr(child, "Y", 1);
			float z = getAttr(child, "Z", 1);
			printf("scale %f %f %f\n", x, y, z);
			glScalef(x, y, z);
		}
		else if (!strcmp("rotate", child->Name())) {
			float a = getAttr(child, "angle", 0);
			float x = getAttr(child, "axisX", 0);
			float y = getAttr(child, "axisY", 0);
			float z = getAttr(child, "axisZ", 0);
			printf("rotate %f %f %f\n", x, y, z);
			glRotatef(a, x, y, z);
		}
		else {
			printf("Wrong file Format");
			break;
		}
		child = child->NextSiblingElement();
	}
	GLfloat *matrix = new GLfloat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	parentGr->matrix = matrix;
	glPopMatrix();
	printf("</group>\n");
}

void Scene::draw() {
	for (auto const& group : groups) {
		group->draw();
	}
}



