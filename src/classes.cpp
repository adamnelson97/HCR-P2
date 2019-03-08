#include "classes.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

void Row::printRow() {
	cout << "Frame " << this->frame_id
	<< ", Joint " << this->joint_id
	<< "\tX: " << this->x_pos
	<< "  \tY: " << this->y_pos
	<< "  \tZ: " << this->z_pos 
	<< "  \tDist. to Center: " << this->dist_to_center 
	<< "  \tAngle: " << this->angle_to_right << endl;
}

Representation::Representation(ifstream& input, bool star) {
	if (star) {
		/*
		Joint 1: Pelvis/Center
		Joint 4: Head
		Joint 8: Right Hand
		Joint 12: Left Hand
		Joint 16: Right Foot
		Joint 20: Left Foot
		*/
		while (!input.eof()) {
			Row row; // Creates Row object
			input >> row.frame_id;
			input >> row.joint_id;
			input >> row.x_pos;
			input >> row.y_pos;
			input >> row.z_pos;
			row.dist_to_center = 0;
			row.angle_to_right = 0;

			if (row.joint_id == 1 || row.joint_id == 4 || row.joint_id == 8 ||
				row.joint_id == 12 || row.joint_id == 16 || row.joint_id == 20) {
				this->rows.push_back(row); // Add to the rep's row vector
				row.printRow(); // Prints the row to confirm it was read properly
			}
		}
		this->rows.pop_back(); // Removes duplicate row
		cout << "All frames read into Representation." << endl;
	}
}


void Representation::printFrames() {
	for (int i = 0; i < frames.size(); i++) {
		cout << "Frame " << frames[i].frame_id << endl;
		for (int j = 0; j < frames[i].rows.size(); j++) {
				frames[i].rows[j].printRow();
		}
	}
}

void Representation::makeFrames() {
	int num_frames = rows.back().frame_id; // Gets the maximum number of frames
	for (int i = 0; i < num_frames; i++) {
		Frame frame;
		frame.frame_id = i+1;
		for (int j = 0; j < rows.size(); j++) {
			if (rows[j].frame_id == i+1) frame.rows.push_back(rows[j]);
		}
		frames.push_back(frame);
	}
	printFrames(); // For debugging
}

void Representation::calculateDistances() {
	// For every frame
	for (int i = 0; i < frames.size(); i++) {
		Row center = frames[i].rows[0];
		// For every row
		for (int j = 0; j < frames[i].rows.size(); j++) {
			Row curr = frames[i].rows[j];
			double dist = sqrt( pow(curr.x_pos - center.x_pos, 2.0)
				+ pow(curr.y_pos - center.y_pos, 2.0)
				+ pow(curr.z_pos - center.z_pos, 2.0));
			frames[i].rows[j].dist_to_center = dist;
			switch(frames[i].rows[j].joint_id) {
				case 4: repr_dist1.push_back(dist);
				break;
				case 8: repr_dist2.push_back(dist);
				break;
				case 12: repr_dist3.push_back(dist);
				break;
				case 16: repr_dist4.push_back(dist);
				break;
				case 20: repr_dist5.push_back(dist);
				break;
			}
		}
	}
	printFrames(); // For debugging
}

double calcAngle(double x1, double y1, double z1, double x2, double y2, double z2) {
	
	double dot_product = x1*x2 + y1*y2 + z1*z2;
	double magn1 = sqrt( pow(x1, 2.0) + pow(y1, 2.0) + pow(z1, 2.0));
	double magn2 = sqrt( pow(x2, 2.0) + pow(y2, 2.0) + pow(z2, 2.0));
	double val = dot_product/(magn1*magn2);
	return acos(val);
}

