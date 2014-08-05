#include "CWMHMM.h"
CWMHMM::CWMHMM()
{
	//中文分词部分需要的文件
	splittraindata = "../database/splittraindata.txt";
	splitsegdata = "../tempdata/splittempdata/splitsegdata.txt";
	splitmarkfile = "../tempdata/splittempdata/splitmarkfile.txt";
	splitstatefile = "../tempdata/splittempdata/splitstatefile.txt";
	splitHMMModel = "../tempdata/splitdata/splitHMMmodel.txt";
	splitMapData = "../tempdata/splitdata/splitMAPdata.txt";
	//词性标注需要的文件
	marktraindata = "../database/marktraindata.txt";
	wordmarkfile = "../tempdata/marktempdata/wordmarkfile.txt"; 
	wordstatefile = "../tempdata/marktempdata/wordstatefile.txt"; 
	stateMapdata =  "../tempdata/markdata/statemapdata.txt";
	wordMapdata =  "../tempdata/markdata/wordmapdata.txt";
	markHMMmodel =  "../tempdata/markdata/markHMMmodel.txt";
	//输入输出文件
	datatestseg = "../tempdata/iotempdata/datatestseg.txt";
	splitOS = "../tempdata/iotempdata/splitOS.txt";
	markwordOS = "../tempdata/iotempdata/markwordOS.txt";

}
CWMHMM::~CWMHMM()
{
}
const char* CWMHMM::getSplitHMMfile()
{
	return splitHMMModel;
}
const char* CWMHMM::getSplitMAPData()
{
	return splitMapData;
}
const char* CWMHMM::getMarkHMMfile()
{
	return markHMMmodel;
}
const char* CWMHMM::getStateMapData()
{
	return wordMapdata;
}
const char* CWMHMM::getWordMapData()
{
	return stateMapdata;
}

//加载训练数据，生成分词用的HMM模型参数
void CWMHMM::getSplitHMMModel()
{
	SplitModel SM;
	//将观测数据集，转换为按标点符号提取的字符串集合
	SM.getSplitSegFile(splittraindata , splitsegdata);
	//得到标记状态之后的markfile，已经纯粹的状态序列集
	SM.getMarkSentenceFile(splitsegdata, splitmarkfile, splitstatefile);
	SM.initialModel();
	SM.getInitMatrix(splitstatefile);	//计算初始概率矩阵
	SM.getTranMatrix(splitstatefile);	//计算转移概率矩阵
	SM.getConMatrix(splitmarkfile);		//计算混淆概率矩阵
	SM.saveMapData(splitMapData);		//提取观测序列集合中，字符对应的map数据表
	SM.saveHMMModel(splitHMMModel);

	return ;
}
//加载训练数据，生成词性标注用的HMM模型参数
void CWMHMM::getMarkHMMModel()
{
	MarkModel MM;
	//文件的预处理，得到按标点分隔的句子集合
	MM.filePreprocess(marktraindata, wordmarkfile);
	//得到statefile，同时还有map信息
	MM.getMapData(wordmarkfile,wordstatefile);
	MM.initialModel();
	MM.getIniPro(wordstatefile);
	MM.getTranPro(wordstatefile);
	MM.getConPro(wordmarkfile);
	MM.saveModelData(markHMMmodel);
	map<string , int> statemap = MM.getStateMap();
	map<string , int> wordmap = MM.getWordMap();
	MM.saveMapData(stateMapdata, statemap);
	MM.saveMapData(wordMapdata, wordmap);

	return ;
}
//输入测试文本，输出分词结果
void CWMHMM::splitStentes(const char* inputfile, const char* outputfile)
{
	WordSplit WS;
	//将输入的待分词文本，首先按标点符号提取字符串
	WS.getSplitSegFile(inputfile, datatestseg);
	//根据MAP的信息，将输入的中文字符串，转换为对应的观测序列编码
	WS.loadMapData(splitMapData);
	WS.getOSequence(datatestseg, splitOS);

	//提取文本中的观测序列，存储在数组中
	WS.getOSdata(splitOS);
	//将输入的中文字符串，存入vector<string>容器内
	WS.file2Vector(datatestseg);
	//准备载入 输入观察序列的长度与编码
	int num = WS.getNum();
	int* T = WS.getT();
	int** O = WS.getO();
	vector<string> vstr = WS.getVStr();
	//初始化HMM模型，并载入由观测序列生成的HMM参数
	HMM WSHMM;
	WSHMM.initialModel(splitHMMModel);
	string str = "";
	
	//分词结果存入segresult中
	ofstream fout(outputfile);
	for (int i = 0; i < num; ++i)
	{			
		WSHMM.setT(T[i]);
		WSHMM.setO(O[i]);
		int* path = new int [T[i]];
		for(int j =0; j<T[i] ; j++)
		{
			path[j] = 0;
		}
		//调用viterbi算法，得到path序列（int 数组）
		WSHMM.viterbi(path);	
		//通过path数组，将分隔符标记在字符串的vector容器中的每一个元素
		WS.getVStrSegResult(path,str);
		fout<<str<<endl;
		delete path;
	}
	return ;
}
//输入分词结果，得到标注结果
void CWMHMM::markWords(const char* inputfile, const char* outputfile)
{
	WordMark WM;
	WM.loadWMapData(wordMapdata);
	WM.loadSMapData(stateMapdata);
	WM.getOSequence(inputfile, markwordOS);
	WM.getOSdata(markwordOS);
	WM.file2Vector(inputfile);
	int num = WM.getNum();
	int* T = WM.getT();
	int** O = WM.getO();
	vector<string> vstr = WM.getVStr();
	//初始化HMM模型，并载入由观测序列生成的HMM参数
	HMM WMHMM;
	WMHMM.initialModel(markHMMmodel);
	string str = "";
	
	//分词结果存入segresult中
	ofstream fout(outputfile);
	for (int i = 0; i < num; ++i)
	{			
		WMHMM.setT(T[i]);
		WMHMM.setO(O[i]);
		int* path = new int [T[i]];
		for(int j =0; j<T[i] ; j++)
		{
			path[j] = 0;
		}
		//调用viterbi算法，得到path序列（int 数组）
		WMHMM.viterbi(path);	
		//通过path数组，将分隔符标记在字符串的vector容器中的每一个元素
		string strtmp = "";
		WM.getMarkResult(path, strtmp);
		str += strtmp;
		delete path;
	}
	fout<<str<<endl;
	return ;	
}
