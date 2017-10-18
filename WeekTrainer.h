#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>

using namespace std;

class WeekTrainer
{
public:
	struct weekClassifer
	{
		int featureNum;
		double threshold;
		int symble;
		double errorRate;
		double weights;
	};

public:
	WeekTrainer(int _dims);
	~WeekTrainer();

	void AddExample(vector<vector<double>> &E, bool M);

	bool StartTrain(double accuracy);

	int Classification(vector<double> &feature);

	void QuickSort(vector<tuple<double, int, double>>& Data, int begin, int end);

	double ExampleErrorRate();

public:

	//符号正样本大于阈值为正
	void GetBestWeekClassifer(
		vector<tuple<vector<double>, int, double>>& Example,
		weekClassifer& Classifer);

	inline void swap(vector<tuple<double, int, double>> & Data, int begin, int end);

	vector<vector<double>> PosExample;
	vector<vector<double>> NegExample;
	vector<weekClassifer> CascadeClassifer;
	int dims;
};

