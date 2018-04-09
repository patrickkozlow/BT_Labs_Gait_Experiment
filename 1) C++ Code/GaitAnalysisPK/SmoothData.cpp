#include "SmoothData.h"
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

SmoothData::SmoothData(vector<JointAngles> *data, unsigned int maximumChange)
{
	this->data = data;
	this->maximumChange = maximumChange;

	assert(maximumChange > 0);
	assert(data != NULL);
	assert((*data).size() > 0);
}

void SmoothData::CalculateDeltas()
{
	for (unsigned int i = 0; i < this->data->size(); i++)
	{
		if (i > 0)
		{
			(*data)[i].leftAnkleDelta = (*data)[i].leftAnkle - (*data)[i - 1].leftAnkle;
			(*data)[i].rightAnkleDelta = (*data)[i].rightAnkle - (*data)[i - 1].rightAnkle;
		}
	}
}

// this function does the moving average to smooth out the bad data 
void SmoothData::Smooth()
{
	for (unsigned int j = 0; j < 2; j++)
	{
		//std::cout << "Running smooth method iteration: " << j << endl;

		for (unsigned int i = 0; i < data->size(); i++)
		{
			if (abs((*data)[i].leftAnkleDelta) > maximumChange)
			{
				SmoothLeftAnkle(i);
			}

			if (abs((*data)[i].rightAnkleDelta) > maximumChange)
			{
				SmoothRightAnkle(i);
			}

		}

		CalculateDeltas();

	}

	GetLeftKneeRange();
	GetRightKneeRange();
	CalculateAverages();
	GaitCycle();
}

void SmoothData::SmoothLeftAnkle(unsigned int i)
{
	double adjustFactor = .75;

	if (i < data->size() - 1)
	{
		//int sum = (*data)[i - 1].leftAnkle + (*data)[i + 1].leftAnkle;
		//sum /= 2;
		//(*data)[i].leftAnkle = sum;
		(*data)[i].leftAnkle = (*data)[i].leftAnkle - ((*data)[i].leftAnkleDelta * adjustFactor);
		(*data)[i].leftAnkleDelta = (*data)[i].leftAnkle - (*data)[i - 1].leftAnkle;
		(*data)[i + 1].leftAnkleDelta = (*data)[i + 1].leftAnkle - (*data)[i].leftAnkle;
	}
}

void SmoothData::SmoothRightAnkle(unsigned int i)
{
	double adjustFactor = .75;

	if (i < data->size() - 1)
	{
		(*data)[i].rightAnkle = (*data)[i].rightAnkle - ((*data)[i].rightAnkleDelta * adjustFactor);
		(*data)[i].rightAnkleDelta = (*data)[i].rightAnkle - (*data)[i - 1].rightAnkle;
		(*data)[i + 1].rightAnkleDelta = (*data)[i + 1].rightAnkle - (*data)[i].rightAnkle;
	}
}

// Moving averages of the data to stabalize it, due to the sensor
// sending erratic data points.
void SmoothData::CalculateAverages()
{
	unsigned int average, sum, count = 0;
	unsigned int movingAvgCount = 4;
	// left ankle
	for (unsigned int i = 0; i < data->size(); i += movingAvgCount)
	{
		average = 0, sum = 0, count = 0;
		
		for (unsigned int j = i; j < i + movingAvgCount; j++) {

			if (j < (*data).size() - 1)
			{
				sum += (*data)[j].leftAnkle;
				count++;
			}
		}

		average = sum / count;

		for (unsigned int j = i; j < i + movingAvgCount; j++)
		{
			if (j < (*data).size())
			{
				(*data)[j].leftAnkleMA = average;
			}
		}

		// finished block of five frames
	}

	// right ankle
	for (unsigned int i = 0; i < data->size(); i += movingAvgCount)
	{
		average = 0, sum = 0, count = 0;

		for (unsigned int j = i; j < i + movingAvgCount; j++) {
			if (j < (*data).size() - 1)
			{
				sum += (*data)[j].rightAnkle;
				count++;
			}
		}

		average = sum / count;

		for (unsigned int j = i; j < i + movingAvgCount; j++)
		{
			if (j < (*data).size())
			{
				(*data)[j].rightAnkleMA = average;
			}
		}
	}




}

