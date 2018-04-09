#include "headers.h"
#include <Windows.h>
#include "SmoothData.h"
#include <iomanip>

using namespace std;

template<class Interface>
static inline void safeRelease(Interface *&interfaceToRelease)
{
	if (interfaceToRelease != nullptr) {
		interfaceToRelease->Release();
		interfaceToRelease = nullptr;
	}
}

int main(int argc, char *argv[])
{
	bool startr = false;
	char mode = NULL;
	//File for recording all data & gait_cycle
	ofstream recording;
	ofstream gait_cycle;
	ofstream geometric_pos;
	ofstream smoothed;

	//ostringstream aa;
	//aa << "subject" << SUBJECT << "_Seq_" << RUN << "_JointAngles_" << TYPE << ".csv";

	//remove("smoothed_jointangles.csv");

	//Vector to store recording angle data
	vector<int> *JRA = new vector<int>;

	//Vector to store recording angle data
	vector<double> *JRD = new vector<double>;

	//Vector to store recording raw data
	vector<double> *data_raw = new vector<double>;

	IKinectSensor *sensor = nullptr;
	IBodyFrameReader *bodyFrameReader = nullptr;

	//Sleep(3000);

	cout << "Start walking..." << endl;

	//Sleep(500);

	//Get the default Kinect sensor
	HRESULT hr = GetDefaultKinectSensor(&sensor);

	//If the function succeeds, open the sensor
	if (SUCCEEDED(hr)) {
		hr = sensor->Open();

		if (SUCCEEDED(hr)) {
			//Get a body frame source from which we can get our body frame reader
			IBodyFrameSource *bodyFrameSource = nullptr;
			hr = sensor->get_BodyFrameSource(&bodyFrameSource);

			if (SUCCEEDED(hr)) {
				hr = bodyFrameSource->OpenReader(&bodyFrameReader);
			}

			//We're done with bodyFrameSource, so we'll release it
			safeRelease(bodyFrameSource);
		}
	}

	if (sensor == nullptr || FAILED(hr)) {
		std::cout << "Cannot find any sensors.\n";
		return E_FAIL;
	}

	//Search for the first instance of a body (WIP)

	//Prompt for start
	//std::cout << "Press 'r' to begin" << std::endl;
	//mode = std::cin.get();

	//if (mode == 'r' || mode == 'R')
	//{
	//recording.open("JointAngles.csv");
	//gait_cycle.open("Gait_Cycle.csv");
	//geometric_pos.open("Geometric_Positions.csv");
	//smoothed.open("Smoothed_JointAngles.csv");



	ostringstream aa;
	aa << ".\\recordings\\subject" << SUBJECT << "_Seq_" << RUN << "_Gen_" << GENDER << "_JointAngles_" << TYPE << ".csv";
	string J_A = aa.str();

	ostringstream bb;
	bb << ".\\recordings\\subject" << SUBJECT << "_Seq_" << RUN << "_Gen_" << GENDER << "_GeometricPositions_" << TYPE << ".csv";
	string G_P = bb.str();

	ostringstream cc;
	cc << ".\\recordings\\subject" << SUBJECT << "_Seq_" << RUN << "_Gen_" << GENDER << "_GaitCycle_" << TYPE << ".csv";
	string G_C = cc.str();

	ostringstream dd;
	dd << ".\\recordings\\subject" << SUBJECT << "_Seq_" << RUN << "_Gen_" << GENDER << "_SmoothedJointAngles_" << TYPE << ".csv";
	string S_J_A = dd.str();


	recording.open(J_A);
	gait_cycle.open(G_C);
	geometric_pos.open(G_P);
	smoothed.open(S_J_A);

	geometric_pos << "Foot Left X, Y, Z, Ankle Left X, Y, Z, Knee Left X, Y, Z, Hip Left X, Y, Z, Foot Right X, Y, Z, Ankle Right X, Y, Z, Knee Right X, Y, Z, Hip Right X, Y, Z,Spine Base X, Y, Z,Spine Mid X, Y, Z , Neck X, Y, Z, Head X, Y, Z , ShouldL X, Y, Z, ShouldR X, Y, Z,WristLeft X,Y,Z,WristRight X,Y,Z,ElbowLeft X,Y,Z,ElbowRight X,Y,Z,SholdD,LSHD,RSHD,HIPD,LWristSpineD,RWristSpineD, ShoulDX, HipDX \n";

	recording << "Left Knee Angle, Right Knee Angle,Left Ankle Angle,Right Ankle Angle,Left Ankle Delta,Right Ankle Delta\n";

	gait_cycle << "Difference Between AL & AR \n";

	smoothed << "Left Knee Angle, Right Knee Angle, Left Ankle Angle, Right Ankle Angle, Left Ankle Delta, Right Ankle Delta,Left Ankle Avg,Right Ankle Avg,LA-GC-Phases,RA-GC-Phases\n";

	receiveData(sensor, bodyFrameReader, BODY_COUNT, JRA, JRD, data_raw);



	// Saving all the joint positions

	// calculate the distance between the two shoulders with the Euclidean formula
	// ankle_distance = sqrt(pow((AL_X - AR_X), 2) + pow((AL_Y - AR_Y), 2) + pow((AL_Z - AR_Z), 2));

	int counter = 0;

	// these are used to calculate distance beteen shoulder and shoulder
	// and between shoulder and hip
	double sxleft, syleft, szleft, sxright, syright, szright = 0;
	double hxleft, hyleft, hzleft, hxright, hyright, hzright = 0;
	double wxleft, wyleft, wzleft, wxright, wyright, wzright = 0;
	double spinex, spiney, spinez = 0;

	// take the x,y,z data of each position that we are interested in and output them
	// to a comma delimited csv file. "counter" is usually the column number. 
	for (unsigned int i = 0; i < (*data_raw).size(); i = i + 3)
	{
		geometric_pos << (*data_raw)[i];
		geometric_pos << ",";
		geometric_pos << (*data_raw)[i + 1];
		geometric_pos << ",";
		geometric_pos << (*data_raw)[i + 2];

		counter++;

		// get x,y,z of left hip (position may change)
		if (counter == 4)
		{
			hxleft = (*data_raw)[i];
			hyleft = (*data_raw)[i + 1];
			hzleft = (*data_raw)[i + 2];
		}

		// get x,y,z of right hip (position may change)
		if (counter == 8)
		{
			hxright = (*data_raw)[i];
			hyright = (*data_raw)[i + 1];
			hzright = (*data_raw)[i + 2];
		}

		// get x,y,z of mid spine to calc distance with other joints
		if (counter == 10)
		{
			spinex = (*data_raw)[i];
			spiney = (*data_raw)[i + 1];
			spinez = (*data_raw)[i + 2];
		}


		// get x,y,z of left shoulder (position may change)
		if (counter == 13)
		{
			sxleft = (*data_raw)[i];
			syleft = (*data_raw)[i + 1];
			szleft = (*data_raw)[i + 2];
		}

		// get x,y,z of right shoulder (position may change)
		if (counter == 14)
		{
			sxright = (*data_raw)[i];
			syright = (*data_raw)[i + 1];
			szright = (*data_raw)[i + 2];
		}
		// get x,y,z of left wrist
		if (counter == 15)
		{
			wxleft = (*data_raw)[i];
			wyleft = (*data_raw)[i + 1];
			wzleft = (*data_raw)[i + 2];
		}
		// get x,y,z of right wrist
		if (counter == 16)
		{
			wxright = (*data_raw)[i];
			wyright = (*data_raw)[i + 1];
			wzright = (*data_raw)[i + 2];
		}

		// if you just wrote the last column, instead of putting a comma at the end
		// put an endl so excel knows there is a new row starting and set
		// counter to zero.  counter is really the column #
		if (counter == DATAPOINTS_POS)
		{

			counter = 0;
			double hip_distance = 0;
			double shoulder_distance = 0;
			double left_shoulder_hip_distance = 0;
			double right_shoulder_hip_distance = 0;
			double left_wrist_spine_distance = 0;
			double right_wrist_spine_distance = 0;

			// we are at the end of the row, so this is a good place to calculate the 
			// distance between the two shoulders and add it to the end of the row.
			// e.g. sqrt(pow((AL_X - AR_X), 2) + pow((AL_Y - AR_Y), 2) + pow((AL_Z - AR_Z), 2));
			// left shoulder = pos 11, right shoulder = pos 12
			// 


			shoulder_distance = sqrt(pow((sxleft - sxright), 2) + pow((syleft - syright), 2) + pow((szleft - szright), 2));
			hip_distance = sqrt(pow((hxleft - hxright), 2) + pow((hyleft - hyright), 2) + pow((hzleft - hzright), 2));
			left_shoulder_hip_distance = sqrt(pow((sxleft - hxleft), 2) + pow((syleft - hyleft), 2) + pow((szleft - hzleft), 2));
			right_shoulder_hip_distance = sqrt(pow((sxright - hxright), 2) + pow((syright - hyright), 2) + pow((szright - hzright), 2));
			left_wrist_spine_distance = sqrt(pow((wxleft - spinex), 2) + pow((wyleft - spiney), 2) + pow((wzleft - spinez), 2));
			right_wrist_spine_distance = sqrt(pow((wxright - spinex), 2) + pow((wyright - spiney), 2) + pow((wzright - spinez), 2));
			geometric_pos << "," << shoulder_distance << "," << left_shoulder_hip_distance << "," << right_shoulder_hip_distance << "," << hip_distance << "," << left_wrist_spine_distance << "," << right_wrist_spine_distance;
			geometric_pos << "," << abs(sxleft - sxright) << "," << abs(hxleft - hxright);

			geometric_pos << endl;
		}
		else
		{
			geometric_pos << ",";
		}



		/*  -- this was the old that didn't work because is was duplicating the last column from
		the 2nd last column. (oops)
		if (counter == DATAPOINTS_POS - 1)
		{
		i = i + 3;
		geometric_pos << (*data_raw)[i];
		geometric_pos << ",";
		geometric_pos << (*data_raw)[i + 1];
		geometric_pos << ",";
		geometric_pos << (*data_raw)[i + 2];
		//geometric_pos << ",";
		geometric_pos << std::endl;
		counter = 0;
		}
		*/

	}

	calculateHeight(*data_raw, (*data_raw).size());
	geometric_pos.close();


	// Saving the whole recording to "JointAngles.csv"
	counter = 0;

	vector<JointAngles> * angles = new vector<JointAngles>();

	for (unsigned int i = 0; i < (*JRA).size(); i += 4) {
		JointAngles row((*JRA)[i], (*JRA)[i + 1], (*JRA)[i + 2], (*JRA)[i + 3]);
		angles->push_back(row);
	}

	SmoothData smoothData(angles, 10);
	smoothData.CalculateDeltas();
	smoothData.Smooth();

	for (unsigned int i = 0; i < angles->size(); i++)
	{
		smoothed << (*angles)[i].leftKnee;
		smoothed << ",";
		smoothed << (*angles)[i].rightKnee;
		smoothed << ",";
		smoothed << (*angles)[i].leftAnkle;
		smoothed << ",";
		smoothed << (*angles)[i].rightAnkle;
		smoothed << ",";
		smoothed << (*angles)[i].leftAnkleDelta;
		smoothed << ",";
		smoothed << (*angles)[i].rightAnkleDelta;
		smoothed << ",";
		smoothed << (*angles)[i].leftAnkleMA;
		smoothed << ",";
		smoothed << (*angles)[i].rightAnkleMA;
		smoothed << ",";
		smoothed << (*angles)[i].leftAnkleGaitCycle;
		smoothed << ",";
		smoothed << (*angles)[i].rightAnkleGaitCycle;

		smoothed << endl;
	}

	smoothed.close();

	counter = 0;

	for (unsigned int i = 0; i < (*JRA).size(); i++)
	{
		recording << (*JRA)[i];
		recording << ",";
		counter++;
		if (counter == DATAPOINTS - 1)
		{
			i++;
			recording << (*JRA)[i];
			// add the left and right ankle deltas (amount of change) into the 5th and 6th column
			if (i > 3) {
				// left ankle deltas
				recording << "," << ((*JRA)[i - 1]) - ((*JRA)[i - 5]);
				// right angle deltas
				recording << "," << ((*JRA)[i]) - ((*JRA)[i - 4]);
			}
			else
				recording << "," << 0 << "," << 0;
			recording << std::endl;
			counter = 0;
		}
	}

	recording.close();


	// Saving the whole recording to "AnkleDistance.csv"
	counter = 0;

	for (unsigned int i = 0; i < (*JRD).size(); i++)
	{
		gait_cycle << (*JRD)[i];
		gait_cycle << std::endl;
	}
	gait_cycle.close();


	/* not using gait_cycle file now. we may come back to it

	//// Step 1) Identifying a complete gait cycle in the caputred data
	//// By considering the ankle position variations over time -> Ankle joint is stationary -> Position starts to change -> Ankle comes to rest again
	int start_point = 0;
	int end_point = 0;

	// Finding the starting location in the data set, by checking for the min
	// number in the first 50% of the numbers.
	double max = 0;

	for (unsigned int i = 2; i < (*JRA).size() / 2; i += 4)
	{
	double t1 = (*JRA)[i];

	if (t1 > max)
	{
	max = t1;
	start_point = i - 2;
	}
	}

	cout << "The max value in the first half is: " << max  << endl;
	max = 0;
	//Finding the end location in the data set
	int s = (*JRA).size();

	for (unsigned int i = 2; i < (*JRA).size(); i += 4)
	{
	double t1 = (*JRA)[i];

	if (t1 > max && i > s/2) {
	max = t1;
	end_point = i - 2;
	}
	}

	cout << "The max value in the 2nd half is: " << max << endl;
	cout << "The start point is: " << start_point << " and the end point is " << end_point << endl;

	////Building the data set containing one gait cycle written to gait_cycle.csv
	counter = 0;

	for (unsigned int i = start_point; i < (*JRA).size(); i++)
	{
	gait_cycle << (*JRA)[i];
	gait_cycle << ",";
	counter++;

	if (counter == DATAPOINTS - 1)
	{
	i++;
	gait_cycle << (*JRA)[i];
	// add the left and right ankle deltas (amount of change) into the 5th and 6th column
	if (i > 4) {
	gait_cycle << "," << ((*JRA)[i - 1]) - ((*JRA)[i - 5]);
	gait_cycle << "," << ((*JRA)[i]) - ((*JRA)[i - 4]);
	}
	else
	gait_cycle << 0;

	gait_cycle << std::endl;
	counter = 0;
	}

	}

	gait_cycle.close();
	*/

	int x = 0;
	std::cout << "press a key to exit";
	std::cin >> x;

	return 0;
}


