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
	if (!strcmp("lights", el->Name())) {
		XMLElement* light = el->FirstChildElement();
		int nl = 0;
		while (light && nl < GL_MAX_LIGHTS) {
			if (strcmp("light", light->Name())) {
				cerr << "Erro no xml: tag desconhecida dentro de lights: " << light->Name() << "\n";
				continue;
			}
			parseLight(light);
			light = light->NextSiblingElement();
			nl++;
		}
		if (nl == GL_MAX_LIGHTS) {
			cerr << "O numero de luzes atingiu o limite maximo\n";
		}
	}
	el = el->NextSiblingElement();
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
	const char* attr = element->Attribute(name);
    return attr == nullptr ? default : (float)atof(attr);
}

float getAttr(XMLElement *element, const char *name) {
	const char* attr = element->Attribute(name);
	if (attr == nullptr) {
		std::string s = name;
		throw s;
	}
    return (float)atof(attr);
}


void Scene::parseLight(XMLElement* el) {
	const char *type = el->Attribute("type");
	if (type == nullptr) {
		std::string s = "luz";
		throw s;
	}
	if (!strcmp("POINT", type)) {
		LightPoint* l = new LightPoint();
		l->posX = getAttr(el, "posX");
		l->posY = getAttr(el, "posY");
		l->posZ = getAttr(el, "posZ");
		lights.push_back(l);
	}
	else if (!strcmp("DIRECTIONAL", type)) {
		LightDirectional* l = new LightDirectional();
		l->dirX = getAttr(el, "dirX");
		l->dirX = getAttr(el, "dirX");
		l->dirX = getAttr(el, "dirX");
		lights.push_back(l);
	}
	else if (!strcmp("SPOT", type)) {
		LightSpot* l = new LightSpot();
		l->posX = getAttr(el, "posX");
		l->posY = getAttr(el, "posY");
		l->posZ = getAttr(el, "posZ");
		lights.push_back(l);
	}
	else {
		cerr << "Erro no xml: luz tem tipo desconhecido: " << type << "\n";
	}
}


void Scene::parseModel(XMLElement* model, Group* group) {
	const char *filename = model->Attribute("file");
	if (!filename) return;

	Model3D model3d;

	if (!models[filename]) {
		ModelBuffers* m = new ModelBuffers();
		m->parse(filename);
		models[filename] = m;
	}
	model3d.buffers = models[filename];

	const char *texture = model->Attribute("texture");
	if (texture) {
		if (!textures[texture]) {
			Texture* t = new Texture();
			t->parse(texture);
			textures[texture] = t;
		}
		model3d.texture = textures[texture];
	}
	
	model3d.diffRGBA[0] = getAttrOrDefault(model, "diffR", 1);
	model3d.diffRGBA[1] = getAttrOrDefault(model, "diffG", 1);
	model3d.diffRGBA[2] = getAttrOrDefault(model, "diffB", 1);
	model3d.diffRGBA[3] = 1;

	model3d.ambiRGBA[0] = getAttrOrDefault(model, "ambiR", 1);
	model3d.ambiRGBA[1] = getAttrOrDefault(model, "ambiG", 1);
	model3d.ambiRGBA[2] = getAttrOrDefault(model, "ambiB", 1);
	model3d.ambiRGBA[3] = 1;

	model3d.specRGBA[0] = getAttrOrDefault(model, "specR", 1);
	model3d.specRGBA[1] = getAttrOrDefault(model, "specG", 1);
	model3d.specRGBA[2] = getAttrOrDefault(model, "specB", 1);
	model3d.specRGBA[3] = 1;

	model3d.emisRGBA[0] = getAttrOrDefault(model, "diffR", 0);
	model3d.emisRGBA[1] = getAttrOrDefault(model, "diffG", 0);
	model3d.emisRGBA[2] = getAttrOrDefault(model, "diffB", 0);
	model3d.emisRGBA[3] = 0;

	group->addModel(model3d);
}



void parseTranslate(XMLElement* el, Group* group) {
	const char *timeAttr = el->Attribute("time");
	if (timeAttr) {
		TranslateAnim* ta = new TranslateAnim();
		ta->time = (float)atof(timeAttr);
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
				if (strcmp("model", model->Name())) {
					cerr << "Erro no xml: tag desconhecida dentro de models: " << model->Name() << "\n";
					break;
				}
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
			cerr << "Erro no xml: tag desconhecida dentro de group: " << child->Name() << "\n";
			break;
		}
		child = child->NextSiblingElement();
	}
}

void Scene::draw() {
	for (auto const& light : lights) {
		light->create();
	}
	for (auto const& group : groups) {
		group->draw();
	}
}