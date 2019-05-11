#include "Scene.h"
#include "main.h"
#include "Model3D.h"
using namespace std;
using namespace tinyxml2;

bool drawCurve;

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

	// adiciona skybox se tiver
	if (!strcmp("skybox", el->Name())) {
		parseSkybox(el);
		el = el->NextSiblingElement();
	}
	
	// adiciona luzes
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
		if (nl == 0) {
			cerr << "Deve existir pelo menos uma luz\n";
			return 1;
		}
		if (nl == GL_MAX_LIGHTS) {
			cerr << "O numero de luzes atingiu o limite maximo\n";
			return 1;
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

void Scene::parseSkybox(XMLElement* el) {
	const char* model = el->Attribute("model");
	if (model == nullptr) {
		throw "skybox model missing";
	}
	const char* texture = el->Attribute("texture");
	if (texture == nullptr) {
		throw "skybox texture missing";
	}
	hasSkybox = true;
	skybox.buffers = new ModelBuffers;
	skybox.buffers->parse(model);
	skybox.texture = new Texture;
	skybox.texture->parse(texture);
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
		l->dirY = getAttr(el, "dirY");
		l->dirZ = getAttr(el, "dirZ");
		lights.push_back(l);
	}
	else if (!strcmp("SPOT", type)) {
		LightSpot* l = new LightSpot();
		l->posX = getAttr(el, "posX");
		l->posY = getAttr(el, "posY");
		l->posZ = getAttr(el, "posZ");
		l->dirX = getAttr(el, "dirX");
		l->dirY = getAttr(el, "dirY");
		l->dirZ = getAttr(el, "dirZ");
		l->cutoff = getAttrOrDefault(el, "cutoff", 180);
		l->exponent = getAttrOrDefault(el, "exponent", 0);
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

	// carrega ficheiro .3d
	if (!modelsBuff[filename]) {
		ModelBuffers* m = new ModelBuffers();
		m->parse(filename);
		modelsBuff[filename] = m;
	}
	model3d.buffers = modelsBuff[filename];

	// carrega textura se tiver
	const char *texture = model->Attribute("texture");
	if (texture) {
		if (!textures[texture]) {
			Texture* t = new Texture();
			t->parse(texture);
			textures[texture] = t;
		}
		model3d.texture = textures[texture];
	}

	// guarda nome
	const char* name = model->Attribute("name");
	if (name) {
		model3d.name = name;
	}

	// guarda caracteristicas especificadas do material
	model3d.diffRGBA[0] = getAttrOrDefault(model, "diffR", 0.8);
	model3d.diffRGBA[1] = getAttrOrDefault(model, "diffG", 0.8);
	model3d.diffRGBA[2] = getAttrOrDefault(model, "diffB", 0.8);
	model3d.diffRGBA[3] = 1;

	model3d.ambiRGBA[0] = getAttrOrDefault(model, "ambiR", 0.2);
	model3d.ambiRGBA[1] = getAttrOrDefault(model, "ambiG", 0.2);
	model3d.ambiRGBA[2] = getAttrOrDefault(model, "ambiB", 0.2);
	model3d.ambiRGBA[3] = 1;

	model3d.specRGBA[0] = getAttrOrDefault(model, "specR", 0);
	model3d.specRGBA[1] = getAttrOrDefault(model, "specG", 0);
	model3d.specRGBA[2] = getAttrOrDefault(model, "specB", 0);
	model3d.specRGBA[3] = 1;

	model3d.emisRGBA[0] = getAttrOrDefault(model, "diffR", 0);
	model3d.emisRGBA[1] = getAttrOrDefault(model, "diffG", 0);
	model3d.emisRGBA[2] = getAttrOrDefault(model, "diffB", 0);
	model3d.emisRGBA[3] = 1;

	model3d.shininess = getAttrOrDefault(model, "shininess", 0);

	// id
	model3d.id = ++modelsN;

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
	//Variavel Global. Precisa de reset|para a camera o seguir Para identificar o numero do modelo
	drawCurve = true;
	for (auto const& light : lights) {
		light->create();
	}
	for (auto const& group : groups) {
		group->draw();
	}
}


void Scene::followModel(int cameraFollow, Point3D* center) {

	drawCurve = false;
	if (cameraFollow <= 0)
		return;

	glLoadIdentity();
	for (auto const& group : groups) {
		if(group->followModel(cameraFollow, center))
			return;
	}
}



void Scene::drawColor() {
	//Variavel Global. Para identificar o numero do modelo
	drawCurve = false;
	for (auto const& group : groups) {
		group->drawColor();
	}
}

void Scene::drawSkybox(int camx, int camy, int camz) {
	//Camera não pode seguir a skybox

	if (!this->hasSkybox)
		return;
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glTranslatef(camx, camy, camz);
	this->skybox.draw();
	glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}