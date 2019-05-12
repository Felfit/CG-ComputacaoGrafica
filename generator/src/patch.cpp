#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <vector>
#include <algorithm>

#include "patch.h"

using namespace std;

double b[4][4] = {
			{ 1, 0, 0, 0},
			{-3, 3, 0, 0},
			{ 3,-6, 3, 0},
			{-1, 3,-3, 1}
}; // b == M

double bt[4][4] = {
		{ 1,-3, 3,-1},
		{ 0, 3,-6, 3},
		{ 0, 0, 3,-3},
		{ 0, 0, 0, 1}
}; // bt == M^T

std::vector<int> fillArrayInt(int size, std::ifstream& file) {
	vector<int> result;
	std::string delimiter = ", ";
	std::string line;

	for (int i = 0; i < size; i++) {
		size_t pos = 0;
		std::string token;
		getline(file, line);

		while ((pos = line.find(delimiter)) != std::string::npos) {
			token = line.substr(0, pos);
			int ind = stoi(token);
			result.push_back(ind);
			line.erase(0, pos + delimiter.length());
			//cout << token << "\n";
		}
		int ind = stoi(line);
		result.push_back(ind);
		//cout << line << "\n";
	}
	return result;
}

std::vector<double> fillArrayDouble(int size, std::ifstream& file) {
	vector<double> result;
	std::string delimiter = ", ";
	std::string line;

	for (int i = 0; i < size; i++) {
		size_t pos = 0;
		std::string token;
		getline(file, line);

		//line.erase(std::remove(line.begin(), line.end(), ','), line.end());

		while ((pos = line.find(delimiter)) != std::string::npos) {
			token = line.substr(0, pos);
			double ind = stod(token);
			result.push_back(ind);
			line.erase(0, pos + delimiter.length());
			//cout << token << "\n";
		}
		double ind = stod(line);
		result.push_back(ind);
		//cout << line << "\n";
	}
	return result;
}

void multLinMatrix4x4(double l[4], double m[4][4], double r[4]) {
	for (int i = 0; i < 4; i++) {
		r[i] = 0;
		for(int j = 0; j < 4; j++) {
			r[i] += m[j][i] * l[j];
		}
	}
}

