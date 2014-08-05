//算法参考wiki百科
//《机器学习导论》Ethem Alplaydin 第二版第15章
//http://zh.wikipedia.org/wiki/%E7%BB%B4%E7%89%B9%E6%AF%94%E7%AE%97%E6%B3%95
//http://www.cnblogs.com/zhangchaoyang/articles/2220398.html
//http://www.52nlp.cn/hmm-learn-best-practices-one-introduction
#include "iostream"
#include "fstream"
//#include "BaumWelch.h"
using namespace std;

//class HMM :public BaumWelch 
class HMM
{
public:
	HMM();
	~HMM();
	//int getM();
	//int getN();
	int getT();
	void setT(int t);
	void setO(int* os);
	void setData(int t, int* o, int m ,int n, double* pi, double** tm, double** cm );
	void initialModel(const char* inputfile);
	void initialData(const char* inputfile1, const char* inputfile2);
	void forward(double& sum_prob);
	void viterbi(int* path);

private:
	int T;	//观测序列的长度
	int* O;	//观测序列
	int M;	//隐藏的状态数目
	int N;	//可观察的行为数目
	double* Pi;	// 初始化概率矩阵 M*1
	double** TMatrix;	// 转移概率矩阵 M*M
	double** CMatrix;	// 混合概率矩阵 M*N,表示为在某一个状态可能的行为

};