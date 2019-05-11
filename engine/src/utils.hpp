#ifndef UTILS_H
#define UTILS_H

struct Point3D {
	float x;
	float y;
	float z;
	Point3D() {
		x = 0, y = 0, z = 0;
	};
	Point3D(float nx, float ny, float nz) {
		x = nx;
		y = ny;
		z = nz;
	}
};

#endif UTILS_H