 
#include <vector> 
#include "Model3D.h"

float catmullMatrix[4][4] = { {-0.5f, 1.5f, -1.5f, 0.5f},
							  { 1.0f, -2.5f,  2.0f, -0.5f },
							  { -0.5f,  0.0f,  0.5f,  0.0f },
							  { 0.0f,  1.0f,  0.0f,  0.0f } };

float bezierMatrix[4][4] = {{-1.0f,  3.0f, -3.0f, 1.0f},
							{ 3.0f, -6.0f,  3.0f, 0.0f},
							{-3.0f,  3.0f,  0.0f, 0.0f},
							{ 1.0f,  0.0f,  0.0f, 0.0f} };
void getGlobalCurvePoint(std::vector<Point3D> points, float gt, float *pos, float *deriv, float curveMatrix[4][4]);
void getGlobalCurvePoint(float points[][3], int numpoints, float gt, float *pos, float *deriv, float curveMatrix[4][4]);
void getCurvePoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv, float curveMatrix[4][4]);
void buildRotMatrix(float *x, float *y, float *z, float *m);
void cross(float *a, float *b, float *res);
float length(float *v);
void normalize(float *a);
void renderCurve(float p[][3], int pointcount, float m[4][4]);