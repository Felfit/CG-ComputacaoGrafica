#include "box.hpp"

#include "utils.h"

struct Vector2D {
	float i, j;
	Vector2D(float ni, float nj) {
		i = ni;
		j = nj;
	}
};


/*
z --> faces paralelas a yox
x --> faces paralelas a yoz
y --> faces paralelas a zox
*/
Point calcPoint(Point p, int i, int j, Vector2D v, char type, int div) {
	Point novo;
	switch (type) {
	case 'z':
		novo.x = p.x + i * v.i;
		novo.y = p.y + j * v.j;
		novo.z = p.z;
		novo.u = (i) / (4.0 * div) + 0.25;
		novo.v = (div - j) / (3.0 * div) + 1.0 / 3;
		novo.nx = 0;
		novo.nx = 0;
		if (novo.z < 0) {
			novo.nz = -1;
			novo.u = (div - i) / (4.0 * div) + 0.75;
		}
		else novo.nz = 1;
		break;
	case 'x':
		novo.x = p.x;
		novo.y = p.y + j * v.j;
		novo.z = p.z + i * v.i;
		novo.u = (div - i) / (4.0 * div) + 0.5;
		novo.v = (div - j) / (3.0 * div) + 1.0 / 3;
		if (novo.x < 0) {
			novo.nx = -1;
			novo.u = (i) / (4.0 * div);
		}
		else novo.nx = 1;
		break;

	case 'y':
		novo.x = p.x + i * v.i;
		novo.y = p.y;
		novo.z = p.z + j * v.j;
		novo.u = (i) / (4.0 * div) + 0.25;
		novo.v = (j) / (3.0 * div);
		if (novo.y < 0) {
			novo.v = (div - j) / (3.0 * div) + 2.0 / 3;;
			novo.ny = -1;
		}
		break;
	}
	return novo;
}

//Constroi uma face
void printMultiSquare(FILE * fp, Point start, char type, int rWise, Vector2D v, int div) {
	for (int j = 1; j <= div; j++) {
		for (int i = 1; i <= div; i++) {
			Point root = calcPoint(start, i - 1, j - 1, v, type, div);
			Point a = calcPoint(start, i, j, v, type, div);
			Point b = calcPoint(start, i, j - 1, v, type, div);
			Point c = calcPoint(start, i - 1, j, v, type, div);
			if (rWise == 1) {
				printUpdatedSquare(fp, root, c, a, b);
			}
			else {
				printUpdatedSquare(fp, root, b, a, c);
			}
		}
	}
}

void box(const char* name, float x, float y, float z, int div) {
	FILE* fp;
	fopen_s(&fp, name, "w");
	const float hx = x / 2;
	const float hy = y / 2;
	const float hz = z / 2;

	//pontos para servir de origem de refencial de cada face (pode parecer um bocado aleatório, se quiserem explico)
	Point front = Point(-hx, -hy, hz);
	Point right = Point(hx, -hy, -hz);
	Point left = Point(-hx, -hy, -hz);
	Point back = Point(-hx, -hy, -hz);
	Point up = Point(-hx, hy, -hz);
	Point down = Point(-hx, -hy, -hz);

	//vetores para usar nos ciclos e obter os pontos dos quadrados
	Vector2D vx = Vector2D(z / div, y / div);
	Vector2D vy = Vector2D(x / div, z / div);
	Vector2D vz = Vector2D(x / div, y / div);

	//desenha
	fprintf(fp, "%d\n", div * div * 36);
	printMultiSquare(fp, front, 'z', 2, vz, div);
	printMultiSquare(fp, right, 'x', 1, vx, div);
	printMultiSquare(fp, left, 'x', 2, vx, div);
	printMultiSquare(fp, back, 'z', 1, vz, div);
	printMultiSquare(fp, up, 'y', 1, vy, div);
	printMultiSquare(fp, down, 'y', 2, vy, div);

	fclose(fp);
}