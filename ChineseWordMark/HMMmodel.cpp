#include "HMMmodel.h"
using namespace std;

HMM::HMM()
{
}

HMM::~HMM()
{
}
int HMM::getT()
{
	return T;
}
void HMM::setT(int t)
{
	T = t;
}
void HMM::setO(int* os)
{
	O = os;
}
void HMM::setData(int t, int* o, int m ,int n, double* pi, double** tm, double** cm )
{
	T = t;
	O = o;
	M = m;
	N = n;
	Pi = pi;
	TMatrix = tm;
	CMatrix = cm;
}
// inputfile 格式
// 隐藏状态数 M
// 可观察状态数 N
// 初始概率矩阵 Pi M*1
// 转移概率矩阵 TMatrix M*M
// 混淆概率矩阵 CMatrix M*N
// 初始化模型
void HMM::initialModel(const char* inputfile)
{
	ifstream fin;
	//const char* inputfile = "outputtest.txt";
	fin.open(inputfile);
	if (!fin.is_open())
	{
		cout<<"can not open inputfile: HMMModel"<<endl;
		return ;
	}

	//HMM hmm;
	fin >> M >> N;

	Pi = new double [M];
	TMatrix = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		TMatrix[i] = new double [M];
	}
	CMatrix = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		CMatrix[i] = new double [N];
	}

	//赋值
	for (int i = 0; i < M; ++i)
	{
		fin >> Pi[i];			
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			fin >> TMatrix[i][j];
		}
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			fin>> CMatrix[i][j];
		}
	}
	fin.close();
}
//同时载入模型参数与观测序列数据
void HMM::initialData(const char* inputfile1, const char* inputfile2)
{
	ifstream fin1;
	//const char* inputfile = "outputtest.txt";
	fin1.open(inputfile1);
	if (!fin1.is_open())
	{
		cout<<"can not open inputfile: HMMModel"<<endl;
		return ;
	}

	//HMM hmm;
	fin1 >> M >> N;

	Pi = new double [M];
	TMatrix = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		TMatrix[i] = new double [M];
	}
	CMatrix = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		CMatrix[i] = new double [N];
	}

	//赋值
	for (int i = 0; i < M; ++i)
	{
		fin1 >> Pi[i];			
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			fin1 >> TMatrix[i][j];
		}
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			fin1>> CMatrix[i][j];
		}
	}
	fin1.close();

	ifstream fin2;
	fin2.open(inputfile2);
	if (!fin2.is_open())
	{
		cout<<"can not open inputfile : datafile"<<endl;
		return ;
	}
	fin2 >> T;
	O = new int [T];
	for (int i = 0; i < T; ++i)
	{
		fin2 >> O[i];
	}
	fin2.close();
}
//前向算法，解决评估问题，即求产生序列的最大概率
void HMM::forward(double& sum_prob)
{
	//sum_prob为最终返回结果
	//sum_prob = sum(alpha[T-1][i]);
	//即对每个可能的终止状态进行求和
	//P(O|M) = sum(P(O,Si|M))
	//理解为，在每个终止状态下，都有可能产生O(T)的行为(观测值)
	//对所有的终止概率求和，即可得到最终的序列
	sum_prob = 0.0; 
	//定义alpha表示在时刻t观测到序列的状态S(i)的概率
	//分配空间并赋初值
	//alpha[1](i) = pi(i)*b(i)(O1);
	double **alpha = new double *[T];
	for (int i = 0; i < T; ++i)
	{
		alpha[i] = new double [M];
	}
	//alpha[1](i) = pi(i)*b(i)(O1);
	for (int i = 0; i < M; ++i)
	{
		alpha[0][i] = Pi[i] * CMatrix[i][O[0]];
	}
	//递归求解alpha(t+1)
	//alpha[t+1][j] = sum(a[t][i]aij)*b[j][O[t+1]]
	for (int t = 0; t < T-1; ++t)
	{
		for (int i = 0; i < M; ++i)
		{
			double sum = 0.0;
			for (int j = 0; j < M; ++j)
			{
				//为什么是TMatrix[j][i]，
				//因为是计算从其他状态转移到i的结果求和
				//		t 		t+1
				//		S1
				//		S2		S2
				//		S3
				sum += alpha[t][j]*TMatrix[j][i];
			}
			alpha[t+1][i] = sum * CMatrix[i][O[t+1]];
		}
	}
	for (int i = 0; i < M; ++i)
	{
		sum_prob += alpha[T-1][i];
	}
	return ;
}
//viterbi，解决解码问题，即就给定序列的最大生成状态
void HMM::viterbi(int* path )
{
	//定义两个T * M的二维矩阵
	//其中 sigma[t][i] 表示在时刻t前t个观测值在状态Si具有的最高概率
	// 记录状态 生成观测序列时最有可能路径的 概率
	// psi[t][i] 跟踪在时刻t-1最大的sigma[t][i]的状态，即最佳前驱状态
	//sigma
	//		   S 		B 	     M
	//	t0	   --		--		 --
			//P1为t0状态下,S0->S1,B0->S1,M0->S1中最大的一个概率
			//P2为t0状态下,S0->B1,B0->B1,M0->B1中最大的一个概率
			//括号中表示t->t+1转移时，产生t+1时刻的Si状态的最有可能的t时刻状态
	//  t1	  P1(B)		P2(S)		 P3(S)
	//  t2    P1(S)     P2(M)		 P3(M)
			//psi即记录t->t+1转移时，产生t+1时刻的Si状态的最有可能的t时刻状态
	// psi
	//		   S 		B 	     M
	//	t0	  -1		-1	 	 -1
	//  t1     B		S 		 S
	//  t2	   S 		M 		 M
	double** sigma = new double *[T];
	int** psi = new int *[T];
	for (int i = 0; i < T; ++i)
	{
		sigma[i] = new double [M];
		psi[i] = new int [M];
	}
	//初始化 
	for (int i = 0; i < M; ++i)
	{
		sigma[0][i] = Pi[i]*CMatrix[i][O[0]];
		psi[0][i] = -1;
	}
	//递归
	//sigma[t][j] = max(sigma[t-1][i])*a[i][j]*b[j][O[t]]
	//psi[t][j] = arg max(sigma[t-1][i])*a[i][j]
	for (int t = 0; t < T-1; ++t)
	{
		for (int i = 0; i < M; ++i)
		{
			double max_value = 0.0;
			double temp_value = 0.0;
			int pointer = -1;
			for (int j = 0; j < M; ++j)
			{
				temp_value = sigma[t][j] * TMatrix[j][i];
				if (temp_value > max_value)
				{
					max_value = temp_value ;
					pointer = j;
				}
			}
			sigma[t+1][i] = max_value * CMatrix[i][O[t+1]];
			psi[t+1][i] = pointer;
		}
	}
	double max_prob = 0.0;
	path[T-1] =-1;
	for (int i = 0; i < M; ++i)
	{
		if (sigma[T-1][i] > max_prob)
		{
			max_prob = sigma[T-1][i];
			path[T-1] = i;
		}
	}
	for (int i = T-2; i >= 0; i--)
	{
		path[i] = psi[i+1][path[i+1]];
	}
	return ;
}