
UNIVERSITY OF CALGARY

#####################################
# "NORMAL & ABNORMAL GAIT DATASET"  #
#####################################

*******************************************************************
THIS DATASET CREATED BY THE UNIVERSITY OF CALGARY BIOMETRIC TECHNOLGIES 
LABORATORY FOR RESEARCH/SCIENTIFIC PURPOSES ONLY.
IF YOU HAVE ANY COMMENTS OR IN CASE YOU ARE INTERESTED IN USING THIS 
DATASET PLEASE CONTACT  Dr. Svetlana Yanushkevich @ syanshk@ucalgary.ca
*******************************************************************

*************
REFERENCE:
*************
In order to use the UCalgary gait dataset you have to refer to the following
papers:

[1] P. Kozlow, N. Abid, and S. Yanushkevich, 
“Utilizing gait traits to improve e-border watchlist performance,” 
IEEE Symposium Series on Computational Intelligence (SSCI),
pp. 1–8, 2017.

*************
INFORMATION:
*************
This dataset was created using a custom C++ application in combination
with the kinect v2 camera.

*************
PURPOSE:
*************
This dataset was created with the intention of investigating soft biometrics
cues that exist within an individuals gait. As well as performing gait recognition
for the individual in question.

*************
ORGANIZATION:
*************
This dataset consists gait sequences from 28 individuals. Each person was recorded
6 times (2 normal + 4 abnormal), totaling 28*6 = 168 sequences OR 28*2 + 28*4 = 56 normal and 112 abnormal sequences.

PLEASE IGNORE Run 7, for subjects 1-5. This was a purely experimental portion.

The database is structured as such:
SUBJECT_#_SEQ_#_GEN_M/F_TYPE.csv

where:
SUBJECT_# == Subject number
SEQ_# == The sequence/run for the subject
GEN_M/F == Gender of the subject (male or female)
TYPE == What the file consists of, as well as what type of gait was tested (N == Normal, AR == Abnormal Right, AL == Abnormal Left).

Each subject performed 6 runs organized as such:
Runs 1-2 == normal gait
Runs 3-4 == abnormal gait (right leg abnormality)
Runs 5-6 == abnormal gait (left leg abnormality)

*************
FILE DETAILS:
*************
ALL files are in comma seperated value (csv) format, default program to open and view 
was Microsoft Excel 2013.

FILE: GaitCycle: In this file the gait cycle is represented with a sequence of frames represented by
the rows, where frame 1 starts at row 2. Therefore, frame N = row N+1. In this file, the values represent
the distance that exists between an individuals ankles. This distance is used with the JRD method to determine
the gait cycle for the individual.

FILE: GeometricPosition: The geometric positions of the individuals joints is represented in this file. Each joint used
is labeled as such:
JOINT X, Y, Z. Where, joint represents which joint is being modeled as well as its repsective X coordinate. The following
two collumns represent this joint's Y and Z coordinates respective to the camera in meters (m). These coordinates are used
in conjunction with Matlab to find various features to indentify an individual's gait features & gait type.

FILE: Height (Currently EXPERIMENTAL): This file details the height parameters related to an individuals geometric coordinates. 
The collumns are labeled as so:
1-3 == Individual's Height in meters, centimeters, feet.
4-6 == Individual's M2???
8-9 == Individual's arm and leg length in centimeters

FILE: JointAngles: This file outputs some calculated angles for selected joints.
Angles are calculated per frame and in DEGREES. The calculations are output for the following joints:
Left/Right Knees, Left/Right Ankles, Left/Right Ankle Delta???

FILE: SmoothedJointAngles: This is a continuation of the previous file, where the angles are smoothed using a moving average filter.
The added parameters are the average angles for the ankles as well as the repective phases that the ankle is in. Where:

HS == Heel Strike (phase 1), this is where your heel makes contact with the ground
FF == Foot Flat (phase 2), this is where your foot makes maximum contact with the ground
TO == Toe Off (phase 3), this is where the front of the foot only makes contact with the ground

These parameters are used to test the validity of the gait cycle.




