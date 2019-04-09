#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "curves.h"

void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

float length(float *v) {

	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

void getCurvePoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv, float curveMatrix[4][4] ) {
	
	// Compute A = M * P
	float px[4] = { p0[0], p1[0], p2[0], p3[0] };
	float ax[4];
	multMatrixVector((float*)curveMatrix, (float*)px, (float *)&ax);

	float py[4] = { p0[1], p1[1], p2[1], p3[1] };
	float ay[4];
	multMatrixVector((float*)curveMatrix, (float*)py, (float *)&ay);

	float pz[4] = { p0[2], p1[2], p2[2], p3[2] };
	float az[4];
	multMatrixVector((float*)curveMatrix, (float*)pz, (float *)&az);
	// Compute pos = T * A
	float t2 = t * t;
	float t3 = t2 * t;
	float ts[4] = { t3, t2, t, 1 };
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	for (int i = 0; i < 4; i++) {
		pos[0] += ts[i] * ax[i];
		pos[1] += ts[i] * ay[i];
		pos[2] += ts[i] * az[i];
	}
	// compute deriv = T' * A
	float dts[4] = { 3*t2, 2*t, 1, 0 };
	for (int i = 0; i < 4; i++) {
		deriv[0] += dts[i] * ax[i];
		deriv[1] += dts[i] * ay[i];
		deriv[2] += dts[i] * az[i];
	}
	// ...
}

// given  global t, returns the point in the curve
void getGlobalCatmullRomCurvePoint(float points [][3],int numpoints,float gt, float *pos, float *deriv) {

	float t = gt * numpoints; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int segpoinst = numpoints - 3;
	int indices[4]; 
	indices[0] = (index)%segpoinst;
	indices[1] = (indices[0]+1);
	indices[2] = (indices[1]+1); 
	indices[3] = (indices[2]+1);

	getCurvePoint(t, points[indices[0]], points[indices[1]], points[indices[2]], points[indices[3]], pos, deriv, catmullMatrix);
}

void getGlobalCatmullRomCurvePoint(std::vector<Point3D> points, float gt, float *pos, float *deriv) {
	int numpoints = points.size();
	float t = gt * numpoints; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int segpoinst = numpoints - 3;
	int indices[4];
	indices[0] = (index) % segpoinst;
	indices[1] = (indices[0] + 1);
	indices[2] = (indices[1] + 1);
	indices[3] = (indices[2] + 1);
	
	float p[4][3] = {   {points.at(indices[0]).x,points.at(indices[0]).y,points.at(indices[0]).z},
						{points.at(indices[1]).x,points.at(indices[1]).y,points.at(indices[1]).z},
						{points.at(indices[2]).x,points.at(indices[3]).y,points.at(indices[2]).z},
						{points.at(indices[3]).x,points.at(indices[3]).y,points.at(indices[3]).z}};
	getCurvePoint(t, p[0], p[1], p[2], p[3], pos, deriv, catmullMatrix);
}

void getGlobalBezierCurvePoint(std::vector<Point3D> points, float gt, float *pos, float *deriv) {
	int numSegments = points.size() / 4;
	float t = gt * numSegments;
	int segment = (int)floor(t) % numSegments;
	t = t - floor(t);
	int indices[4];
	indices[0] = segment*4;
	indices[1] = (indices[0] + 1);
	indices[2] = (indices[1] + 1);
	indices[3] = (indices[2] + 1);
	float p[4][3] = { {points.at(indices[0]).x,points.at(indices[0]).y,points.at(indices[0]).z},
					  {points.at(indices[1]).x,points.at(indices[1]).y,points.at(indices[1]).z},
					  {points.at(indices[2]).x,points.at(indices[3]).y,points.at(indices[2]).z},
					  {points.at(indices[3]).x,points.at(indices[3]).y,points.at(indices[3]).z}};
	getCurvePoint(t, p[0], p[1], p[2], p[3], pos, deriv, bezierMatrix);
}

void renderCatmullCurve(int npoints, float p[][3],int pointcount,float m[4][4]) {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < npoints; i++)
	{
		float pos[4] = { 0 };
		float der[4] = { 0 };
		getGlobalCatmullRomCurvePoint(p,pointcount,i / (float)npoints, pos, der);
		glVertex3f(pos[0], pos[1], pos[2]);
	}
	glEnd();
}

void renderSingleBezierCurve(int npoints, float p0[3], float p1[3], float p2[3], float p3[3]) {
	glBegin(GL_LINE_STRIP);

	for (int i = 0; i < npoints; i++)
	{
		float pos[4] = { 0 };
		float der[4] = { 0 };
		getCurvePoint(i/(float)npoints, p0, p1, p2, p3, pos, der, bezierMatrix);
		glVertex3f(pos[0], pos[1], pos[2]);
	}
	glEnd();
}