void cross(double *a, double *b, double *res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double *a) {

	double l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

//Calcula todas as multiplicações --> U * M * P * M^T * V^T
void allCalcs(double controlPoints[3][4][4], double us[4], double vs[4], double point[3]) {
	double temp1[3][4];
	double temp2[3][4];
	double temp3[3][4];
	for (int i = 0; i < 3; i++) {
		multLinMatrix4x4(us, b, temp1[i]); // U * M
		multLinMatrix4x4(temp1[i], controlPoints[i], temp2[i]); // U * M * P
		multLinMatrix4x4(temp2[i], bt, temp3[i]); // U * M * P * M^T
		point[i] = 0;
		for (int j = 0; j < 4; ++j) {
			point[i] += temp3[i][j] * vs[j]; // V * M * P * M^T * V^T
		}
	}
}

void dUBezier(double controlPoints[3][4][4], double u, double v, double point[3]) {
	double u2 = 3 * u * u;
	double v2 = v * v; // v^2
	double v3 = v * v2; // v^3
	double us[4] = { 0, 1, 2* u, u2 }; // [3u^2 u 1 0]
	double vs[4] = { 1, v, v2, v3 }; // [v^3 v^2 v 1]^T
	allCalcs(controlPoints, us, vs, point);
}

void dVBezier(double controlPoints[3][4][4], double u, double v, double point[3]) {
	double u2 = u * u; // u^2
	double u3 = u * u2; // u^3
	double v2 = 3 * v * v; // v^2
	double us[4] = { 1, u, u2, u3 }; // [u^3 u^2 u 1]
	double vs[4] = { 0, 1, 2 * v, v2 }; // [3*v^2 v 1 0]^T
	allCalcs(controlPoints, us, vs, point);
}

void bezierNormal(double controlPoints[3][4][4], double u, double v, double normal[3]) {
	double dU[3];
	double dV[3];
	
	dUBezier(controlPoints, u, v, dU);
	dVBezier(controlPoints, u, v, dV);
	
	cross(dV, dU, normal);
	normalize(normal);
}

void bezierPoint(double controlPoints[3][4][4], double u, double v, double point[3]) {
	double u2 = u * u; // u^2
	double u3 = u * u2; // u^3
	double v2 = v * v; // v^2
	double v3 = v * v2; // v^3
	double us[4] = { 1, u, u2, u3}; // [u^3 u^2 u 1]
	double vs[4] = { 1, v, v2, v3 }; // [v^3 v^2 v 1]^T

	allCalcs(controlPoints, us, vs, point);
}

void parsePatches(const char* name, const char* cpFile, int tesselations) {
	FILE *fp = fopen(name, "w+");
	std::string line;
	std::ifstream file;
	file.open(cpFile);

	if (file.is_open()) {
		getline(file, line);
		int nPatches = stoi(line);
		vector<int> indexes = fillArrayInt(nPatches, file);

		getline(file, line);
		int nCPoints = stoi(line);
		 vector<double> points = fillArrayDouble(nCPoints, file);

		//Coisas
		fprintf(fp, "%d\n", nPatches * tesselations * tesselations * 6);
		for (int l = 0; l < nPatches; ++l) {
			int patchOffset = 16 * l;
			double controlPoints[3][4][4];
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					for (int k = 0; k < 3; ++k) {
						int indexOffset = patchOffset + (i * 4 + j);
						// Insere na matrix por coluna
						controlPoints[k][i][j] = points[indexes[indexOffset] * 3 + k];
					}
					//printf("%d %d (%f, %f, %f)\n", l, indexes[16 * l + (i * 4 + j)], controlPoints[0][i][j], controlPoints[1][i][j], controlPoints[2][i][j]);
				}
			}
			// Loop que divide as curvas de bezier
			float parts = 1.0f / tesselations;
			for (int ui = 0; ui < tesselations; ++ui) {
				double u = (double)ui / tesselations;
				for (int vi = 0; vi < tesselations; ++vi) {
					double v = (double)vi / tesselations;
					double point[3];
					double normal[3];

					bezierPoint(controlPoints, u, v, point);
					bezierNormal(controlPoints, u, v, normal);
					fprintf(fp, "%f %f %f %f %f %f %f %f\n", point[0], point[1], point[2], normal[0], normal[1], normal[2], 2.1, 1.5);
					bezierPoint(controlPoints, u, (v + parts), point);
					bezierNormal(controlPoints, u, (v + parts), normal);
					fprintf(fp, "%f %f %f %f %f %f %f %f\n", point[0], point[1], point[2], normal[0], normal[1], normal[2], 2.1, 1.5);
					bezierPoint(controlPoints, (u + parts), (v + parts), point);
					bezierNormal(controlPoints, (u + parts), (v + parts), normal);
					fprintf(fp, "%f %f %f %f %f %f %f %f\n", point[0], point[1], point[2], normal[0], normal[1], normal[2], 2.1, 1.5);

					bezierPoint(controlPoints, u, v, point);
					bezierNormal(controlPoints, u, v, normal);
					fprintf(fp, "%f %f %f %f %f %f %f %f\n", point[0], point[1], point[2], normal[0], normal[1], normal[2], 2.1, 1.5);
					bezierPoint(controlPoints, (u + parts), (v + parts), point);
					bezierNormal(controlPoints, (u + parts), (v + parts), normal);
					fprintf(fp, "%f %f %f %f %f %f %f %f\n", point[0], point[1], point[2], normal[0], normal[1], normal[2], 2.1, 1.5);
					bezierPoint(controlPoints, (u + parts), v, point);
					bezierNormal(controlPoints, (u + parts), v, normal);
					fprintf(fp, "%f %f %f %f %f %f %f %f\n", point[0], point[1], point[2], normal[0], normal[1], normal[2], 2.1, 1.5);
				}
			}
		}

		file.close();

	}
	else std::cout << "Unable to open bezier patches file\n";

	if (fp) fclose(fp);
}