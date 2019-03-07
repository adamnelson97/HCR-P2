#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Row {
public:
	// Attributes
	int frame_id;
	int joint_id;
	double x_pos;
	double y_pos;
	double z_pos;
	double dist_to_center;
	double angle_to_right;

	// Methods
	void printRow();
};

class Frame {
public:
	int frame_id;
	vector<Row> rows;
};

class Representation {
public:
	Representation();
	Representation(ifstream& input, bool star);
	vector<Row> rows;
	vector<Frame> frames;
	vector<double> repr_distances;
	vector<double> repr_angles;
	void makeFrames();
	void printFrames();
	void calculateDistances();
	void calculateStarAngles();
	void printAnglesDistances();
};