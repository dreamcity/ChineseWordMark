#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "map"
#include "regex"
using namespace std;
class MarkModel
{
public:
	MarkModel();
	~MarkModel();
	map<string , int> getStateMap();
	map<string , int> getWordMap();
	//对训练集预处理，得到最终的一个单词一个标签的文件
	void filePreprocess(const char* inputfile, const char* outputfile);
	//inputfile : 输入预处理之后的带标签文件markfile.txt outputfile : 提取之后的状态文件statefile.txt
	//同时提取到statemap与wordmap数据表
	void getMapData(const char* inputfile, const char* outputfile);
	void initialModel(); //初始化模型参数
	void getIniPro(const char* inputfile);//求解初始概率矩阵
	void getTranPro(const char* inputfile);// 求解转移概率矩阵
	void getConPro(const char* inputfile);//求解混淆概率矩阵
	void saveMapData(const char* outputfile, map<string, int> MapData);
	void saveModelData(const char* outputfile);

private:
	map<string , int> statemap;	//词性标签的map表
	map<string , int> wordmap;	//单词的map数据表
	int M;						//隐藏状态，即标签个数
	int N;						//观测状态，即单词个数
	double* Pi;					// M*1
	double** TranMatrix;		// M*M
	double** ConMatrix;			// M*N
	
};
