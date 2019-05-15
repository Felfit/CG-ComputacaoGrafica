#include "plane.hpp"
#include <string>

#include "utils.h"

void plane(const char* name, float x, float z) {
	FILE* fp;
	fopen_s(&fp, name, "w");

	fputs("6\n", fp);

	Point p1 = Point(x, 0, z, 0, 1, 0, 1, 1);
	Point p2 = Point(x, 0, -z, 0, 1, 0, 1, 0);
	Point p3 = Point(-x, 0, -z, 0, 1, 0, 0, 0);
	Point p4 = Point(-x, 0, z, 0, 1, 0, 0, 1);

	printUpdatedSquare(fp, p1, p2, p3, p4);

	fclose(fp);
}

void terrain(const char* name, float x, float z, std::string heightMap) {
	/*
	ilInit();

	// 	Load the height map "terreno.jpg"
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)heightMap);
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	imageData = ilGetData();


	// 	Build the vertex arrays
	const int arraySize = (th - 1) * tw * 6;
	float* vertexB = new float[arraySize];
	// fill arrays with vertex values
	int c = 0;
	for (int k = 0; k < (th - 1); k++) {
		for (int i = 0; i < tw; i++) {
			vertexB[c++] = i;
			vertexB[c++] = h(i, k);
			vertexB[c++] = k;
			vertexB[c++] = i;
			vertexB[c++] = h(i, k + 1);
			vertexB[c++] = (k + 1);
		}
	}
	


	for (int j = 0; j < (th - 1); j++) {
		glDrawArrays(GL_TRIANGLE_STRIP, j * tw * 2, tw * 2);
	}
	*/
}