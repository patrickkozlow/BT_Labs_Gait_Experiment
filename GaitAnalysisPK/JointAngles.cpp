#include "JointAngles.h"


JointAngles::JointAngles(int lk, int rk, int la, int ra)
{
	leftKnee = lk;
	rightKnee = rk;
	leftAnkle = la;
	leftAnkleDelta = 0;
	rightAnkle = ra;
	rightAnkleDelta = 0;
	leftAnkleMA = 0;
	rightAnkleMA = 0;

}


JointAngles::JointAngles()
{

}

JointAngles::~JointAngles()
{

}

