#ifndef UTILS_H
#define UTILS_H

#include <cstdio>

struct Point {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
	Point();
	Point(float px, float py, float pz);
	Point(float px, float py, float pz, float pnx, float pny, float pnz, float pu, float pv);
};

void printUpdatedTriangle(FILE* fp, Point p1, Point p2, Point p3);

/*
 * p3  -  p2
 *  |  \  |
 * p4  -  p1
 */
void printUpdatedSquare(FILE* fp, Point p1, Point p2, Point p3, Point p4);

#endif UTILS_H