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

	string class_num = "";
	char temp = fileName[15];
	if (temp != '0') class_num += fileName[15];
	class_num += fileName[16];

	bool star = true;
	if (argc == 4) star = false;
	Representation repr(fileIn, star);
	repr.makeFrames();
	if (star) {
		repr.calculateDistances();
		repr.calculateStarAngles();
		repr.printStats();
		repr.makeHist();
	}
	else {
		repr.calculateDistancesCust();
		repr.makeHistCust();
	}
	

	string outFile;
	if (star) {
		if (strcmp(argv[2], "train") == 0) outFile = "rad_d1";
		if (strcmp(argv[2], "test") == 0) outFile = "rad_d1.t";
	}
	else {
		if (strcmp(argv[2], "train") == 0) outFile = "cust_d1";
		if (strcmp(argv[2], "test") == 0) outFile = "cust_d1.t";
	}

	// string class_num = "";
	// class_num += fileName[1];
	// class_num += fileName[2];

	ofstream fileOut;
	fileOut.open(outFile, ios::app);
	fileOut << class_num << " ";
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