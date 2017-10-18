// WeekAdaBoost.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include "WeekTrainer.h"


using namespace std;


float PosExampleArray[][3] = {
	{ 5.5,  2.1,  0.3 },
	{ 2. ,  1.1,  3.4 },
	{ 1.3,  5. ,  0.2 }
};
float NegExampleArray[][3] = {
	{ 1. ,  1.7,  1.1 },
	{ 7.4,  1.8,  1.3 },
	{ 2.1,  0.3,  1.35 }
};


vector<vector<double>> PosExample;
vector<vector<double>> NegExample;

int dims = 3;

void CreateExample(int count);

int main()
{
	WeekTrainer W(3);

	//样本初始化
	//PosExample.push_back(vector<double>(PosExampleArray[0], PosExampleArray[0] + 3));
	//PosExample.push_back(vector<double>(PosExampleArray[1], PosExampleArray[1] + 3));
	//PosExample.push_back(vector<double>(PosExampleArray[2], PosExampleArray[2] + 3));
	//NegExample.push_back(vector<double>(NegExampleArray[0], NegExampleArray[0] + 3));
	//NegExample.push_back(vector<double>(NegExampleArray[1], NegExampleArray[1] + 3));
	//NegExample.push_back(vector<double>(NegExampleArray[2], NegExampleArray[2] + 3));

	srand(2);
	CreateExample(100);

	W.AddExample(PosExample, true);
	W.AddExample(NegExample, false);
	W.StartTrain(0.9999);

	system("pause");

    return 0;
}


const double a = 4.5, b = -9.4, c = 2.8, d = -3;
void CreateExample(int count)
{
	for (int index = 0; index < count; ++index)
	{
		double x = ((rand() % 2000) - 1000) / 10.0;
		double y = ((rand() % 2000) - 1000) / 10.0;
		double z = ((rand() % 2000) - 1000) / 10.0;

		vector<double> temp;

		temp.push_back(x);
		temp.push_back(y);
		temp.push_back(z);

//		if (a * x + b * y + c * z + d > 0)
		if( x * x + y * y + z * z < 2500)
		{
			PosExample.push_back(temp);
		}
		else
		{
			NegExample.push_back(temp);
		}
	}
}
