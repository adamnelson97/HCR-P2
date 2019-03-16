# HCR-P2
Mines CSCI 473 Project 2

=== Part 0 ===
Before running any commands in Parts 1 or 2, go into /part1/ and delete
rad_d1, rad_d1.t, cust_d1, and cust_d1.t if they exist.

=== Part 1 ===
To run the representation:
1) cd part1
2) make
3) bash train.sh
4) bash test.sh
5) bash train-custom.sh
6) bash test-custom.sh

This should generate rad_d1, rad_d1.t, cust_d1, and cust_d1.t


Implementation Information:

I wrote my implementation in C++. I started by creating a class to store
each row of data from the text files. Next, these rows would be grouped
into frames. A Representation class tracks all of the rows and frames
and performs calculations on them, such as getting the distance from one
joint to the center of the model, or calculating the angles between joints.

Then, the Represenation creates histograms from the vectors of distances
and angles, normalizes those histograms, and then exports them to the
appropriate file.

I decided to use 5 bins for each histogram. Each group increases by 20
percentiles, which gives a good but not overly specific picture of the
data.


Custom Implementation:

For my custom implementation I decided to not calculate angles and instead
I added in the elbow and knee joints to the data set.


=== Part 2 ===
To apply the C-SVM model:
1) Copy rad_d1, rad_d1.t, cust_d1, and cust_d1.t from the part1 folder and
	past inside the part2/tools directory (the same directory containing easy.py)
2) cd part2/tools
3) make
4) ./conv rad_d1		// Creates rad_d2
5) ./conv rad_d1.t      // Creates rad_d2.t
6) ./conv cust_d1       // Creates cust_d2
7) ./conv cust_d1.t     // Creates cust_d2.t
8) python easy.py rad_d2 rad_d2.t      // Generates rad_d2.t.predict
9) python easy.py cust_d2 cust_d2.t    // Generates cust_d2.t.predict

Best C and G values for RAD Representation:
	C: 2    G: 0.03125

Best C and G values for Custom Representation:
	C: 2    G: 0.03125