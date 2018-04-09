#pragma once

#include <vector>
#include "JointAngles.h"

using namespace std;


class SmoothData
{
private:
	SmoothData();
	vector<JointAngles> *data;
	unsigned int maximumChange = 0;
	void SmoothLeftAnkle(unsigned int i);
	void SmoothRightAnkle(unsigned int i);
	unsigned int leftKneeMax = 0;
	unsigned int rightKneeMax = 0;
	unsigned int leftKneeMin = 0;
	unsigned int rightKneeMin = 0;

	void GetLeftKneeRange();
	void GetRightKneeRange();


public:
	SmoothData(vector<JointAngles> *, unsigned int);
	void Smooth();
	void CalculateDeltas();
	void CalculateAverages();
	void GaitCycle();
	~SmoothData();
};

