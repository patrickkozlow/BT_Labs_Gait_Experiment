#pragma once

#include <Kinect.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>
#include <direct.h>



#define PI 3.14159265
// DATASIZE was originally 100
#define DATASIZE 120
#define DATAPOINTS 4
#define DATAPOINTS_POS 18
#define SUBJECT 25
#define RUN 1
#define TYPE "N"	//N for Normal and A for Abnormal AL for Left and AR for right OA for old age
#define GENDER "M" //F for Female and M for Male

void receiveData(IKinectSensor *sensor, IBodyFrameReader *bodyFrameReader, const unsigned int &bodyCount, std::vector<int> *JRA, std::vector<double> *JRD, std::vector<double>*data_raw);
void processBodies(const unsigned int &bodyCount, IBody **bodies, std::vector<int> *JRA, std::vector<double> *JRD, std::vector<double> *data_raw);
double calc3dDistance(double x1, double y1, double z1, double x2, double y2, double z2);
double calculateHeight(std::vector<double>  data, int size);