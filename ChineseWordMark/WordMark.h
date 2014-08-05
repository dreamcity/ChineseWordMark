#include "iostream"
#include "fstream"
#include "string"
#include "map"
#include "vector"
#include "sstream"

using namespace std;
class WordMark
{
public:
	WordMark();
	~WordMark();
	int getNum();
	int* getT();
	int** getO();
	vector<string> getVStr();
	void loadWMapData(const char* inputfile);//载入词语的map表
	void loadSMapData(const char* inputfile);//载入词语的map表
	//将观测文本的每一行制作成观察序列
	void getOSequence(const char* inputfile, const char* outputfile);
	//将所有观察序列制成数组存储int* T, int** O
	void getOSdata(const char* inputfile);
	//将每一行字符串push进vector中
	void file2Vector(const char* inputfile);
	//输入viterbi解码之后的序列，得到标注结果
	void getMarkResult(int* path, string& str);
private:
	map<string, int> wordmap;
	map<int, string> restatemap; //反向的statemap，用于将输出的path转换为词性标记
	vector<string> vstr;
	int num;
	int* T;
	int** O;

};