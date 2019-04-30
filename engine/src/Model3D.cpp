#include<GL/glew.h>
#include "Model3D.h"
using namespace std;


const void Model3D::draw() {

	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

	/*
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, buffers->vertexB[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, buffers->size);

	// glBindTexture(GL_TEXTURE_2D, 0);
}

Model3D::Model3D() {
}

Model3D::~Model3D() {
}


ModelBuffers::~ModelBuffers() {
	glDeleteBuffers(1, vertexB);
}

int ModelBuffers::parse(const char* filename) {
	string line;
	ifstream file;
	file.open(filename);

	if (file.is_open()) {
		getline(file, line);
		int count = stoi(line);
		float *pointsf = new float[count * 3];
		for (int i = 0; getline(file, line); i++) {
			Point3D p;

			const string delimiter = " ";

			size_t pos = 0;
			std::string token;

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			p.x = stof(token);
			line.erase(0, pos + delimiter.length());

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			p.y = stof(token);
			line.erase(0, pos + delimiter.length());

			p.z = stof(line);

			pointsf[i * 3] = p.x;
			pointsf[i * 3 + 1] = p.y;
			pointsf[i * 3 + 2] = p.z;
			size++;
		}
		file.close();

		glGenBuffers(1, vertexB);
		glBindBuffer(GL_ARRAY_BUFFER, vertexB[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size*3, pointsf, GL_STATIC_DRAW);
		/*
		glGenBuffers(3, buffers);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), &(position[0]), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), &(normal[0]), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(float), &(texCoord[0]), GL_STATIC_DRAW);
		*/
		delete[] pointsf;
	}
	else fprintf(stderr, "%s: %s\n", strerror(errno), filename);


	file.close();

	return 0;
}




int Texture::parse(const char* filename) {
	ifstream file;
	file.open(filename);

	if (file.is_open()) {
		// TODO: coisas
	}
	else fprintf(stderr, "%s: %s\n", strerror(errno), filename);


	file.close();

	return 0;
}

