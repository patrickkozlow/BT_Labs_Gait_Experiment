#pragma once

#include <string>

using namespace std;

class JointAngles
{

private:
	JointAngles();

public:
	
	JointAngles(int lk, int rk, int la, int ra);
	
	~JointAngles();

	int leftKnee = 0;
	int rightKnee = 0;
	int leftAnkle = 0;
	int leftAnkleDelta = 0;
	int rightAnkle = 0 ;
	int rightAnkleDelta = 0;
	int leftAnkleMA = 0;
	int rightAnkleMA = 0;
	string leftAnkleGaitCycle;
	string rightAnkleGaitCycle;

};

