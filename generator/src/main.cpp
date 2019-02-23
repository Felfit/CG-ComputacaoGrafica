#include <stdio.h>;
#include <string.h>;
#include <stdlib.h>;

struct Point
{
	float x;
	float y;
	float z;
};

Point newPoint(float x, float y, float z) {
	Point p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}

void printTriangle(FILE *fp, Point p1, Point p2, Point p3) {
	fprintf(fp, "%f %f %f\n", p1.x, p1.y, p1.z);
	fprintf(fp, "%f %f %f\n", p2.x, p2.y, p2.z);
	fprintf(fp, "%f %f %f\n", p3.x, p3.y, p3.z);
}

void printSquare(FILE *fp, Point p1, Point p2, Point p3, Point p4) {
	printTriangle(fp, p1, p2, p3);
	printTriangle(fp, p3, p4, p1);
}

void plane(const char* name, float x, float z) {
	FILE *fp;
	fp = fopen(name, "w");

	Point p1 = newPoint(x, 0, z); 
	Point p2 = newPoint(x, 0, -z); 
	Point p3 = newPoint(-x, 0, -z);
	Point p4 = newPoint(-x, 0, z);

	printSquare(fp, p1, p2, p3, p4);

	fclose(fp);
}



void box(const char* name, float x, float y, float z) {
	FILE *fp;
	fp = fopen(name, "w");
	float hx = x / 2;
	float hy = y / 2;
	float hz = z / 2;

	Point p1 = newPoint(hx, -hy, hz);
	Point p2 = newPoint(hx, -hy, -hz);
	Point p3 = newPoint(-hx, -hy, -hz);
	Point p4 = newPoint(-hx, -hy, hz);
	Point p5 = newPoint(hx, hy, hz);
	Point p6 = newPoint(hx, hy, -hz);
	Point p7 = newPoint(-hx, hy, -hz);
	Point p8 = newPoint(-hx, hy, hz);

	//face down
	printSquare(fp, p1, p4, p3, p2);
	//face up
	printSquare(fp, p5, p6, p7, p8);
	//face front
	printSquare(fp, p1, p5, p8, p4);
	//face back
	printSquare(fp, p3, p7, p6, p2);
	//face right
	printSquare(fp, p2, p6, p5, p1);
	//face left
	printSquare(fp, p4, p8, p7, p3);

	fclose(fp);
}

void box(const char* name, float x, float y, float z, int divisions) {
	FILE *fp;
	fp = fopen("box.3d", "w");

	fclose(fp);
}

void sphere(const char* name, float radius, int slices, int stacks) {
	FILE *fp;
	fp = fopen("sphere.3d", "w");

	fclose(fp);
}

void cone(const char* name, float radius, int slices, int stacks) {
	FILE *fp;
	fp = fopen("cone.3d", "w");

	fclose(fp);
}

int main(int argc, char const *argv[]) {
	
    if (argc <= 1) {
		fputs("Usage: generator [OPTION]...\n", stdout);
		return 1;
    }
	
	if (strcmp(argv[1], "plane") == 0) {
		// a square in the XZ plane, centred in the origin, made with 2 triangles
		if (argc <= 4) {
			fputs("Usage: generator plane <X dimension> <Z dimension> <output>", stdout);
			return 1;
		}

		float x = atof(argv[2]);
		float z = atof(argv[3]);

		plane(argv[4], x, z);
	}
	else if (strcmp(argv[1], "box") == 0)  {
		// requires X, Y and Z dimensions, and optionally the number of divisions
		if (argc <= 5) {
			fputs("Usage: generator box <X dimension> <Z dimension> <Z dimension> <divisions(optional)> <output>", stdout);
			return 1;
		}

		float x = atof(argv[2]);
		float y = atof(argv[3]);
		float z = atof(argv[4]);

		if (argc > 6) {
			int divisions = atoi(argv[5]);
			box(argv[6], x, y, z, divisions);
		}
		else {
			box(argv[5], x, y, z);
		}
	}
	else if (strcmp(argv[1], "sphere") == 0)  {
		// requires radius, slices and stacks
		if (argc <= 5) {
			fputs("Usage: generator sphere <radius> <slices> <stacks> <output>", stdout);
			return 1;
		}

		float radius = atof(argv[2]);
		int slices = atoi(argv[3]);
		int stacks = atoi(argv[4]);

		sphere(argv[5], radius, slices, stacks);
	}
	else if (strcmp(argv[1], "cone") == 0)  {
		// requires bottom radius, height, slices and stacks
		if (argc <= 5) {
			fputs("Usage: generator sphere <radius> <slices> <stacks> <output>", stdout);
			return 1;
		}

		float radius = atof(argv[2]);
		int slices = atoi(argv[3]);
		int stacks = atoi(argv[4]);

		cone(argv[5], radius, slices, stacks);
	}


    return 0;
}




