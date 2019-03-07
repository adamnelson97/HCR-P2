#include "classes.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

void Row::printRow() {
	cout << "Frame " << this->frame_id
	<< ", Joint " << this->joint_id
	<< "\tX: " << this->x_pos
	<< "  \tY: " << this->y_pos
	<< "  \tZ: " << this->z_pos << endl;
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

void Representation::makeFrames() {
	int num_frames = rows.back().frame_id;
	for (int i = 0; i < num_frames; i++) {
		Frame frame;
		frame.frame_id = i+1;
		for (int j = 0; j < rows.size(); j++) {
			if (rows[j].frame_id == i+1) frame.rows.push_back(rows[j]);
		}
		frames.push_back(frame);

		// Confirm the frame was created correctly
		if (frame.rows.size() != 0) {
			cout << "Frame " << frame.frame_id << endl;
			for (int j = 0; j < frame.rows.size(); j++) {
				frame.rows[j].printRow();
			}
		}
	}
}

void Representation::calculateDistances() {

}