// return the height in metres.
// head to neck +
// neck to mid-spine +
// mid-spine to hip centre +
// left hip to left knee +
// left knee to left ankle
double calculateHeight(vector<double>  data, int size)
{
	double height = 0;
	double headx = 0, heady = 0, headz = 0;
	double neckx = 0, necky = 0, neckz = 0;
	double midspinex = 0, midspiney = 0, midspinez = 0;
	double spinebasex = 0, spinebasey = 0, spinebasez = 0;
	double lefthipx = 0, lefthipy = 0, lefthipz = 0;
	double leftkneex = 0, leftkneey = 0, leftkneez = 0;
	double leftanklex = 0, leftankley = 0, leftanklez = 0;
	int counter = 0;
	vector<double> heightData;
	vector<double> leftArmLengthData;
	vector<double> leftLegLengthData;

	// add variables to capture arm length - shoulder->elbow->wrist left side only.
	// arm lenght is not related to height, just calculated here and output to
	// height file for conveneience

	double armlength = 0;
	double leftshoulderx = 0, leftshouldery = 0, leftshoulderz = 0;
	double leftelbowx = 0, leftelbowy = 0, leftelbowz = 0;
	double leftwristx = 0, leftwristy = 0, leftwristz = 0;
	double leftleglength = 0;

	ostringstream aa;
	aa << ".\\recordings\\subject" << SUBJECT << "_Seq_" << RUN << "_Gen_" << GENDER << "_Height_" << TYPE << ".csv";
	string filename = aa.str();


	for (unsigned int i = 0; i < size; i = i + 3)
	{
		counter++;

		// head
		if (counter == 12)
		{
			headx = data[i];
			heady = data[i + 1];
			headz = data[i + 2];
		}

		//neck
		if (counter == 11)
		{
			neckx = data[i];
			necky = data[i + 1];
			neckz = data[i + 2];
		}

		// mid spine
		if (counter == 10)
		{
			midspinex = data[i];
			midspiney = data[i + 1];
			midspinez = data[i + 2];
		}

		// spine base
		if (counter == 9)
		{
			spinebasex = data[i];
			spinebasey = data[i + 1];
			spinebasez = data[i + 2];
		}

		// left hip
		if (counter == 4)
		{
			lefthipx = data[i];
			lefthipy = data[i + 1];
			lefthipz = data[i + 2];
		}

		// left knee
		if (counter == 3)
		{
			leftkneex = data[i];
			leftkneey = data[i + 1];
			leftkneez = data[i + 2];
		}

		// left ankle
		if (counter == 2)
		{
			leftanklex = data[i];
			leftankley = data[i + 1];
			leftanklez = data[i + 2];
		}

		// left shoulder - to get arm length
		if (counter == 13)
		{
			leftshoulderx = data[i];
			leftshouldery = data[i + 1];
			leftshoulderz = data[i + 2];
		}

		// left elbow - to get arm length
		if (counter == 17)
		{
			leftelbowx = data[i];
			leftelbowy = data[i + 1];
			leftelbowz = data[i + 2];
		}

		// left wrist to get get arm lenght
		if (counter == 15)
		{
			leftwristx = data[i];
			leftwristy = data[i + 1];
			leftwristz = data[i + 2];
		}


		if (counter == DATAPOINTS_POS)
		{
			// height is calculated for each frame with two different joint paths. once in excel
			// we can look at average or median.
			// data is converted 
			counter = 0;

			// head is tracked in the middle of the face so it is recommeded to add
			// 9 to 12 cm to adjust height to the top of head.
			double adjust = 0;

			// this method traverses down:
			// head->neck->mid spine->spine base ->  left knee = left ankle;
			height = 0;
			height = calc3dDistance(headx, heady, headz, neckx, necky, neckz);
			height += calc3dDistance(neckx, necky, neckz, midspinex, midspiney, midspinez);
			height += calc3dDistance(midspinex, midspiney, midspinez, spinebasex, spinebasey, spinebasez);
			height += calc3dDistance(spinebasex, spinebasey, spinebasez, leftkneex, leftkneey, leftkneez);
			height += calc3dDistance(leftkneex, leftkneey, leftkneez, leftanklex, leftankley, leftanklez);
			height += adjust;  // compensate for tracking middle of head not top.
			heightData.push_back(height);

			// this method traverses down:
			// head->neck->mid spine->spine base -> left hip -> left knee = left ankle;
			height = 0;  // temp height accumulation of the joint distances
			height = calc3dDistance(headx, heady, headz, neckx, necky, neckz);
			height += calc3dDistance(neckx, necky, neckz, midspinex, midspiney, midspinez);
			height += calc3dDistance(midspinex, midspiney, midspinez, spinebasex, spinebasey, spinebasez);
			height += calc3dDistance(lefthipx, lefthipy, lefthipz, leftkneex, leftkneey, leftkneez);
			height += calc3dDistance(leftkneex, leftkneey, leftkneez, leftanklex, leftankley, leftanklez);
			height += adjust;
			heightData.push_back(height);


			// left leg length

			leftleglength = 0;
			leftleglength += calc3dDistance(lefthipx, lefthipy, lefthipz, leftkneex, leftkneey, leftkneez);
			leftleglength += calc3dDistance(leftkneex, leftkneey, leftkneez, leftanklex, leftankley, leftanklez);

			leftLegLengthData.push_back(leftleglength);
			leftLegLengthData.push_back(0.0L);

			// calculate the arm length

			armlength = 0;
			armlength += calc3dDistance(leftshoulderx, leftshouldery, leftshoulderz, leftelbowx, leftelbowy, leftelbowz);
			armlength += calc3dDistance(leftelbowx, leftelbowy, leftelbowz, leftwristx, leftwristy, leftwristz);
			leftArmLengthData.push_back(armlength);
			leftArmLengthData.push_back(0.0L);
		}
	}

	ofstream osheight;
	int rows = DATASIZE + 1;

	osheight.open(filename);

	osheight << "Height M 1" << "," << "Height Cm 1" << "," << "Height Ft 1" << "," << "Height M 2" << "," << "Height Cm 2" << "," << "Height Ft 2" << ",," << "Left Arm Cm" << "," << "Left Leg CM" << endl;
	for (int i = 0; i < heightData.size(); i += 2)
	{
		osheight << setprecision(3) << heightData[i] << ",";
		osheight << setprecision(5) << heightData[i] * 100 << ",";
		osheight << setprecision(3) << ((heightData[i] * 100) / 2.54) / 12 << ",";
		osheight << setprecision(3) << heightData[i + 1] << ",";
		osheight << setprecision(5) << heightData[i + 1] * 100 << ",";
		osheight << setprecision(3) << ((heightData[i + 1] * 100) / 2.54) / 12 << ",,";
		osheight << setprecision(3) << leftArmLengthData[i] * 100 << ",";
		osheight << setprecision(3) << leftLegLengthData[i] * 100;
		osheight << endl;
	}

	ostringstream avg1, avg2, avg3, avg4, avg5, avg6;
	ostringstream med1, med2, med3, med4, med5, med6;
	ostringstream max1, max2, max3, max4, max5, max6;

	avg1 << "=average(a2:" << "a" << rows << ")";
	avg2 << "=average(b2:" << "b" << rows << ")";
	avg3 << "=average(c2:" << "c" << rows << ")";
	avg4 << "=average(d2:" << "d" << rows << ")";
	avg5 << "=average(e2:" << "e" << rows << ")";
	avg6 << "=average(f2:" << "f" << rows << ")";

	osheight << avg1.str() << ",";
	osheight << avg2.str() << ",";
	osheight << avg3.str() << ",";
	osheight << avg4.str() << ",";
	osheight << avg5.str() << ",";
	osheight << avg6.str();
	osheight << endl;

	med1 << "=median(a2:" << "a" << rows << ")";
	med2 << "=median(b2:" << "b" << rows << ")";
	med3 << "=median(c2:" << "c" << rows << ")";
	med4 << "=median(d2:" << "d" << rows << ")";
	med5 << "=median(e2:" << "e" << rows << ")";
	med6 << "=median(f2:" << "f" << rows << ")";

	osheight << med1.str() << ",";
	osheight << med2.str() << ",";
	osheight << med3.str() << ",";
	osheight << med4.str() << ",";
	osheight << med5.str() << ",";
	osheight << med6.str();
	osheight << endl;


	max1 << "=max(a2:" << "a" << rows << ")";
	max2 << "=max(b2:" << "b" << rows << ")";
	max3 << "=max(c2:" << "c" << rows << ")";
	max4 << "=max(d2:" << "d" << rows << ")";
	max5 << "=max(e2:" << "e" << rows << ")";
	max6 << "=max(f2:" << "f" << rows << ")";

	osheight << max1.str() << ",";
	osheight << max2.str() << ",";
	osheight << max3.str() << ",";
	osheight << max4.str() << ",";
	osheight << max5.str() << ",";
	osheight << max6.str();
	osheight << endl;
	osheight.close();

	return height;
}

