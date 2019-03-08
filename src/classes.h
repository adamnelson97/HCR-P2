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

class Histogram {
public:
	Histogram();
	int first_pct;
	int second_pct;
	int third_pct;
	int fourth_pct;
	int fifth_pct;
	double bin1;
	double bin2;
	double bin3;
	double bin4;
	double bin5;
	int total;
};

class Representation {
public:
	Representation();
	Representation(ifstream& input, bool star);
	vector<Row> rows;
	vector<Frame> frames;
	vector<double> repr_dist1;
	vector<double> repr_dist2;
	vector<double> repr_dist3;
	vector<double> repr_dist4;
	vector<double> repr_dist5;
	vector<double> repr_angle1;
	vector<double> repr_angle2;
	vector<double> repr_angle3;
	vector<double> repr_angle4;
	vector<double> repr_angle5;
	vector<Histogram> hists;
	void makeFrames();
	void printFrames();
	void calculateDistances();
	void calculateStarAngles();
	void printStats();
	void makeHist();
};