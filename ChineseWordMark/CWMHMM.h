#include "SplitModel.h"
#include "WordSplit.h"
#include "WordMark.h"
#include "MarkModel.h"
#include "HMMmodel.h"

class CWMHMM
{
public:
	CWMHMM();
	~CWMHMM();
	const char* getSplitHMMfile();
	const char* getSplitMAPData();
	const char* getMarkHMMfile();
	const char* getStateMapData();
	const char* getWordMapData();

	//加载训练数据，生成分词用的HMM模型参数
	void getSplitHMMModel();
	//加载训练数据，生成词性标注用的HMM模型参数
	void getMarkHMMModel();
	//输入测试文本，输出分词结果
	void splitStentes(const char* datatest, const char* segresult);
	//输入分词结果，得到标注结果
	void markWords(const char* inputfile, const char* outputfile);

private:
	//中文分词部分需要的文件
	const char* splittraindata ;	//分词的训练集
	const char* splitsegdata ;		//对训练集按标点分隔之后的训练集
	const char* splitmarkfile ;		//加入标签的训练集
	const char* splitstatefile ;	//提取的标签序列集合
	const char* splitHMMModel ;		//分词的HMM模型参数
	const char* splitMapData ;		//训练集的map数据表
	//词性标注需要的文件
	const char* marktraindata ;		//词性标注的训练集
	const char* wordmarkfile ;		 //加入标签的训练集，实际上只是预处理了一下，源训练集自带标签
	const char* wordstatefile ;		//提取的标签序列集合
	const char* stateMapdata ;		//标签的map数据表
	const char* wordMapdata ;		//词语的map数据集
	const char* markHMMmodel ;		//词性标注的HMM参数集
	//输入输出的测试数据
	const char* datatestseg ;		//输入的测试文本
	const char* splitOS ;			//分词时提取的观测序列
	const char* markwordOS ;		//词性标注时的观测序列

};