// calculate a distance in 3d space
// e.g. sqrt(pow((AL_X - AR_X), 2) + pow((AL_Y - AR_Y), 2) + pow((AL_Z - AR_Z), 2));
double calc3dDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
	double distance = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2) + pow((z1 - z2), 2));
	return distance;
}

//function to receive all the data
void receiveData(IKinectSensor *sensor, IBodyFrameReader *bodyFrameReader, const unsigned int &bodyCount, std::vector<int> *JRA, std::vector<double> *JRD, std::vector<double> *data_raw)
{
	HRESULT hr;

	while (bodyFrameReader != nullptr) {
		IBodyFrame *bodyFrame = nullptr;
		hr = bodyFrameReader->AcquireLatestFrame(&bodyFrame);
		if (SUCCEEDED(hr)) {
			IBody *bodies[BODY_COUNT] = { 0 };
			hr = bodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);

			if (SUCCEEDED(hr)) {

				processBodies(BODY_COUNT, bodies, JRA, JRD, data_raw);

				//After body processing is done, we're done with our bodies so release them.
				for (unsigned int bodyIndex = 0; bodyIndex < _countof(bodies); bodyIndex++) {
					safeRelease(bodies[bodyIndex]);
				}

				safeRelease(bodyFrame);
			}

			if ((*JRA).size() == DATASIZE * DATAPOINTS)
			{
				std::cout << "Data Recorded..." << std::endl;
				break;
			}

		}
		else if (sensor) {
			BOOLEAN isSensorAvailable = false;
			hr = sensor->get_IsAvailable(&isSensorAvailable);
			if (SUCCEEDED(hr) && isSensorAvailable == false) {
				std::cerr << "No available sensor is found.\n";
			}
		}
		else {
			std::cerr << "Trouble reading the body frame.\n";
		}
	}
}

