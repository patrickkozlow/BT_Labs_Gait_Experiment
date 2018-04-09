        ###################################################### 
        #      University of Patras Computer Vision Group    #
        #                 UPCV Group
        #          http://www.upcv.upatras.gr/               #
        ######################################################

                   "UPCV GAIT DATASET"

*******************************************************************
THIS DATASET CREATED BY UNIVERSITY OF PATRAS COMPUTER VISION GROUP 
        FOR RESEARCH/SCIENTIFIC PURPOSES ONLY.
IF YOU HAVE ANY COMMENTS OR IN CASE YOU ARE INTERESTED IN USING THIS 
DATASET PLEASE CONTACT  Mr. Dimitrios Kastaniotis (PhD candidate) @
[dkastaniotis at upatras.gr]
 
*******************************************************************

Reference

In order to use the UPCV action dataset you have to refer to the following
papers:

[1] Dimitris Kastaniotis, Ilias Theodorakopoulos,
Christos Theoharatos, George Economou, Spiros Fotopoulos,
"A framework for gait-based recognition using Kinect", Pattern Recognition Letters, 
Available online 9 July 2015, ISSN 0167-8655, http://dx.doi.org/10.1016/j.patrec.2015.06.020.

[2] D. Kastaniotis, I. Theodorakopoulos, G. Economou and S. Fotopoulos,
Gait-based Gender Recognition using Pose Information for Real Time Applications", Digital Signal Processing (DSP),
 2013 18th International Conference on , vol., no., pp.1,6, 1-3 July 2013 

*******************************************************************
 
 
Information:


This dataset was created using a custom C++ application and the Microsoft
1.5 SDK.

Details:
This database was created in order to be used as a benchmark for evaluating
different gait recognition techniques targeting on gait based identification
and or gender recognition. 

Actions:
In this dataset contains gait sequences from 30 persons.
Each person was recorded five times thus totally there are
30*5 = 150 gait sequences.

The database is totally balances with respect to the gender and thus the first 
15 persons are males and the last 15 are females.
 
*******************
Data (txt format):
*******************
Skeletal data for every sequence are saved in separate .txt files
with filenames seq{#}.txt in the person{*} folder (where # denotes the sequence
number and * the person).

In every file, a gait sequence is represented with a sequence of
frames in the form of a 20 joints skeletal model.
Thus every 20 lines a model of the human
body consisted by 20 three dimensional joints (x,y,z) is provided.

.txt Files are located in /GaitSequences folder.


*******************
Numbering the Points
*******************
1. Head 		6. Wrist Left 		11. Spine 		16. Ankle Left
2. Shoulder Center 	7. Hand Left 		12. Hip Center 		17. Foot Left
3. Shoulder Right 	8. Elbow Right 		13. Hip Right 		18. Knee Right
4. Shoulder Left 	9. Wrist Right 		14. Hip Left 		19. Ankle Right
5. Elbow Left 		10. Hand Right 		15. Knee Left 		20. Foot Right


*******************
Data (Matlab format):
*******************
We also provide a Matlab cell array with name upcv.
The user can acces users gait sequences by accessing upcv structure 
as follows:
upcv{i,j} where i can take values from 1 to 30 and j values from 1 to 5.

The .mat file is located in /MatlabFormat folder.


*******************
Joint Connectivity
*******************

Every line in seqx.txt files (where x varies from 1 to 5 indicating the number of sequence)
corresponds to a joint represented as a vector in three dimensions.
A skeleton pose is represented by 20 joints.
Thus every 20 lines we have a new pose- frame.
The following table shows the connectivity between joints (for every pose)

     1     2
     2     4
     4     5
     5     6
     6     7
     2     3
     3     8
     8     9
     9    10
     2    11
    11    12
    12    14
    14    15
    15    16
    16    17
    12    13
    13    18
    18    19
    19    20


**********************
Complementary Material
**********************

 In /MatlabFormat folder we have a Matlab script with name
preview_data.m that allows the user to visualize gait sequences.

connections.m is a script that creates a variable used by preview_data.m
in order to draw the connections between joints.