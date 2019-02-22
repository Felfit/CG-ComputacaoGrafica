#include <stdio.h>;
#include <string.h>;
#include <stdlib.h>;

void plane(float x, float z) {
	FILE *fp;
	fp = fopen("plane.3d", "w");
	fprintf(fp, "%f 0 %f\n", x, z);
	fclose(fp);
}

int main(int argc, char const *argv[]) {
	
    if (argc <= 1) {
        printf("Usage: generator <graphical primitive> ...");
		return 1;
    }
	
    printf("%s",argv[1]);

	 
	if (strcmp(argv[1], "plane") == 0) {
		// a square in the XZ plane, centred in the origin, made with 2 triangles
		if (argc <= 3) {
			printf("Usage: generator plane <X dimension> <Z dimension>");
			return 1;
		}

		float x = atof(argv[2]);
		printf("X: %f", x);

		float z = atof(argv[3]);
		printf("Z: %f", z);

		plane(x, z);
	}
	else if (strcmp(argv[1], "box") == 0)  {
		// requires X, Y and Z dimensions, and optionally the number of divisions
	}
	else if (strcmp(argv[1], "sphere") == 0)  {
		// requires radius, slices and stacks
	}
	else if (strcmp(argv[1], "cone") == 0)  {
		// requires bottom radius, height, slices and stacks
	}

    return 0;
}




