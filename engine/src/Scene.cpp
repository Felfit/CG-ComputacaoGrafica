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
		cerr << "Erro a abrir o ficheiro xml de configuracao: " << filename << "\n";
		return -1;
	}
	XMLNode* n = doc.FirstChild();
	if (!n) {
		cerr << "Erro no xml: ficheiro nao tem nodos\n";
		return -2;
	}
	// <scene> 
	XMLElement* el = n->ToElement();
	if (strcmp("scene", el->Name())) {
		cerr << "Erro no xml: primeira tag deve ser scene\n";
	}
	
	el = el->FirstChildElement();
	// adiciona luzes se tiver
	int nl = 0;
	if (!strcmp("lights", el->Name()) && nl < GL_MAX_LIGHTS) {
		XMLElement* light = el->FirstChildElement();
		while (light) {
			if (strcmp("light", light->Name())) {
				parseLight(light);
			}
			light = light->NextSiblingElement();
		}
	}
	// percorre os <group> do primeiro nível
	while (el) {
		if (strcmp("group", el->Name())) {
			cerr << "Erro no xml: a scene deve apenas conter tags group\n";
		}
		Group *group = new Group();
		parseGroup(el, group);
		groups.push_front(group);
		el = el->NextSiblingElement();
	}
	return 0;
}

float getAttrOrDefault(XMLElement *element, const char *name, float default) {
	const XMLAttribute *attr = element->FindAttribute(name);
    return attr == nullptr ? default : (float)atof(attr->Value());
}

float getAttr(XMLElement *element, const char *name) {
	const XMLAttribute *attr = element->FindAttribute(name);
	if (attr == nullptr) {
		std::string s = name;
		throw s;
	}
    return (float)atof(attr->Value());
}



void Scene::parseLight(XMLElement* el) {
	const XMLAttribute *type = el->FindAttribute("type");
	if (type == nullptr) {
		std::string s = "luz";
		throw s;
	}
	if (strcmp("POINT", type->Value())) {
		LightPoint* l = new LightPoint();
		l->posX = getAttr(el, "posX");
		l->posY = getAttr(el, "posY");
		l->posZ = getAttr(el, "posZ");
		lights.push_back(l);
	}
	else if (strcmp("DIRECTIONAL", type->Value())) {
		LightDirectional* l = new LightDirectional();

		// TODO adicionar luz
	}
	else if (strcmp("SPOT", type->Value())) {
		LightSpot* l = new LightSpot();

		// TODO adicionar luz
	}
	else {
		// TODO erro
	}
}

void parseTranslate(XMLElement* el, Group* group) {
	const XMLAttribute *timeAttr = el->FindAttribute("time");
	if (timeAttr) {
		TranslateAnim* ta = new TranslateAnim();
		ta->time = (float)atof(timeAttr->Value());
		try {
			XMLElement* point = el->FirstChildElement();
			int n = 0;
			while (point) {
				Point3D p;
				p.x = getAttr(point, "X");
				p.y = getAttr(point, "Y");
				p.z = getAttr(point, "Z");
				ta->points.push_back(p);
				point = point->NextSiblingElement();
				n++;
			}
			if (n < 4) {
				cerr << "Erro no xml: as translações dinamicas devem ter no minimo 4 pontos\n";
				return;
			}
			group->addTransform(ta);
		}
		catch (string s) {
			cerr << "Erro no xml: numa transformacao dinamica um ponto nao tem a coordenada " << s << "\n";
			delete ta;
			return;
		}
	}
	else {
		TranslateStatic* ts = new TranslateStatic();
		ts->x = getAttrOrDefault(el, "X", 0);
		ts->y = getAttrOrDefault(el, "Y", 0);
		ts->z = getAttrOrDefault(el, "Z", 0);
		group->addTransform(ts);
	}
}

void parseScale(XMLElement* el, Group* group) {
	ScaleStatic* s = new ScaleStatic();
	s->x = getAttrOrDefault(el, "X", 1);
	s->y = getAttrOrDefault(el, "Y", 1);
	s->z = getAttrOrDefault(el, "Z", 1);
	group->addTransform(s);
}

void parseRotate(XMLElement* el, Group* group) {
	float x = getAttrOrDefault(el, "axisX", 0);
	float y = getAttrOrDefault(el, "axisY", 0);
	float z = getAttrOrDefault(el, "axisZ", 0);
	const XMLAttribute *timeAttr = el->FindAttribute("time");
	if (timeAttr != nullptr) {
		RotateAnim* ra = new RotateAnim();
		ra->x = x; ra->y = y; ra->z = z;
		ra->time = (float)atof(timeAttr->Value());
		group->addTransform(ra);
	}
	else {
		RotateStatic* rs = new RotateStatic();
		rs->x = x; rs->y = y; rs->z = z;
		rs->angle = getAttrOrDefault(el, "angle", 0);
		group->addTransform(rs);
	}
}

void Scene::parseModel(XMLElement* model, Group* group) {
	const char *filename = model->Attribute("file");
	if (!models[filename]) {
		Model3D *m = new Model3D();
		m->parse(filename);
		models[filename] = m;
	}
	group->addModel(models[filename]);
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
				parseModel(model, parentGr);
				model = model->NextSiblingElement();
			}
		}
		else if (!strcmp("translate", child->Name())) {
			parseTranslate(child, parentGr);
		}
		else if (!strcmp("scale", child->Name())) {
			parseScale(child, parentGr);
		}
		else if (!strcmp("rotate", child->Name())) {	
			parseRotate(child, parentGr);
		}
		else {
			cerr << "Erro no xml: tag desconhecida dentro de group\n";
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