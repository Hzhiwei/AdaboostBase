#include "stdafx.h"
#include "WeekTrainer.h"


WeekTrainer::WeekTrainer(int _dims):
		dims(_dims)
{

}


WeekTrainer::~WeekTrainer()
{

}

void WeekTrainer::AddExample(vector<vector<double>> &E, bool M)
{
	if (M)
	{
		PosExample.insert(PosExample.end(), E.begin(), E.end());
	}
	else
	{
		NegExample.insert(NegExample.end(), E.begin(), E.end());
	}
}


bool WeekTrainer::StartTrain(double accuracy)
{
	vector<double> exaWight;			//样本权重
	int posExaNum = PosExample.size();	//正样本总数量
	int negExaNum = NegExample.size();	//负样本总数量
	int exaNum = PosExample.size() + NegExample.size();	//样本总数量

	vector<tuple<vector<double>, int, double>> example;

	example.resize(exaNum);
	vector<tuple<vector<double>, int, double>>::iterator itor = example.begin();
	for (int index = 0; index < posExaNum; ++index)
	{
		*itor = tuple<vector<double>, int, double>(PosExample[index], 1, 1.0 / exaNum);
		++itor;
	}
	for (int index = 0; index < negExaNum; ++index)
	{
		*itor = tuple<vector<double>, int, double>(NegExample[index], -1, 1.0 / exaNum);
		++itor;
	}

	/*********************************************************************/
	CascadeClassifer.clear();
	weekClassifer tempClassifer;
	vector<double> Zparam;
	Zparam.resize(exaNum);

	tempClassifer.errorRate = 1;

	for (int counter = 0; ExampleErrorRate() > 1 - accuracy; ++counter)
	{
		GetBestWeekClassifer(example, tempClassifer);
		tempClassifer.weights = 0.5 * log((1 - tempClassifer.errorRate) / tempClassifer.errorRate);

		CascadeClassifer.push_back(tempClassifer); 

		cout << "the " << counter << " weekClassifer, " << "error rate is " << ExampleErrorRate() << endl;

		double Z = 0;
		for (int index = 0; index < exaNum; ++index)
		{
			Z += get<2>(example[index]) * exp(-tempClassifer.weights * get<1>(example[index]) * Classification(get<0>(example[index])));
		}
		for (int index = 0; index < exaNum; ++index)
		{
			get<2>(example[index]) = get<2>(example[index]) / Z * exp(-tempClassifer.weights * get<1>(example[index]) * Classification(get<0>(example[index])));
		}
	}

	/*********************************************************************/

	return true;
}

int WeekTrainer::Classification(vector<double>& features)
{
	double sum = 0;
	for (vector<weekClassifer>::iterator itor = CascadeClassifer.begin(); itor != CascadeClassifer.end(); ++itor)
	{
		sum += (features[itor->featureNum] * itor->symble > itor->threshold * itor->symble ? 1 : -1) * itor->weights;
	}

	if (sum > 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

void WeekTrainer::GetBestWeekClassifer(
	vector<tuple<vector<double>, int, double>>& Example,
	weekClassifer& Classifer)
{
	int exaNum = Example.size();	//样本总数量

	vector<tuple<double, int, double>> exaFeature;
	exaFeature.resize(exaNum);

	weekClassifer tempAllClassifer;
	tempAllClassifer.errorRate = 1;
	for (int feature = 0; feature < dims; ++feature)
		//对所有特征进行循环
	{
		//针对某一特征加入所有样本
		vector<tuple<double, int, double>>::iterator itor = exaFeature.begin();

		for (int index = 0; index < exaNum; ++index)
		{
			*itor = tuple<double, int, double>((get<0>(Example[index]))[feature], get<1>(Example[index]), get<2>(Example[index]));
			++itor;
		}

		//特征值排序
		QuickSort(exaFeature, 0, exaFeature.size() - 1);

		//找出误差最小的阈值位置
		weekClassifer tempFeatureClassifer;
		tempFeatureClassifer.errorRate = 1;
		for (int index = 1; index < exaNum; ++index)
			//对单一特征所有阈值（样本）进行循环
		{
			double tempError = 0;
			int tempSymbol;
			double tempThreshold = (get<0>(exaFeature[index - 1]) + get<0>(exaFeature[index])) / 2;
			for (int err = 0; err < exaNum; ++err)
				//循环统计误差
			{
				if (
					((get<0>(exaFeature[err]) <= tempThreshold)
					&& (get<1>(exaFeature[err]) == 1))
					||
					((get<0>(exaFeature[err]) >= tempThreshold)
						&& (get<1>(exaFeature[err]) == -1))
					)
				{
					tempError += get<2>(exaFeature[err]);
				}
			}
			if (tempError > 0.5)
			{
				tempError = 1 - tempError;
				tempSymbol = -1;
			}
			else
			{
				tempSymbol = 1;
			}

			if (tempFeatureClassifer.errorRate > tempError)
			{
				tempFeatureClassifer.errorRate = tempError;
				tempFeatureClassifer.threshold = tempThreshold;
				tempFeatureClassifer.symble = tempSymbol;
			}
		}

		if (tempAllClassifer.errorRate > tempFeatureClassifer.errorRate)
		{
			tempAllClassifer.errorRate = tempFeatureClassifer.errorRate;
			tempAllClassifer.symble = tempFeatureClassifer.symble;
			tempAllClassifer.featureNum = feature;
			tempAllClassifer.threshold = tempFeatureClassifer.threshold;
		}
	}
	Classifer = tempAllClassifer;
}

void WeekTrainer::QuickSort(vector<tuple<double, int, double>>& Data, int begin, int end)
{
	if (end - begin <= 0)
	{
		return;
	}
	else if (end -begin == 1)
	{
		if (get<0>(Data[begin]) > get<0>(Data[end]))
		{
			swap(Data, begin, end);
		}
	}
	else
	{
		swap(Data, (begin + end) / 2, end);

		int storeIndex = end;
		for (int index = end - 1; index >= begin; --index)
		{
			if (get<0>(Data[index]) > get<0>(Data[end]))
			{
				--storeIndex;
				swap(Data, storeIndex, index);
			}
		}
		swap(Data, storeIndex, end);
		QuickSort(Data, begin, storeIndex - 1);
		QuickSort(Data, storeIndex + 1, end);
	}
}

double WeekTrainer::ExampleErrorRate()
{
	int errorCount = 0;
	for (int index = PosExample.size() - 1; index >= 0; --index)
	{
		if (Classification(PosExample[index]) == -1)
		{
			++errorCount;
		}
	}
	for (int index = NegExample.size() - 1; index >= 0; --index)
	{
		if (Classification(NegExample[index]) == 1)
		{
			++errorCount;
		}
	}

	return (double)errorCount / (PosExample.size() + NegExample.size());
}

inline void WeekTrainer::swap(vector<tuple<double, int, double>> &Data, int begin, int end)
{
	tuple<double, int, double> temp;
	temp = Data[begin];
	Data[begin] = Data[end];
	Data[end] = temp;
}
