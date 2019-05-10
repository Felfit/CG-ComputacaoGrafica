#include "sphere.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include "utils.h"

void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

void sphereNormalsAndUV(Point * x, int xi, int yj, int slices, int stacks) {
	float coords[3] = { x->x,x->y,x->z };
	normalize(coords);
	x->u = ((float)slices - xi) / (float)slices;
	x->v = ((float)yj) / (float)stacks;
	x->nx = coords[0];
	x->ny = coords[1];
	x->nz = coords[2];
}




void sphere(const char* name, float radius, int slices, int stacks) {
	FILE* fp;
	fopen_s(&fp, name, "w");
	fprintf(fp, "%d\n", 6 * (stacks - 1) * slices);

	Point top = Point(0.0, radius, 0.0); // polo do topo
	sphereNormalsAndUV(&top, slices / 2, 0, slices, stacks);
	Point bottom = Point(0.0, -radius, 0.0); // polo do fundo
	sphereNormalsAndUV(&bottom, slices / 2, stacks, slices, stacks);
	float alfa = 0.0f; // ângulo atual a ser desenhado
	float delta = (2 * M_PI) / slices; // variação do alfa para o próximo meridiano
	float zeta = M_PI / stacks; // variação do beta para o próximo paralelo
	float betaT = M_PI / 2 - zeta; // beta dos pontos no paralelo abaixo do polo mais acima
	float yt = radius * sin(betaT); // y dos pontos no primeiro paralelo abaixo do polo
	float yb = -yt; // y dos pontos no primeiro paralelo acima do polo
	float gr = radius * cos(betaT); // raio dos pontos no primeiro e no último paralelo
	float x1, x2, z1, z2;
	// Ligar os pontos em triangulos
	for (int i = 0; i < slices; ++i) {
		Point p1, p2, p3, p4;
		float alfaLeft = alfa + delta;
		x1 = gr * cos(alfa); // Coordenada x do p1
		x2 = gr * cos(alfaLeft); // Coordenada x do p2
		z1 = gr * sin(alfa); // Coordenada z do p1
		z2 = gr * sin(alfaLeft); // Coordenada z do p2
		// Imprime os triangulos do topo
		p1 = Point(x1, yt, z1);
		p2 = Point(x2, yt, z2);
		sphereNormalsAndUV(&p1, i, 1, slices, stacks);
		sphereNormalsAndUV(&p2, i + 1, 1, slices, stacks);
		printUpdatedTriangle(fp, p2, p1, top);
		// Imprime os triangulos do fundo
		p1 = Point(x1, yb, z1);
		p2 = Point(x2, yb, z2);
		sphereNormalsAndUV(&p1, i, stacks - 1, slices, stacks);
		sphereNormalsAndUV(&p2, i + 1, stacks - 1, slices, stacks);
		printUpdatedTriangle(fp, p1, p2, bottom);
		// Imprime os restantes
		float beta = betaT; // Começa com beta em cima
		for (int j = 0; j < stacks - 2; j++) { // Imprime até chegar ao penúltimo paralelo
			float rt = radius * cos(beta); // raio dos pontos
			float rb = radius * cos(beta - zeta); // raio dos pontos de baixo
			float yt = radius * sin(beta); // Coordenada y dos pontos
			float yb = radius * sin(beta - zeta); // Coordenada y dos pontos de baixo
			x1 = cos(alfa); // Coordenada x dos pontos da direita
			x2 = cos(alfaLeft); // Coordenada x dos pontos da esquerda
			z1 = sin(alfa); // Coordenada z dos pontos da direta
			z2 = sin(alfaLeft); // Coordenada z dos pontos da esquerda
			p1 = Point(rb * x1, yb, rb * z1);
			p2 = Point(rt * x1, yt, rt * z1);
			p3 = Point(rt * x2, yt, rt * z2);
			p4 = Point(rb * x2, yb, rb * z2);
			sphereNormalsAndUV(&p1, i, j + 2, slices, stacks);
			sphereNormalsAndUV(&p2, i, j + 1, slices, stacks);
			sphereNormalsAndUV(&p3, i + 1, j + 1, slices, stacks);
			sphereNormalsAndUV(&p4, i + 1, j + 2, slices, stacks);
			// Liga os pontos do paralelo j com os pontos do paralelo inferior
			printUpdatedSquare(fp, p1, p2, p3, p4);
			beta -= zeta; // roda a esfera para baixo
		}
		alfa += delta; // roda a esfera para a esquerda
	}

	fclose(fp);
}