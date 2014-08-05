#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "map"

using namespace std;
class SplitModel
{
public:
	SplitModel();
	~SplitModel();
	map<string , int > getMapData();
	//outputfile 为观测样本集添加标注
	// eg 今B天E是S星B期M一E
	// outputfile1 保存观测样本集的标注
	// eg BESBME
	void initialModel();//初始化HMMmodel，主要是分配数组空间
	//对训练集按标点符号分割
	void getSplitSegFile(const char* inputfile, const char* outputfile);
	//得到添加标签之后的markfile
	void getMarkSentenceFile(const char* inputfile, const char* outputfile1, const char* outputfile2);
	//计算转移概率
	void countFre(int* count, double* markfre, int m);
	//将countFre得到的概率存入二维数组中
	void get2DTMatrix(double* markfre);
	//计算初始概率局长呢、转移概率矩阵、混淆概率矩阵
	void getInitMatrix(const char* inputfile);
	void getTranMatrix(const char* inputfile);
	void getConMatrix(const char* inputfile);
	void saveHMMModel(const char* outputfile);
	void saveMapData(const char* outputfile);
private:
	int M;	//隐藏的状态数目
	int N;	//可观察的行为数目
	double* Pi;	// 初始化概率矩阵 M*1
	double** TMatrix;	// 转移概率矩阵 M*M
	double** CMatrix;	// 混合概率矩阵 M*N,表示为在某一个状态可能的行为
	map<string , int > MapData;	
		
};
