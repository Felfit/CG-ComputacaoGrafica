#ifndef MODEL3D_H
#define MODEL3D_H

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glut.h>
#include <IL/il.h>
#include "utils.hpp"


class Texture {
	public:
		unsigned int texture;

		int parse(const char* filename);
};

class ModelBuffers {
	public:
		int size = 0;
		GLuint buffers[3];

		int parse(const char* filename);
};


class Model3D {
	public:
		Model3D();
		~Model3D();

		int id;
		ModelBuffers* buffers;
		Texture* texture;
		float diffRGBA[4];
		float ambiRGBA[4];
		float emisRGBA[4];
		float specRGBA[4];
		float shininess; // [0, 128]

		bool followModel(int cameraFollow, Point3D* center);
		const void draw();
		const void drawColor();
};

#endif MODEL3D_H
