#include <stdio.h>;
#include <string.h>;
#include <stdlib.h>;

void printVertice(FILE *fp, float x, float y, float z) {
	fprintf(fp, "%f %f %f\n", x, y, z);
}

void plane(const char* name, float x, float z) {
	FILE *fp;
	fp = fopen(name, "w");

	printVertice(fp, x, 0, z);

	fclose(fp);
}

void box(const char* name, float x, float y, float z) {
	FILE *fp;
	fp = fopen(name, "w");
	float hx = x / 2;
	float hy = y / 2;
	float hz = z / 2;

	//face down
	printVertice(fp, hx, -hy, hz);
	printVertice(fp, -hx, -hy, hz);
	printVertice(fp, -hx, -hy, -hz);

	printVertice(fp, -hx, -hy, -hz);
	printVertice(fp, hx, -hy, -hz);
	printVertice(fp, hx, -hy, hz);

	//face up
	printVertice(fp, -hx, hy, -hz);
	printVertice(fp, -hx, hy, hz);
	printVertice(fp, hx, hy, hz);

	printVertice(fp, hx, hy, hz);
	printVertice(fp, hx, hy, -hz);
	printVertice(fp, -hx, hy, -hz);

	//face front
	//face back
	//face right
	//face left
	

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