void SmoothData::GaitCycle()
{
	unsigned int minLeft, maxLeft, avgLeft, sumLeft;
	unsigned int minRight, maxRight, avgRight, sumRight;

	// first get the min, max and avg of the data set to determine how we will
	// find the gait cycle points.

	minLeft = 400, maxLeft = 0, avgLeft = 0, sumLeft = 0;
	minRight = 400, maxRight = 0, avgRight = 0, sumRight = 0;

	// this loop calculates the sum, avg, min and max of the vector
	for (unsigned int i = 0; i < (*data).size(); i++)
	{
		int angleLeft = (*data)[i].leftAnkleMA;
		sumLeft += angleLeft;
		if (angleLeft < minLeft)  minLeft = angleLeft;
		if (angleLeft > maxLeft)  maxLeft = angleLeft;

		int angleRight = (*data)[i].rightAnkleMA;
		sumRight += angleRight;
		if (angleRight < minRight)  minRight = angleRight;
		if (angleRight > maxRight)  maxRight = angleRight;

	}


	avgLeft = sumLeft / (*data).size();
	avgRight = sumRight / (*data).size();

	/*
	cout << endl;
	cout << "Maximun: " << max << endl;
	cout << "Minimum: " << min << endl;
	cout << "Average: " << avg << endl;
	cout << "Sum " << sum << endl;
	cout << endl;
	*/
	// this loop tries to find the heel strike and the toe off by seeing if angles
	// are within reasonable ranges of the min and max.  min value implies a to-off,
	// while a max value implies a heel strike.
	for (unsigned int i = 0; i < (*data).size(); i++)
	{
		// left side
		unsigned int angleLeft = (*data)[i].leftAnkleMA;
		unsigned int kneeAngleLeft = (*data)[i].leftKnee;

		// right side
		unsigned int angleRight = (*data)[i].rightAnkleMA;
		unsigned int kneeAngleRight = (*data)[i].rightKnee;

		//if ((angle / max) >= .95) {
		if (abs(int(maxLeft - angleLeft)) <= 10) {
			cout << "Left heel strike at frame: " << i << " value " << angleLeft << endl;
			(*data)[i].leftAnkleGaitCycle = "HS";
		}

		if (abs(int(maxRight - angleRight)) <= 10) {
			cout << "Right heel strike at frame: " << i << " value " << angleRight << endl;
			(*data)[i].rightAnkleGaitCycle = "HS";
		}

		//if ((min / angle) >= .95) {
		//if (abs(int(min - angle)) <= 10 || abs(int(kneeAngle-leftKneeMin <= 10))) {
		if (abs(int(minLeft - angleLeft)) <= 10){
			// cout << "Toe off at frame: " << i << " value " << angle << endl;
			(*data)[i].leftAnkleGaitCycle = "TO";
		}

		if (abs(int(minRight - angleRight)) <= 10) {
			(*data)[i].rightAnkleGaitCycle = "TO";
		}
		
		if (angleLeft < (maxLeft - 10) && angleLeft >(minLeft + 10)) {
			//cout << "Foot Flat or mid-stance: " << i << " value " << angleLeft << endl;
			(*data)[i].leftAnkleGaitCycle = "FF";
		}

		if (angleRight < (maxRight - 10) && angleRight >(minRight + 10)) {
			(*data)[i].rightAnkleGaitCycle = "FF";
		}
	}
}

void SmoothData::GetLeftKneeRange()
{
	unsigned int max = 0, min = 180, count = 0, sum = 0, average = 0, range = 0;
	unsigned int angle = 0;

	for (unsigned int i = 0; i < (*data).size(); i++)
	{
		angle = (*data)[i].leftKnee;
		if (angle > max) max = angle;
		if (angle < min) min = angle;

		sum += angle;
		count++;
	}

	average = sum / count;
	range = max - min;

	/*
	cout << endl;
	cout << "Left knee angle max:  " << max << endl;
	cout << "Left knee angle min:  " << min << endl;
	cout << "Left knee angle range: " << range << endl;
	cout << endl
	*/

	this->leftKneeMax = max;
	this->leftKneeMin = min;
}

void SmoothData::GetRightKneeRange()
{
	unsigned int max = 0, min = 180, count = 0, sum = 0, average = 0, range = 0;
	int angle = 0;

	for (unsigned int i = 0; i < (*data).size(); i++)
	{
		angle = (*data)[i].rightKnee;
		if (angle > max) max = angle;
		if (angle < min) min = angle;

		sum += angle;
		count++;
	}

	average = sum / count;
	range = max - min;

	/*
	cout << endl;
	cout << "Right knee angle max:  " << max << endl;
	cout << "Right knee angle min:  " << min << endl;
	cout << "Right knee angle range: " << range << endl;
	cout << endl;
	*/

	this->rightKneeMax = max;
	this->rightKneeMin = min;

}




SmoothData::~SmoothData()
{
}


