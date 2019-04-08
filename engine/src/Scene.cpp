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

float getAttrOrDefault(XMLElement *element, const char *name, float default) {
	const XMLAttribute *attr = element->FindAttribute(name);
    return attr == nullptr ? default : (float)atof(attr->Value());
}

float getAttr(XMLElement *element, const char *name) {
	const XMLAttribute *attr = element->FindAttribute(name);
    return (float)atof(attr->Value()); // TODO: exception
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
			const XMLAttribute *timeAttr = child->FindAttribute("time");
			if (timeAttr != nullptr) {
				TranslateAnim ta;
				ta.time = (float)atof(timeAttr->Value());

				vector<Point3D> points; // new ?
				ta.points = points;
				XMLElement *point = child->FirstChildElement();
				while (point) {
					Point3D p;
					p.x = getAttr(child, "X");
					p.y = getAttr(child, "Y");
					p.z = getAttr(child, "Z");
					points.push_back(p);
					point = point->NextSiblingElement();
				}
				parentGr->addTranslateAnim(ta);
			}
			else {
				TranslateStatic ts;
				ts.x = getAttrOrDefault(child, "X", 0);
				ts.y = getAttrOrDefault(child, "Y", 0);
				ts.z = getAttrOrDefault(child, "Z", 0);
				parentGr->addTranslate(ts);
			}
		}
		else if (!strcmp("scale", child->Name())) {
			Scale s;
			s.x = getAttrOrDefault(child, "X", 1);
			s.y = getAttrOrDefault(child, "Y", 1);
			s.z = getAttrOrDefault(child, "Z", 1);
			parentGr->addScale(s);
		}
		else if (!strcmp("rotate", child->Name())) {
			RotateStatic rs;
			rs.x = getAttrOrDefault(child, "axisX", 0);
			rs.y = getAttrOrDefault(child, "axisY", 0);
			rs.z = getAttrOrDefault(child, "axisZ", 0);
			const XMLAttribute *timeAttr = child->FindAttribute("time");
			if (timeAttr != nullptr) {
				RotateAnim ra;
				ra.time = (float)atof(timeAttr->Value());
				ra.x = rs.x; ra.y = rs.y; ra.z = rs.z;
				parentGr->addRotateAnim(ra);
			} 
			else {
				rs.angle = getAttrOrDefault(child, "angle", 0);
				parentGr->addRotate(rs);
			}
			
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