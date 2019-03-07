#include "classes.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
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
			repr_distances.push_back(dist);
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
		repr_angles.push_back(angle);

		// Left Hand to Left Foot
		r1 = frames[i].rows[3];
		r2 = frames[i].rows[5];
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[3].angle_to_right = angle;
		repr_angles.push_back(angle);

		// Left Foot to Right Foot
		r1 = frames[i].rows[5];
		r2 = frames[i].rows[4];
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[5].angle_to_right = angle;
		repr_angles.push_back(angle);

		// Right Foot to Right Hand
		r1 = frames[i].rows[4];
		r2 = frames[i].rows[2];
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[4].angle_to_right = angle;
		repr_angles.push_back(angle);

		// Right Hand to Head
		r1 = frames[i].rows[2];
		r2 = frames[i].rows[1];
		angle = calcAngle(r1.x_pos, r1.y_pos, r1.z_pos, r2.x_pos, r2.y_pos, r2.z_pos);
		frames[i].rows[2].angle_to_right = angle;
		repr_angles.push_back(angle);
	}
	printFrames(); // For debugging
}

void Representation::printAnglesDistances() {
	for (int i = 0; i < repr_distances.size(); i++) {
		cout << "Distance: " << repr_distances[i] << endl;
	}
	for (int i = 0; i < repr_angles.size(); i++) {
		cout << "Angle: " << repr_angles[i] << endl;
	}
}