void Representation::calculateStarAngles() {
	/*
	frames[i].rows[0] = Joint 1: Pelvis/Center
	frames[i].rows[1] = Joint 4: Head
	frames[i].rows[2] = Joint 8: Right Hand
	frames[i].rows[3] = Joint 12: Left Hand
	frames[i].rows[4] = Joint 16: Right Foot
	frames[i].rows[5] = Joint 20: Left Foot
	*/

	/* 
	Note: angle_to_right refers to looking at
	the 'Da Vinci' diagram. For example, the
	angle_to_right for the Head joint is the 
	angle between the head and left hand,
	since the left hand is to the 'right'
	when you look at the diagram.
	
	Angle calculated from Law of Cosines:
		cos(theta) = a (dot) b / magnitude(a)*magnitude(b)

	magnitude(x) is the dist_to_center for joint x
	a (dot) b is the normal dot product of a and b,
	and then the distance from THAT point to the location of the Center joint
	*/

	Row r1, r2;
	double angle;
	// For every frame
	for (int i = 0; i < frames.size(); i++) {

		// Head to Left Hand
		r1 = frames[i].rows[1];
		r2 = frames[i].rows[3];
		r1.printRow();
		r2.printRow();
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[1].angle_to_right = angle;
		repr_angle1.push_back(angle);

		// Left Hand to Left Foot
		r1 = frames[i].rows[3];
		r2 = frames[i].rows[5];
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[3].angle_to_right = angle;
		repr_angle2.push_back(angle);

		// Left Foot to Right Foot
		r1 = frames[i].rows[5];
		r2 = frames[i].rows[4];
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[5].angle_to_right = angle;
		repr_angle3.push_back(angle);

		// Right Foot to Right Hand
		r1 = frames[i].rows[4];
		r2 = frames[i].rows[2];
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[4].angle_to_right = angle;
		repr_angle4.push_back(angle);

		// Right Hand to Head
		r1 = frames[i].rows[2];
		r2 = frames[i].rows[1];
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[2].angle_to_right = angle;
		repr_angle5.push_back(angle);
	}
	printFrames(); // For debugging
}

bool mySortFunction (double i, double j) { return (i<j); }

void Representation::printStats() {
	sort(repr_angle1.begin(), repr_angle1.end(), mySortFunction);
	sort(repr_angle2.begin(), repr_angle2.end(), mySortFunction);
	sort(repr_angle3.begin(), repr_angle3.end(), mySortFunction);
	sort(repr_angle4.begin(), repr_angle4.end(), mySortFunction);
	sort(repr_angle5.begin(), repr_angle5.end(), mySortFunction);

	sort(repr_dist1.begin(), repr_dist1.end(), mySortFunction);
	sort(repr_dist2.begin(), repr_dist2.end(), mySortFunction);
	sort(repr_dist3.begin(), repr_dist3.end(), mySortFunction);
	sort(repr_dist4.begin(), repr_dist4.end(), mySortFunction);
	sort(repr_dist5.begin(), repr_dist5.end(), mySortFunction);

	// Angles
	double sum = 0;
	for (int i = 0; i < repr_angle1.size(); i++) {
		sum += repr_angle1[i];
	}
	double min, max, range;
	min = repr_angle1[0];
	max = repr_angle1[repr_angle1.size()-1];
	range = (max-min)/5;

	cout << "Angle Vec 1:\tMin: " << min << "\tAvg: " << sum/repr_angle1.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;

	sum = 0;
	for (int i = 0; i < repr_angle2.size(); i++) {
		sum += repr_angle2[i];
	}
	min = repr_angle2[0];
	max = repr_angle2[repr_angle2.size()-1];
	range = (max-min)/5;

	cout << "Angle Vec 2:\tMin: " << min << "\tAvg: " << sum/repr_angle2.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;

	sum = 0;
	for (int i = 0; i < repr_angle3.size(); i++) {
		sum += repr_angle3[i];
	}
	min = repr_angle3[0];
	max = repr_angle3[repr_angle3.size()-1];
	range = (max-min)/5;

	cout << "Angle Vec 3:\tMin: " << min << "\tAvg: " << sum/repr_angle3.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;

	sum = 0;
	for (int i = 0; i < repr_angle4.size(); i++) {
		sum += repr_angle4[i];
	}
	min = repr_angle4[0];
	max = repr_angle4[repr_angle4.size()-1];
	range = (max-min)/5;

	cout << "Angle Vec 4:\tMin: " << min << "\tAvg: " << sum/repr_angle4.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;

	sum = 0;
	for (int i = 0; i < repr_angle5.size(); i++) {
		sum += repr_angle5[i];
	}
	min = repr_angle5[0];
	max = repr_angle5[repr_angle5.size()-1];
	range = (max-min)/5;

	cout << "Angle Vec 5:\tMin: " << min << "\tAvg: " << sum/repr_angle5.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;
	
	cout << endl;

	// Distances
	sum = 0;
	for (int i = 0; i < repr_dist1.size(); i++) {
		sum += repr_dist1[i];
	}
	min = repr_dist1[0];
	max = repr_dist1[repr_dist1.size()-1];
	range = (max-min)/5;

	cout << "Dist Vec 1:\tMin: " << min << "\tAvg: " << sum/repr_dist1.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;

	sum = 0;
	for (int i = 0; i < repr_dist2.size(); i++) {
		sum += repr_dist2[i];
	}
	min = repr_dist2[0];
	max = repr_dist2[repr_dist2.size()-1];
	range = (max-min)/5;

	cout << "Dist Vec 2:\tMin: " << min << "\tAvg: " << sum/repr_dist2.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;

	sum = 0;
	for (int i = 0; i < repr_dist3.size(); i++) {
		sum += repr_dist3[i];
	}
	min = repr_dist3[0];
	max = repr_dist3[repr_dist3.size()-1];
	range = (max-min)/5;

	cout << "Dist Vec 3:\tMin: " << min << "\tAvg: " << sum/repr_dist3.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;

	sum = 0;
	for (int i = 0; i < repr_dist4.size(); i++) {
		sum += repr_dist4[i];
	}
	min = repr_dist4[0];
	max = repr_dist4[repr_dist4.size()-1];
	range = (max-min)/5;

	cout << "Dist Vec 4:\tMin: " << min << "\tAvg: " << sum/repr_dist4.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;

	sum = 0;
	for (int i = 0; i < repr_dist5.size(); i++) {
		sum += repr_dist5[i];
	}
	min = repr_dist5[0];
	max = repr_dist5[repr_dist5.size()-1];
	range = (max-min)/5;

	cout << "Dist Vec 5:\tMin: " << min << "\tAvg: " << sum/repr_dist5.size() 
	<< "\tMax: " << max << "\tRange/5: " << range << endl;
}

