#include<GL/glew.h>
#include "Model3D.h"
using namespace std;


const void Model3D::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	//Dizlhe como ler do buffer
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, size);

}

Model3D::Model3D() {
}

int Model3D::parse(const char* filename) {
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
		glGenBuffers(1, buffers);
		//Set buffer as active
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		//Copia coisas para o buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size*3, pointsf, GL_STATIC_DRAW);
		delete[] pointsf;
	}
	else fprintf(stderr, "%s: %s\n", strerror(errno), filename);


	file.close();

	return 0;
}

Model3D::~Model3D() {
	glDeleteBuffers(1,buffers);
}

