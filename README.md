# HCR-P2
Mines CSCI 473 Project 2

To run the representation:
1) make
2) bash train.sh
3) bash test.sh
4) bash train-custom.sh
5) bash test-custom.sh

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
