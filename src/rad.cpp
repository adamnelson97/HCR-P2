#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "functions.h"
#include "classes.h"
#include <cstring>
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

	Representation repr(fileIn, true);
	repr.makeFrames();
	repr.calculateDistances();
	repr.calculateStarAngles();
	repr.printStats();
	repr.makeHist();

	string outFile;
	if (strcmp(argv[2], "train") == 0) outFile = "rad_d1";
	if (strcmp(argv[2], "test") == 0) outFile = "rad_d1.t";

	ofstream fileOut;
	fileOut.open(outFile, ios::app);
	for (int i = 0; i < repr.hists.size(); i++) {
		fileOut << repr.hists[i].bin1 << " " 
		<< repr.hists[i].bin2 << " " 
		<< repr.hists[i].bin3 << " " 
		<< repr.hists[i].bin4 << " " 
		<< repr.hists[i].bin5; 
	}
	fileOut << endl;

	return 0;
}