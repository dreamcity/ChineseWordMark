#include "iostream"
#include "fstream"
#include "string"
#include "map"
#include "vector"
#define Separator "/ "
using namespace std;

class WordSplit
{
public:
	WordSplit();
	~WordSplit();
	//将文件按标点符号分隔
	int getNum();
	int* getT();
	int**getO();
	vector<string> getVStr();

	//对输入的测试文本进行处理，得到按照标点符号分隔之后的句子集合,每一句话，单独写入一行
	void getSplitSegFile(const char* inputfile, const char* outputfile);
	void loadMapData(const char* inputfile); //载入wordmap数据表
	//将观测文本的每一行制作成观察序列
	void getOSequence(const char* inputfile, const char* outputfile);
	//将所有观察序列制成数组存储int* T, int** O
	void getOSdata(const char* inputfile);
	//将每一行字符串push进vector中
	void file2Vector(const char* inputfile);
	//输入viterbi解码之后的序列，得到标注结果
	void getVStrSegResult(int* path,  string & str2);


private:
	map<string, int> wordmap;
	vector<string> vstr;
	int num;
	int* T;
	int** O;

};