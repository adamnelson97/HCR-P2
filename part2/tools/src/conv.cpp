#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
using namespace std;

struct Pair {
	int index;
	string value;
};

struct Row {
	int label;
	vector<Pair> pairs;
};

int main(int argc, char*argv[]) {

	// Create input file stream
	string fileName = argv[1];
	cout << "Input File: " << fileName << endl;

	ifstream fileIn(fileName);
	if (fileIn.fail()) {
		cerr << "File " << fileName << " not found." << endl;
		return -1;
	}

	/*
	For each row, extract the row from the input file.
	Add the row to a vector, with each number as different index
	Index the number.
	Output it to a different file.
	*/
	vector<Row> rows;
	while (!fileIn.eof()) {
		string row_data, word;
		int index = 0;

		getline(fileIn, row_data); // Extracts the line from rad_d1, cust_d1, etc.
		istringstream iss(row_data);

		Row currRow;
		iss >> currRow.label;
		while(iss >> word) {
        	index++;
        	Pair pair;
        	pair.index = index;
        	pair.value = word;
        	currRow.pairs.push_back(pair);
    	}

    	rows.push_back(currRow);
    }


	string outFile;
	if (strcmp(argv[1], "rad_d1") == 0) outFile = "rad_d2";
	if (strcmp(argv[1], "rad_d1.t") == 0) outFile = "rad_d2.t";
	if (strcmp(argv[1], "cust_d1") == 0) outFile = "cust_d2";
	if (strcmp(argv[1], "cust_d1.t") == 0) outFile = "cust_d2.t";
	
    ofstream fileOut;
	fileOut.open(outFile, ios::app);
	for (int i = 0; i < rows.size(); i++) {
		fileOut << rows[i].label;
		for (int j = 0; j < rows[i].pairs.size(); j++) {
			fileOut << " " << rows[i].pairs[j].index << ":" << rows[i].pairs[j].value;
		}
		fileOut << endl;
	}

	cout << fileName << " was processed successfully." << endl;
	
	return 0;
}