Histogram::Histogram() {
	this->first_pct = 0;
	this->second_pct = 0;
	this->third_pct = 0;
	this->fourth_pct = 0;
	this->fifth_pct = 0;
	this->bin1 = 0;
	this->bin2 = 0;
	this->bin3 = 0;
	this->bin4 = 0;
	this->bin5 = 0;
	this->total = 0;
}

Histogram histMaker(vector<double> vec) {
	double max = vec[vec.size()-1];
	Histogram hist;
	for (int i = 0; i < vec.size(); i++) {
		if (0 < vec[i] && vec[i] < max*0.2) hist.first_pct++;
		else if (max*0.2 < vec[i] && vec[i] < max*0.4) hist.second_pct++;
		else if (max*0.4 < vec[i] && vec[i] < max*0.6) hist.third_pct++;
		else if (max*0.6 < vec[i] && vec[i] < max*0.8) hist.fourth_pct++;
		else if (max*0.8 < vec[i] && vec[i] < max) hist.fifth_pct++;
	}
	hist.total = vec.size();
	hist.bin1 = (double) hist.first_pct/hist.total;
	hist.bin2 = (double) hist.second_pct/hist.total;
	hist.bin3 = (double) hist.third_pct/hist.total;
	hist.bin4 = (double) hist.fourth_pct/hist.total;
	hist.bin5 = (double) hist.fifth_pct/hist.total;
	return hist;
}

void Representation::makeHist() {
	// Sort all of the vectors
	sort(repr_angle1.begin(), repr_angle1.end(), mySortFunction);
	sort(repr_angle2.begin(), repr_angle2.end(), mySortFunction);
	sort(repr_angle3.begin(), repr_angle3.end(), mySortFunction);
	sort(repr_angle4.begin(), repr_angle4.end(), mySortFunction);
	sort(repr_angle5.begin(), repr_angle5.end(), mySortFunction);

	sort(repr_dist1.begin(), repr_dist1.end(), mySortFunction);
	sort(repr_dist2.begin(), repr_dist2.end(), mySortFunction);
	sort(repr_dist3.begin(), repr_dist3.end(), mySortFunction);
	sort(repr_dist4.begin(), repr_dist4.end(), mySortFunction);
	sort(repr_dist5.begin(), repr_dist5.end(), mySortFunction);

	Histogram temp;
	temp = histMaker(repr_angle1);
	hists.push_back(temp);

	temp = histMaker(repr_angle2);
	hists.push_back(temp);

	temp = histMaker(repr_angle3);
	hists.push_back(temp);

	temp = histMaker(repr_angle4);
	hists.push_back(temp);

	temp = histMaker(repr_angle5);
	hists.push_back(temp);

	temp = histMaker(repr_dist1);
	hists.push_back(temp);

	temp = histMaker(repr_dist2);
	hists.push_back(temp);

	temp = histMaker(repr_dist3);
	hists.push_back(temp);

	temp = histMaker(repr_dist4);
	hists.push_back(temp);

	temp = histMaker(repr_dist5);
	hists.push_back(temp);
}