#include "cone.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include "utils.h"

void coneSideNormal(Point* p, float height, float radius, float teta) {
	float ang = M_PI_2 - atan2(height, radius);
	//2D x|y
	float x = cos(ang);
	float y = sin(ang);
	float z = 0;
	//Apply rotation Theta
	p->nx = cos(teta) * x;
	p->ny = y;
	p->nz = sin(teta) * x;
}

void cone(const char* name, float radius, float height, int slices, int stacks) {
	FILE* fp;
	fopen_s(&fp, name, "w");

	fprintf(fp, "%d\n", stacks * slices * 6);

	float x = radius;
	float y = 0;
	float z = 0;
	const float teta = M_PI * 2 / (float)slices;
	const float cost = cos(teta);
	const float sent = sin(teta);

	const float stackHeight = height / stacks;
	for (int i = 1; i <= stacks; i++) {
		float nxtheight = i * stackHeight;
		float nxtradius = (height - nxtheight) * radius / height;

		float upz = 0;
		float upy = nxtheight;
		float upx = nxtradius;

		for (int j = 0; j < slices; j++) {
			float upnx = upx * cost - upz * sent;
			float upnz = upx * sent + upz * cost;
			float nx = x * cost - z * sent;
			float nz = x * sent + z * cost;
			Point p1 = Point(x, y, z);
			Point p2 = Point(upx, upy, upz);
			Point p3 = Point(nx, y, nz);
			coneSideNormal(&p1, height, radius, teta * j);
			coneSideNormal(&p2, height, radius, teta * j);
			coneSideNormal(&p3, height, radius, teta * (j + 1));
			printUpdatedTriangle(fp, p1, p2, p3);
			if (i != stacks) {//para nao desenhar o triangulo do topo duas vezes
				p1 = Point(upnx, upy, upnz);
				p2 = Point(nx, y, nz);
				p3 = Point(upx, upy, upz);
				coneSideNormal(&p1, height, radius, teta * (j + 1));
				coneSideNormal(&p2, height, radius, teta * (j + 1));
				coneSideNormal(&p3, height, radius, teta * j);
				printUpdatedTriangle(fp, p1, p2, p3);
			}
			x = nx;
			z = nz;
			upx = upnx;
			upz = upnz;
		}
		x = nxtradius;
		z = 0;
		y = nxtheight;
	}
	//TODO: Calcular UV coords cilindro
	int u = 0;
	int v = 0;
	x = radius;
	for (int i = 0; i < slices; i++) {
		float nx = x * cost - z * sent;
		float nz = x * sent + z * cost;
		Point p1 = Point(nx, 0, nz, 0, -1, 0, u, v);
		Point p2 = Point(0, 0, 0, 0, -1, 0, u, v);
		Point p3 = Point(x, 0, z, 0, -1, 0, u, v);
		printUpdatedTriangle(fp, p1, p2, p3);
		x = nx;
		z = nz;
	}

	fclose(fp);
}
