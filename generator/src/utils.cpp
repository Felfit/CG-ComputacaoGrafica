#include "utils.h"


Point::Point() {

};

Point::Point(float px, float py, float pz) {
	x = px;
	y = py;
	z = pz;
	nx = 0;
	ny = 0;
	nz = 0;
	u = 0;
	v = 0;
}

Point::Point(float px, float py, float pz, float pnx, float pny, float pnz, float pu, float pv) {
	x = px;
	y = py;
	z = pz;
	nx = pnx;
	ny = pny;
	nz = pnz;
	u = pu;
	v = pv;
}


void printUpdatedTriangle(FILE* fp, Point p1, Point p2, Point p3) {
	fprintf(fp, "%f %f %f %f %f %f %f %f\n", p1.x, p1.y, p1.z, p1.nx, p1.ny, p1.nz, p1.u, p1.v);
	fprintf(fp, "%f %f %f %f %f %f %f %f\n", p2.x, p2.y, p2.z, p2.nx, p2.ny, p2.nz, p2.u, p2.v);
	fprintf(fp, "%f %f %f %f %f %f %f %f\n", p3.x, p3.y, p3.z, p3.nx, p3.ny, p3.nz, p3.u, p3.v);
}


void printUpdatedSquare(FILE* fp, Point p1, Point p2, Point p3, Point p4) {
	printUpdatedTriangle(fp, p1, p2, p3);
	printUpdatedTriangle(fp, p3, p4, p1);
}
