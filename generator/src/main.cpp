#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926

struct Point {
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
struct Vector2D
{
	float i;
	float j;
};

Vector2D newVector2D(float i, float j) {
	Vector2D v;
	v.i = i;
	v.j = j;
	return v;
}
/*
z --> faces paralelas a yox
x --> faces paralelas a yoz
y --> faces paralelas a zox
*/
Point calcPoint(Point p, int i, int j, Vector2D v, char type) {
	Point novo;
	switch (type) {
	case 'z':
		novo.x = p.x + i * v.i;
		novo.y = p.y + j * v.j;
		novo.z = p.z;
		break;
	case 'x':
		novo.x = p.x;
		novo.y = p.y + j * v.j;
		novo.z = p.z + i * v.i;
		break;

	case 'y':
		novo.x = p.x + i * v.i;
		novo.y = p.y;
		novo.z = p.z + j * v.j;
		break;
	}
	return novo;
}
void printTriangle(FILE *fp, Point p1, Point p2, Point p3) {
	fprintf(fp, "%f %f %f\n", p1.x, p1.y, p1.z);
	fprintf(fp, "%f %f %f\n", p2.x, p2.y, p2.z);
	fprintf(fp, "%f %f %f\n", p3.x, p3.y, p3.z);
}

void printTriangleDebug(FILE *fp, Point p1, Point p2, Point p3) {
    if(fp) {
        fprintf(fp, "glVertex3f(%f,%f,%f);\n", p1.x, p1.y, p1.z);
        fprintf(fp, "glVertex3f(%f,%f,%f);\n", p2.x, p2.y, p2.z);
        fprintf(fp, "glVertex3f(%f,%f,%f);\n", p3.x, p3.y, p3.z);
    } else {
        printf("glVertex3f(%f,%f,%f);\n", p1.x, p1.y, p1.z);
        printf("glVertex3f(%f,%f,%f);\n", p2.x, p2.y, p2.z);
        printf("glVertex3f(%f,%f,%f);\n", p3.x, p3.y, p3.z);
    }
}
/*
 * C -- B
 * | \\ |
 * D -- A
 *
 * printSquare(A,B,C,D);
 */
void printSquare(FILE *fp, Point p1, Point p2, Point p3, Point p4) {
	printTriangle(fp, p1, p2, p3);
	printTriangle(fp, p3, p4, p1);
}

void printSquareDebug(FILE *fp, Point p1, Point p2, Point p3, Point p4) {
    printTriangleDebug(fp, p1, p2, p3);
    printTriangleDebug(fp, p3, p4, p1);
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
//Constroi uma face
void printMultiSquare(Point start, char type, int rWise, Vector2D v, int div) {
	for (int j = 1; j <= div; j++) {
		for (int i = 1; i <= div; i++) {
			Point root = calcPoint(start, i - 1, j - 1, v, type);
			Point a = calcPoint(start, i, j, v, type);
			Point b = calcPoint(start, i, j - 1, v, type);
			Point c = calcPoint(start, i - 1, j, v, type);
			if (rWise == 1) {
				printSquare(root, c, a, b);
			}
			else {
				printSquare(root, b, a, c);
			}
		}
	}
}
void box(const char* name, float x, float y, float z, int div) {
	FILE *fp;
	fp = fopen(name, "w");
	float hx = x / 2;
	float hy = y / 2;
	float hz = z / 2;

	//pontos para servir de origem de refencial de cada face (pode parecer um bocado aleatório, se quiserem explico)
	Point front = newPoint(-hx, -hy, hz);
	Point right = newPoint(hx, -hy, -hz);
	Point left  = newPoint(-hx, -hy, -hz);
	Point back  = newPoint(-hx, -hy, -hz);
	Point up	= newPoint(-hx, hy, -hz);
	Point down  = newPoint(-hx, -hy, -hz);

	//vetores para usar nos ciclos e obter os pontos dos quadrados
	Vector2D vx = newVector2D((float)dimZ / div, (float)dimY / div);
	Vector2D vy = newVector2D((float)dimX / div, (float)dimZ / div);
	Vector2D vz = newVector2D((float)dimX / div, (float)dimY / div);

	//desenha
	printMultiSquare(front, 'z', 2, vz, div);
	printMultiSquare(right, 'x', 1, vx, div);
	printMultiSquare(left, 'x', 2, vx, div);
	printMultiSquare(back, 'z', 1, vz, div);
	printMultiSquare(up, 'y', 1, vy, div);
	printMultiSquare(down, 'y', 2, vy, div);

	fclose(fp);
}

void sphere(const char* name, float radius, int slices, int stacks) {
	FILE *fp;
	fp = fopen(name, "w");
    Point** points = new Point*[stacks-1];
    for (int i = 0; i < stacks; i++)
        points[i] = new Point[slices];
    Point top = newPoint(0.0, radius, 0.0); // Ponto no topo da esfera

    //Restantes pontos de cima para baixo e no sentido contrário dos ponteiros do relógio
    for (int i = 1; i < stacks; ++i) {
        double phi = PI * (double) i / (double) stacks;
        float y = radius * cos(phi); // Posição y do ponto
        double r = radius * sin(phi); // Projeção do ponto no plano xy
        for (int j = 0; j < slices; ++j) {
            double teta = 2 * PI * (double) j / (double) slices;
            float x = r * cos(teta); // Posição x do ponto
            float z = r * sin(teta); // Posição z do ponto
            points[i-1][j] = newPoint(x, y, z); // Adiciona ao array de pontos
        }
    }

    Point bottom = newPoint(0.0, -radius, 0.0); // Ponto no fundo da esfera
    for(int i = 0; i < slices; ++i) {
        Point p1, p2, p3, p4;

        //Ligar os pontos no topo da esfera
        p1 = points[0][i]; // Ponto no primeiro paralelo depois do ponto no topo
        p2 = points[0][(i+1)%slices]; // Ponto à direita do p1
        printTriangle(fp, p1, p2, top); // Triangulos de cima
        //Ligar os pontos no meio da esfera
        for(int j = 0; j < stacks-2; j++) {
            p4 = points[j+1][i]; // Ponto esquerda-baixo
            p1 = points[j+1][(i+1)%slices]; // Ponto direita-baixo
            p3 = points[j][i]; // Ponto esquerda-cima
            p2 = points[j][(i+1)%slices]; // Ponto direita-cima
            printSquare(fp, p1, p2, p3, p4);
        }
        //Ligar os pontos no fundo da esfera
        p1 = points[stacks-2][i]; // Ponto no primeiro paralelo depois do ponto no topo
        p2 = points[stacks-2][(i+1)%slices]; // Ponto à direita do p1
        printTriangle(fp, p1, p2, bottom); // Triangulos de baixo
    }

	// cleanup
	for (int i = 0; i < stacks; i++)
		delete[] points[i];
	delete[] points;

	fclose(fp);
}

void cone(const char* name, float radius, float height, int slices, int stacks) {
    FILE *fp;
    fp = fopen(name, "w");
    float x;    float y;    float z;
    float teta = PI * 2 / (float)slices;
    float stackHeight = height / stacks; float cost = cos(teta); float sent = sin(teta);//constants
    float upx;    float upz;    float upy;
    x = radius;    z = 0;    y = 0;
    for (int i = 1; i <= stacks; i++)
    {
        float nxtheight = i * stackHeight;
        float nxtradius = (height - nxtheight)*radius / height;
        upx = nxtradius; upz = 0; upy = nxtheight;
        for (int j = 0; j < slices; j++)
        {
            float upnx = upx * cost - upz * sent;
            float upnz = upx * sent + upz * cost;
            float nx = x * cost - z * sent;
            float nz = x * sent + z * cost;
            Point p1 = newPoint(x, y, z);
            Point p2 = newPoint(upx, upy, upz);
            Point p3 = newPoint(nx, y, nz);
			printTriangle(fp, p1, p2, p3);
            if (i != stacks) {//para nao desenhar o triangulo do topo duas vezes
            	p1 = newPoint(upnx, upy, upnz);
            	p2 = newPoint(nx, y, nz);
            	p3 = newPoint(upx, upy, upz);
				printTriangle(fp, p1, p2, p3);
            }
            x = nx; z = nz; upx = upnx; upz = upnz;
        }
        x = nxtradius; z = 0; y = nxtheight;
    }
    x = radius;    z = 0;
    for (int i = 0; i < slices; i++)
    {
        float nx = x * cost - z * sent;
        float nz = x * sent + z * cost;
        Point p1 = newPoint(nx, 0, nz);
        Point p2 = newPoint(0, 0, 0);
        Point p3 = newPoint(x, 0, z);
        printTriangle(fp, p1, p2, p3);
        x = nx; z = nz;
    }
    fclose(fp);
}
/*
void cone(const char* name, float radius, float height, int slices, int stacks) {
	FILE *fp;
	fp = fopen(name, "w");
	Point top = newPoint(0, height,0);
    Point points[stacks][slices];
	for (int i = 1; i <= stacks; ++i) {
	    float percentage = i / (float) stacks;
	    float y = height - height * percentage;
        float r = radius * percentage;
        // Faz os pontos dos circulos
        for (int j = 0; j < slices; ++j) {
            double teta = 2 * PI * (double) j / (double) slices;
            float x = r * cos(teta); // Posição x do ponto
            float z = r * sin(teta); // Posição z do ponto
            points[i - 1][j] = newPoint(x, y, z); // Adiciona ao array de pontos
        }
    }
	for(int i = 0; i < slices; i++){
	    Point p1, p2, p3, p4;
	    // Liga os pontos no topo
        p1 = points[0][(i+1)%slices];
	    p2 = points[0][i];
	    p3 = top;
	    printTriangle(fp, p1, p2, p3);
	    // Liga os restantes pontos das faces
        for (int j = 0; j < stacks-1; ++j) {
            p1 = points[j+1][i]; // Ponto esquerda-baixo
            p4 = points[j+1][(i+1)%slices]; // Ponto direita-baixo
            p2 = points[j][i]; // Ponto esquerda-cima
            p3 = points[j][(i+1)%slices]; // Ponto direita-cima
            printSquare(fp, p1, p2, p3, p4);
        }
	}
    Point pivot = newPoint(0,0,0);
	for(int i = 2; i < slices; i++){
        Point p1 = points[stacks-1][i];
        Point p2 = points[stacks-1][i-1];
	    printTriangle(fp, p1, p2, pivot);
	}
	fclose(fp);
}
*/
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
		if (argc <= 6) {
			fputs("Usage: generator sphere <radius> <height> <slices> <stacks> <output>", stdout);
			return 1;
		}

		float radius = atof(argv[2]);
		float height = atof(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);

		cone(argv[6], radius, height, slices, stacks);
	}


    return 0;
}




