#include "Model3D.h"
#include <GL/glut.h>
using namespace std;


void Model3D::drawModel()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < size; i++)
	{
		glVertex3f(points[i].x, points[i].y, points[i].z);
	}
	glEnd();
}

Model3D::Model3D() {
}

Model3D::Model3D(string filename) {

	string line;
	ifstream file;
	file.open(filename);

	if (file.is_open())
	{
		getline(file, line);
		int count = stoi(line);
		points = new Point3D[count];

		cout << count << '\n';

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

			// teste
			std::cout << p.x << " " << p.y << " " << p.z << std::endl;


			points[i] = p;
			size++;
		}
		file.close();
	}
	else cout << "Unable to open file";

	file.close();
	
}

Model3D::~Model3D() {
	delete points;
	cout << "Destruction";
}

