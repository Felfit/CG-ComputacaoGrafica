#include "Scene.h"
#include "main.h"
#include "Model3D.h"
using namespace std;
using namespace tinyxml2;

bool drawCurve;

Scene::Scene() {
	modelsN = 0;
}

Scene::~Scene() {

}

int Scene::parse(char *filename) {
	// abre ficheiro de configuração xml
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

	// <skybox ... /> adiciona skybox se tiver
	if (!strcmp("skybox", el->Name())) {
		parseSkybox(el);
		el = el->NextSiblingElement();
	}
	
	// <lights> adiciona luzes
	if (!strcmp("lights", el->Name())) {
		XMLElement* light = el->FirstChildElement();
		int nl = 0;
		while (light && nl < GL_MAX_LIGHTS) {
			// <light ... />
			if (strcmp("light", light->Name())) {
				cerr << "Erro no xml: tag desconhecida dentro de lights: " << light->Name() << "\n";
				continue;
			}
			parseLight(light);
			light = light->NextSiblingElement();
			nl++;
		}
		if (nl == 0) {
			cerr << "Erro no xml: deve existir pelo menos uma luz\n";
			return 1;
		}
		if (nl == GL_MAX_LIGHTS) {
			cerr << "Erro no xml: o numero de luzes atingiu o limite maximo\n";
			return 1;
		}
	}
	el = el->NextSiblingElement();

	// <group> percorre os grupos do primeiro nível
	while (el) {
		if (strcmp("group", el->Name())) {
			cerr << "Erro no xml: tag desconhecida dentro de scene\n";
		}
		Group *group = new Group();
		parseGroup(el, group);
		groups.push_front(group);
		el = el->NextSiblingElement();
	}
	return 0;
}

float getAttrOrDefaultf(XMLElement *element, const char *name, float default) {
	const char* attr = element->Attribute(name);
    return attr == nullptr ? default : (float)atof(attr);
}

float getAttrf(XMLElement *element, const char *name) {
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
		cerr << "Erro no xml: a skybox deve conter o atributo model\n";
		return;
	}
	const char* texture = el->Attribute("texture");
	if (texture == nullptr) {
		cerr << "Erro no xml: a skybox deve conter o atributo texture\n";
		return;
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
		l->posX = getAttrf(el, "posX");
		l->posY = getAttrf(el, "posY");
		l->posZ = getAttrf(el, "posZ");
		l->conAtt = getAttrOrDefaultf(el, "conAtt", 1);
		l->linAtt = getAttrOrDefaultf(el, "linAtt", 0);
		l->quaAtt = getAttrOrDefaultf(el, "quaAtt", 0);
		lights.push_back(l);
	}
	else if (!strcmp("DIRECTIONAL", type)) {
		LightDirectional* l = new LightDirectional();
		l->dirX = getAttrf(el, "dirX");
		l->dirY = getAttrf(el, "dirY");
		l->dirZ = getAttrf(el, "dirZ");
		lights.push_back(l);
	}
	else if (!strcmp("SPOT", type)) {
		LightSpot* l = new LightSpot();
		l->posX = getAttrf(el, "posX");
		l->posY = getAttrf(el, "posY");
		l->posZ = getAttrf(el, "posZ");
		l->dirX = getAttrf(el, "dirX");
		l->dirY = getAttrf(el, "dirY");
		l->dirZ = getAttrf(el, "dirZ");
		l->cutoff = getAttrOrDefaultf(el, "cutoff", 180);
		l->exponent = getAttrOrDefaultf(el, "exponent", 0);
		l->conAtt = getAttrOrDefaultf(el, "conAtt", 1);
		l->linAtt = getAttrOrDefaultf(el, "linAtt", 0);
		l->quaAtt = getAttrOrDefaultf(el, "quaAtt", 0);
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

	// guarda caracteristicas do material
	model3d.diffRGBA[0] = getAttrOrDefaultf(model, "diffR", 0.8);
	model3d.diffRGBA[1] = getAttrOrDefaultf(model, "diffG", 0.8);
	model3d.diffRGBA[2] = getAttrOrDefaultf(model, "diffB", 0.8);
	model3d.diffRGBA[3] = 1;

	model3d.ambiRGBA[0] = getAttrOrDefaultf(model, "ambiR", 0.2);
	model3d.ambiRGBA[1] = getAttrOrDefaultf(model, "ambiG", 0.2);
	model3d.ambiRGBA[2] = getAttrOrDefaultf(model, "ambiB", 0.2);
	model3d.ambiRGBA[3] = 1;

	model3d.specRGBA[0] = getAttrOrDefaultf(model, "specR", 0);
	model3d.specRGBA[1] = getAttrOrDefaultf(model, "specG", 0);
	model3d.specRGBA[2] = getAttrOrDefaultf(model, "specB", 0);
	model3d.specRGBA[3] = 1;

	model3d.emisRGBA[0] = getAttrOrDefaultf(model, "emisR", 0);
	model3d.emisRGBA[1] = getAttrOrDefaultf(model, "emisG", 0);
	model3d.emisRGBA[2] = getAttrOrDefaultf(model, "emisB", 0);
	model3d.emisRGBA[3] = 1;

	model3d.shininess = getAttrOrDefaultf(model, "shininess", 0);

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
				p.x = getAttrf(point, "X");
				p.y = getAttrf(point, "Y");
				p.z = getAttrf(point, "Z");
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
		ts->x = getAttrOrDefaultf(el, "X", 0);
		ts->y = getAttrOrDefaultf(el, "Y", 0);
		ts->z = getAttrOrDefaultf(el, "Z", 0);
		group->addTransform(ts);
	}
}

void parseScale(XMLElement* el, Group* group) {
	ScaleStatic* s = new ScaleStatic();
	s->x = getAttrOrDefaultf(el, "X", 1);
	s->y = getAttrOrDefaultf(el, "Y", 1);
	s->z = getAttrOrDefaultf(el, "Z", 1);
	group->addTransform(s);
}

void parseRotate(XMLElement* el, Group* group) {
	float x = getAttrOrDefaultf(el, "axisX", 0);
	float y = getAttrOrDefaultf(el, "axisY", 0);
	float z = getAttrOrDefaultf(el, "axisZ", 0);
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
		rs->angle = getAttrOrDefaultf(el, "angle", 0);
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

void Scene::drawSkybox(float camx, float camy, float camz) {
	//Camera não pode seguir a skybox
	if (!this->hasSkybox)
		return;
	glPushMatrix();
	bool lightingstate = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glTranslatef(camx, camy, camz);
	this->skybox.draw();
	glCullFace(GL_BACK);
	if(lightingstate)
		glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}