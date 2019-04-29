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

class Model3D {
	private:
		int size = 0;
		GLuint buffers[1];
		float diffRGB[3];
		float specRGB[3];
		float emisRGB[3];
		float ambiRGB[3];
		//texture

	public:
		Model3D();
		int parse(const char* filename);
		const void draw();
		~Model3D();
	};

#endif MODEL3D_H
