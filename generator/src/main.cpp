#include <string.h>
#include <stdlib.h>
#include <cstdio>

#include "patch.h"
#include "plane.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "cone.hpp"


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
	else if (strcmp(argv[1], "box") == 0) {
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
			box(argv[5], x, y, z, 1);
		}
	}
	else if (strcmp(argv[1], "sphere") == 0) {
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
	else if (strcmp(argv[1], "cone") == 0) {
		// requires bottom radius, height, slices and stacks
		if (argc <= 6) {
			fputs("Usage: generator cone <radius> <height> <slices> <stacks> <output>", stdout);
			return 1;
		}

		float radius = atof(argv[2]);
		float height = atof(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);

		cone(argv[6], radius, height, slices, stacks);
	}
	else if (strcmp(argv[1], "bezier") == 0) {
		// requires tessellation level and bezier patches descrition info filename
		if (argc <= 4) {
			fputs("Usage: generator bezier <tessellation> <bezier patches filename> <output>", stdout);
			return 1;
		}

		int tessellation = atoi(argv[2]);
		parsePatches(argv[4], argv[3], tessellation);
	}
	return 0;
}




