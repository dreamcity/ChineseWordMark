#include "WordMark.h"
using namespace std;
WordMark::WordMark()
{
}
WordMark::~WordMark()
{
}

int WordMark::getNum()
{
	return num;
}
int* WordMark::getT()
{
	return T;
}
int** WordMark::getO()
{
	return O;
}
vector<string> WordMark::getVStr()
{
	return vstr;
}
vector<string>wmSplit(string str,string pattern)
 {
     std::string::size_type pos;
     std::vector<std::string> result;
     str+=pattern;//扩展字符串以方便操作
     int size=str.size(); 
     for(int i=0; i<size; i++)
     {
         pos=str.find(pattern,i);
         if(pos<size)
         {
             std::string s=str.substr(i,pos-i);
             if (s.length()>0)
             {
             	result.push_back(s);
 	            i=pos+pattern.size()-1;
             }

         }
     }
     return result;
 }

//加载最初由训练时的观察集生成的map表
// map表的每一行，由一个中文字符和对应的map表标号
void WordMark::loadWMapData(const char* inputfile)
{
	 ifstream infile1;
	 infile1.open(inputfile);
	 string linetmp;
	 int linenum=0;
	 // 统计输入文件的行数
	 while(getline(infile1, linetmp, '\n'))
	 {
		if (linetmp.empty())
		{
			continue ;
		}
	 	linenum++;
	 }
	 infile1.close();
	 //载入新的map表
	 ifstream infile2;
	 infile2.open(inputfile);
	 string line, strtmp;
	 for (int i = 0; i < linenum; ++i)
	 {
	 	infile2 >> line;
	 	infile2 >>strtmp;
		int num = atoi(strtmp.c_str());
	 	wordmap.insert(pair<string,int>(line,num));
	 }
	 infile2.close();
	 return ;
}
void WordMark::loadSMapData(const char* inputfile)
{
	 ifstream infile1;
	 infile1.open(inputfile);
	 string linetmp;
	 int linenum=0;
	 // 统计输入文件的行数
	 while(getline(infile1, linetmp, '\n'))
	 {
		if (linetmp.empty())
		{
			continue ;
		}
	 	linenum++;
	 }
	 infile1.close();
	 //载入新的map表
	 ifstream infile2;
	 infile2.open(inputfile);
	 string line, strtmp;
	 for (int i = 0; i < linenum; ++i)
	 {
	 	infile2 >> line;
	 	infile2 >>strtmp;
		int num = atoi(strtmp.c_str());
	 	restatemap.insert(pair<int,string>(num,line));
	 }
	 infile2.close();
	 return ;
}
//观测文本的前提是已经按标点分隔好的
//将观测文本的每一行制作成观察序列
void WordMark::getOSequence(const char* inputfile, const char* outputfile)
{
	num =0;
	string strtmp;
	ifstream infile(inputfile);
	ofstream outfile(outputfile);
	while(getline(infile, strtmp , '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		num++;
		string pattern = "/ ";
		std::vector<string> vstr = wmSplit(strtmp,pattern);
		int T = vstr.size();
		int* OS = new int[T];
		outfile << T <<endl;
		for (int i = 0; i < T; ++i)
		{
			string	tmp = vstr[i].c_str();
			OS[i] = wordmap[tmp];
			outfile << OS[i] <<" ";
		}
		outfile <<endl;
	}
	return ;
}
//将所有观察序列制成数组存储
void WordMark::getOSdata(const char* inputfile)
{
	T = new int [num];
	O = new int* [num];
	ifstream fin;
	fin.open(inputfile);
	for (int i = 0; i < num; ++i)
	{
			
		fin >> T[i];
		O[i] = new int [T[i]];
		for (int j = 0; j < T[i]; ++j)
		{
			fin >> O[i][j];
		}
	}
	fin.close();
	return ;
}
//将每一行字符串push进vector中
void WordMark::file2Vector(const char* inputfile)
{	
	ifstream fin;
	string strtmp;
	fin.open(inputfile);
	while(getline(fin, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		vstr.push_back(strtmp);
	}
	return ;
}
//输入viterbi解码之后的序列，得到标注结果
void WordMark::getMarkResult(int* path, string& str)
{
	if(!vstr.empty())
	{
		string strtmp = vstr[0];//得到vector<string>中的第一个字符串
		vector<string>::iterator v_it;
		v_it = vstr.begin();
		vstr.erase(v_it);//删除vector<string>中的第一个字符串
		string strout = "";

		string pattern = "/ ";
		std::vector<string> vstr = wmSplit(strtmp,pattern);
		for (int i = 0; i < vstr.size(); ++i)
		{
			strout += vstr[i] + "/";
			string ss = restatemap[path[i]] + " ";
			strout += ss;
		}
		str= strout;
	}
	return ;
}
