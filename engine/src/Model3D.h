#ifndef MODEL3D_H
#define MODEL3D_H

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glut.h>

struct Point3D {
	float x;
	float y;
	float z;
};

class Texture {
	public:
		int parse(const char* filename);
};

class ModelBuffers {
	public:
		~ModelBuffers();
		int size = 0;
		GLuint vertexB[1];
		// TODO: normais e texturas
		int parse(const char* filename);
};


class Model3D {
	public:
		Model3D();
		~Model3D();
		ModelBuffers* buffers;
		Texture* texture;
		float diffRGB[3];
		float specRGB[3];
		float emisRGB[3];
		float ambiRGB[3];
		const void draw();
};

#endif MODEL3D_H
