#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "functions.h"
using namespace std;
int main(int argc, char *argv[]) {
	
	// Create input file stream
	string fileName;
	for (int i = 0; i < argc; i++) {
		if (isTextFile(argv[i], ".txt")) { fileName = argv[i]; }
	}
	cout << "Input File: " << fileName << endl;

	ifstream fileIn(fileName);
	if (fileIn.fail()) {
		cerr << "File " << fileName << " not found." << endl;
		return -1;
	}

	return 0;
}