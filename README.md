# HCR-P2
Mines CSCI 473 Project 2

To run the representation:
1) make
2) bash train.sh
3) bash test.sh

This should generate rad_d1 and rad_d1.t


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
