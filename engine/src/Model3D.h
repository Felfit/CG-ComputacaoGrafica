#ifndef MODEL3D_H
#define MODEL3D_H

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glut.h>
#include <IL/il.h>

struct Point3D {
	float x;
	float y;
	float z;
};

class Texture {
	public:
		unsigned int texture;
		int parse(const char* filename);
};

class ModelBuffers {
	public:
		~ModelBuffers();
		int size = 0;
		GLuint buffers[3];
		int parse(const char* filename);
};


class Model3D {
	public:
		Model3D();
		~Model3D();
		ModelBuffers* buffers;
		Texture* texture;
		float diffRGBA[4];
		float specRGBA[4];
		float emisRGBA[4];
		float ambiRGBA[4];
		const void draw();
};

#endif MODEL3D_H
