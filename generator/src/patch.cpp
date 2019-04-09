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

std::vector<int> fillArray(int size, std::ifstream& file) {
	vector<int> result;
	std::string delimiter = ", ";
	std::string line;

	for (int i = 0; i < size; i++) {
		size_t pos = 0;
		std::string token;
		getline(file, line);

		//line.erase(std::remove(line.begin(), line.end(), ','), line.end());

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

void parsePatches(const char* name, const char* cpFile, int tesselations) {
	FILE *fp;
	fopen_s(&fp, name, "w");

	std::string line;
	std::ifstream file;
	file.open(cpFile);
	
	if (file.is_open()) {

		getline(file, line);
		int nPatches = stoi(line);
		vector<int> indexes = fillArray(nPatches,file);

		getline(file, line);
		int nCPoints = stoi(line);
		vector<int> points = fillArray(nCPoints, file);
		
		//Coisas
		
		file.close();

	}
	else std::cout << "Unable to open bezier patches file";
	file.close();

	fclose(fp);
}