void processBodies(const unsigned int &bodyCount, IBody **bodies, std::vector<int> *JRA, std::vector<double> *JRD, std::vector<double> *data_raw)
{
	for (unsigned int bodyIndex = 0; bodyIndex < bodyCount; bodyIndex++) {
		IBody *body = bodies[bodyIndex];

		//Get the tracking status for the body, if it's not tracked we'll skip it
		BOOLEAN isTracked = false;
		HRESULT hr = body->get_IsTracked(&isTracked);
		if (FAILED(hr) || isTracked == false) {
			continue;
		}

		//If we're here the body is tracked so lets get the joint properties for this skeleton
		Joint joints[JointType_Count];
		hr = body->GetJoints(_countof(joints), joints);
		if (SUCCEEDED(hr)) {

			CameraSpacePoint pA, pB, pC, P_raw;
			float AR_X, AR_Y, AR_Z, AL_X, AL_Y, AL_Z;
			double angle_prev, ankle_distance;

			int indexA[] = { JointType_AnkleLeft, JointType_AnkleRight, JointType_FootLeft, JointType_FootRight };
			int indexB[] = { JointType_HipLeft, JointType_HipRight, JointType_KneeLeft, JointType_KneeRight };
			int indexC[] = { JointType_KneeLeft, JointType_KneeRight, JointType_AnkleLeft, JointType_AnkleRight };
			int indexD[] = { JointType_FootLeft, JointType_AnkleLeft, JointType_KneeLeft, JointType_HipLeft, JointType_FootRight,
				JointType_AnkleRight, JointType_KneeRight, JointType_HipRight, JointType_SpineBase, JointType_SpineMid, JointType_Neck,
				JointType_Head ,JointType_ShoulderLeft,JointType_ShoulderRight,JointType_WristLeft,JointType_WristRight,JointType_ElbowLeft,JointType_ElbowRight };

			for (int i = 0; i < DATAPOINTS; i++)
			{
				pA = joints[indexA[i]].Position;
				pB = joints[indexB[i]].Position;
				pC = joints[indexC[i]].Position;


				//	indexC[2].Position;
				//	get X, Y, Z coordinates : Pc.X...
				//	indexC[3].Position;
				//	do the same
				//	find Euclidean distance*/
				if (i == 0)
				{
					AL_X = pA.X;
					AL_Y = pA.Y;
					AL_Z = pA.Z;
				}

				if (i == 1)
				{
					AR_X = pA.X;
					AR_Y = pA.Y;
					AR_Z = pA.Z;

					ankle_distance = sqrt(pow((AL_X - AR_X), 2) + pow((AL_Y - AR_Y), 2) + pow((AL_Z - AR_Z), 2));
					//std::cout << angledeg << std::endl;
					//std::cout << ankle_distance << std::endl;
					// Insert JRD into vector
					(*JRD).push_back(ankle_distance);
				}

				// left ankle x = left ankle x - left knee x
				pA.X = pA.X - pC.X;

				// left ankle y = left ankle y - left knee y
				pA.Y = pA.Y - pC.Y;

				// left ankle z = left ankle z - left knee z
				pA.Z = pA.Z - pC.Z;

				pB.X = pB.X - pC.X;
				pB.Y = pB.Y - pC.Y;
				pB.Z = pB.Z - pC.Z;

				double normA = sqrt(pow(pA.X, 2) + pow(pA.Y, 2) + pow(pA.Z, 2));
				double normB = sqrt(pow(pB.X, 2) + pow(pB.Y, 2) + pow(pB.Z, 2));

				double anglerad = std::acos((pA.X*pB.X + pA.Y*pB.Y + pA.Z*pB.Z) / (normA*normB));
				double angledeg = anglerad * (180 / PI);

				if (i == 2 || i == 3)
				{
					//angledeg = angledeg - 110;
				}

				// Insert JRA into vector
				(*JRA).push_back(angledeg);

			}

			for (int j = 0; j < DATAPOINTS_POS; j++)
			{
				P_raw = joints[indexD[j]].Position;

				// Insert Raw Position into vector
				(*data_raw).push_back(P_raw.X);
				(*data_raw).push_back(P_raw.Y);
				(*data_raw).push_back(P_raw.Z);
			}
		}


		else
		{
			std::cout << "NO BODY FOUND" << std::endl;
		}
	}
}