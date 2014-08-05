#include "SplitModel.h"
using namespace std;
map<string , int > mapdata;

template <class T1, class T2>
void disPlay1D(T1* arr, T2 m)
{
	for (int i = 0; i < m; ++i)
	{
		cout<<arr[i]<< "   ";
		
	}
	cout <<endl;;
}
template <class T1, class T2>
void disPlay2D(T1** arr, T2 m, T2 n)
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout<<arr[i][j]<<"     ";
		}
		cout<<endl;
	}
}
SplitModel::SplitModel()
{
}
SplitModel::~SplitModel()
{
}
map<string , int >  SplitModel::getMapData()
{
	return MapData;
}
//将每一种状态的转移存入map表中
void  initiation(map<string , int >& mapstring)
{
	//map<string , int >mapstring;
	mapstring["SS"] = 0;
	mapstring["SB"] = 1;
	mapstring["BM"] = 2;
	mapstring["BE"] = 3;
	mapstring["MM"] = 4;
	mapstring["ME"] = 5;
	mapstring["ES"] = 6;
	mapstring["EB"] = 7;
	return ;
}
//初始化HMMmodel，主要是分配数组空间
void SplitModel::initialModel()
{
	M = 4;
	N = 5300;
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
	for (int i = 0; i < M; ++i)
	{
		Pi[i] = 0;
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			TMatrix[i][j] = 0;
		}
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			CMatrix[i][j] = 0;
		}
	}
}
//对输入的测试文本进行处理，得到按照标点符号分隔之后的句子集合
//每一句话，单独写入一行
//类似getSplitSegFile
void SplitModel::getSplitSegFile(const char* inputfile, const char* outputfile)
{
	ifstream infile;
	infile.open(inputfile);
	ofstream outfile;
	outfile.open(outputfile);
	string strtmp;		//每一个待分割的字符串
	string str1 = "";	//存储分割之后的字符串
	string str2 = "";	//存储未完成的句子的后半部分
	//今天是个好天气，我们
	//去游泳，好不好
	//strtmp = 今天是个好天气，我们
	//str1 = 今天是个好天气，
	//str2 = 我们
	//strtmp = 去游泳，好不好
	//str1 = 我们 + 去游泳，
	//str2 = 好不好
	while(getline(infile, strtmp, '\n'))
	{
		if(strtmp.empty())
		{
			continue;
		}
		while(!strtmp.empty())
		{
			int len = strtmp.length();
			int index = 0;
			for (int i = 0; i < len; )
			{
				unsigned char ch0 = strtmp[i];				
				if (ch0 < 128)
				{
					i++;
				}
				else
				{
					unsigned char ch1 = strtmp[i+1];
					if ((ch0 ==163 && ch1==161)||(ch0 ==161 && ch1==163)||(ch0 ==163 && ch1==187)
						||(ch0 ==163 && ch1==172)||(ch0 ==163 && ch1==191)||(ch0 ==161 && ch1==170)
						||(ch0 ==163 && ch1==186)||(ch0 ==161 && ch1==176)||(ch0 ==161 && ch1==177))
					{
						index = i;
						break;
					}
					else
					{
						i+=2;
					}
				}
			}
			if (index == 0)
			{
				str2 = strtmp.substr(0, len);
				strtmp = "";
			}
			else
			{
				str1 = str2 + strtmp.substr(0, index+2);
				str2 = "";
				strtmp =strtmp.substr(index+2, len-index-2);
			}

			if (str2.empty()&& str1.length()>20)
			{
				outfile << str1 <<endl;
				
			}
		}
	}
	if(str2.length()>20)
	{
		outfile << str2 <<endl;
	}
	return ;
}
//将字符串按 指定的字符串分割
vector<string>smsplit(string str,string pattern)
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
//begin********************************************************
//给输入的字符串进行标记，分别得到计算转移概率矩阵的statefile--str3
//以及计算混淆概率矩阵的markfile--str2
void markSentence(string str1, string& str2 ,string& str3)
{
	//int index;
	int len = str1.length();
	unsigned char ch = (unsigned char )str1[0];
	// 英文字符串直接在最后添加/S，单词不分割
	if ( ch < 128 )
	{
		str2 = str1 + "S";
		str3 = "S";
	}
	//else if ( ch < 176 && ch > 128)
	//{

	//	str2 = str1 +"S";
	//	str3 = "S";
	//}
	else
	{
		if ( len == 2)
		{
			str2 = str1 + "S";
			str3 = "S";
		}
		else if (len==4)
		{
			str2 = str1.substr(0,2) + "B";
			str2 += str1.substr(2,4) + "E";
			str3 = "B";
			str3 += "E";
		}
		else
		{
			str2 = str1.substr(0,2) + "B";
			str3 =  "B";
			for (int j = 2; j < len-2;)
			{
				str2 += str1.substr(j,2) + "M";
				str3 += "M";
				j+=2;
			}
			str2 += str1.substr(len-2, 2) + "E";
			str3 += "E";
		}
	}
	return ;	
}
void SplitModel::getMarkSentenceFile(const char* inputfile, const char* outputfile, const char* outputfile1)
{
	string strtmp; //用于保存从语料库中读入的每一行
 	string markline;
 	ifstream infile;
	infile.open(inputfile);
 	ofstream outfile(outputfile);
	//outfile.open(outputfile);
	ofstream outfile1(outputfile1);
	//outfile.open(outputfile1);
	int num = 0;
	cout<<"loading filedata..."<<endl;
	while(getline(infile, strtmp, '\n'))
	//for (int j = 0; j < 500; ++j)
 	{
		//getline(infile, strtmp, '\n');
		if (strtmp.empty())
		{
			continue ;
		}

		//cout<<"the num of records "<<num<<" ! " <<endl;
		num++;
 		//getline(infile, strtmp, '\n');
     	string pattern = " ";
     	vector<std::string> result=smsplit(strtmp,pattern);
		
		string str2;
		string str3;
		for( int i=0; i<result.size(); i++)
 	    {
			string t = result[i].c_str();
			markSentence(t,str2,str3);
			outfile << str2 ;
			outfile1 << str3;
		}
		outfile1<<endl;
		outfile<<endl;
	}
	//outfile1.close();
	//outfile.close();
	//infile.close();
	cout<<"the num of records "<<num<<" ! " <<endl;
}
//end**********************************************************

//计算初始概率矩阵Pi[S,B,M,E]
void SplitModel::getInitMatrix(const char* inputfile)
{
	
	string strtmp;
	ifstream infile;
	infile.open(inputfile);
	if (!infile.is_open())
	{
		cout<<"can not open inputfile"<<endl;
		return ;
	}
	int* count = new int [M];
	for (int i = 0; i < M; ++i)
	{
		count[i] = 0;
	}

	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		if (strtmp[0] == 'S')
		{
			count[0]+=1;
		}
		else
		{
			count[1]+=1;
		}
	}
	int sum = count[0] + count[1];
	Pi[0] = (double)count[0] / sum;
	Pi[1] = (double)count[1] / sum;
	Pi[2] = (double)count[2] / sum;
	Pi[3] = (double)count[3] / sum;
	infile.close();
	cout<<"the inilital probality matrix is:"<<endl;
	disPlay1D(Pi, 4);
	return ;
}
//begin**********************************************************************
//计算每一种转移的数目
void countNum(string& mark, int* count)
{
	//int count[8] = {0};
	//string mark;
	map<string , int > mapstring;
	initiation(mapstring);
	int val = mapstring[mark];
	switch (val)
		{
			case 0: 
				count[0]+=1; break;
			case 1: 
				count[1]+=1; break;
			case 2:
				count[2]+=1; break;
			case 3:
				count[3]+=1; break;
			case 4: 
				count[4]+=1; break;
			case 5:
				count[5]+=1; break;
			case 6:
				count[6]+=1; break;
			case 7:
				count[7]+=1; break;
			default :cout<<"error"<<endl; break;
		}
	return ;
}
//将转移状态的数目，转变为相应的概率
//并将概率存入二维数组中
void SplitModel::countFre(int* count, double* markfre, int m)
{
	int* sum = new int [M];
	for (int i = 0; i < M; ++i)
	{
		sum[i] = 0;
	}
	int j = 0;
	for (int i = 0; i < M; ++i)
	{
		sum[i] = count[j] + count[j+1];
		j+=2;
	}

	for (int i = 0; i < m; ++i)
	{
		markfre[i] = (double)count[i] / sum[i/2];
	}
	return ;
}
void SplitModel::get2DTMatrix(double* markfre)
{
	TMatrix[0][0] = markfre[0];
	TMatrix[0][1] = markfre[1];
	TMatrix[1][2] = markfre[2];
	TMatrix[1][3] = markfre[3];
	TMatrix[2][2] = markfre[4];
	TMatrix[2][3] = markfre[5];
	TMatrix[3][0] = markfre[6];
	TMatrix[3][1] = markfre[7];
	return ;
}
//计算转移概率矩阵
void SplitModel::getTranMatrix(const char* inputfile)
{
	cout<<"counting the transfrom probality matrix ..."<<endl;
	string strtmp;
	string mark;

	int* countmark = new int [8];
	for (int i = 0; i < 8; ++i)
	{
		countmark[i] = 0;
	}
	double* countfre = new double [8];
	for (int i = 0; i < 8; ++i)
	{
		countfre[i] = 0;
	}
	ifstream infile;
	infile.open(inputfile);
	if (!infile.is_open())
	{
		cout<<"can not open inputfile"<<endl;
		return ;
	}
	
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		int length = strtmp.length() ;
		for (int j = 0; j < length-1; ++j)
		{
			mark = strtmp[j];
			mark += strtmp[j+1];
			countNum(mark, countmark);
		}
	}
	countFre(countmark, countfre, 8);	
	get2DTMatrix(countfre);
	cout<<"the transfrom probality matrix is:"<<endl;
	disPlay2D(TMatrix,M,M);
	infile.close();
	return ;
}
//end**********************************************************************

//begin********************************************************************
//将输入的观测序列的集合文件，制作为每个中文字符的map表
//同时将map保存在全局变量mapdata中
void getMap(const char* inputfile, std::map<string, int>& mm)
{
	string strtmp;
	ifstream infile;
	infile.open(inputfile); 
	if (!infile.is_open())
	{
		cout<<"can not open inputfile"<<endl;
		return ;
	} 

	map<string,int>::iterator m_it;
	int num=0;
	string t0,t1,tmp;
 	while(getline(infile, strtmp, '\n'))
 	{
		if (strtmp.empty())
		{
			continue ;
		}
 		int length = strtmp.length();
 		for (int i = 0; i < length-3; i+=3)
 		{
 			t0=strtmp.substr(i,2);
 			t1=strtmp.substr(i+2,1);
			unsigned char ch = (unsigned char )t0[0];
			if ( ch < 128)
			{
				int j = i;
				unsigned char ch1 = (unsigned char )t0[0];
				while (ch1 < 128)
				{
					j++;
					t0 = strtmp.substr(j,2);
					ch1 = t0[0];
				}
				t0 = strtmp.substr(i, j-i-1);
				t1 = strtmp.substr(j-1,1);
			}
 			tmp = t0 + t1;
 			m_it = mm.find(t0);
 			if (m_it == mm.end())
 			{
 				mm.insert(pair<string,int>(t0,num));
 				num++;
 			}
 			else
 			{
 				continue;
 			}		
 		}
 	}
	mapdata = mm;
	infile.close();
 	return ;
}
//通过查找map表，计算出混淆矩阵每个位置对应的计数值
void getCMCoutData(const char* inputfile, double** countCMData)
{
	string strtmp;
	string t0,t1,tmp;
	std::map<string, int> mm0;
	std::map<string, int> mm1;
	map<string,int>::iterator m_it;
	getMap(inputfile,  mm0);
	cout << "the map size: "<<mm0.size()<<endl;
	ifstream infile;
	infile.open(inputfile);
 	while(getline(infile, strtmp, '\n'))
 	{
		if (strtmp.empty())
		{
			continue ;
		}
 		int length = strtmp.length();
 		for (int i = 0; i < length-3;i+=3)
 		{
 			t0 = strtmp.substr(i,2);
 			t1 = strtmp.substr(i+2,1);
			unsigned char ch = (unsigned char )t0[0];
			if ( ch < 128)
			{
				int j = i;
				while (ch < 128)
				{
					j++;
					t0 = strtmp.substr(j,2);
					ch = t0[0];
				}
				t0 = strtmp.substr(i, j-i-1);
				t1 = strtmp.substr(j-1,1);
			}

 			tmp= t0 + t1;
 			unsigned char c = t1[0];
 			int val = mm0[t0];
 			m_it = mm1.find(tmp);
 			if (m_it == mm1.end())
 			{
 				mm1.insert(pair<string, int>(tmp,val));
 				switch(c)
 				{
	 				case 'S':	countCMData[0][val]=1; break;
	 				case 'B':	countCMData[1][val]=1; break;
	 				case 'M':	countCMData[2][val]=1; break;
	 				case 'E':	countCMData[3][val]=1; break;
					default :   cout<<"Error0!!!: "<<c<<endl; break;
 				}
 			}
 			else
 			{
 				switch(c)
 				{
	 				case 'S':	countCMData[0][val]+=1; break;
	 				case 'B':	countCMData[1][val]+=1; break;
	 				case 'M':	countCMData[2][val]+=1; break;
	 				case 'E':	countCMData[3][val]+=1; break;
					default :   cout<<"Error1!!!: "<<c<<endl; break;
 				}
 			}
 		}
 	}	
	infile.close();
	return ;
}
//根据计数值，得到概率值
void SplitModel::getConMatrix(const char* inputfile)
{
	cout<<"counting the confuse probality matrix ..."<<endl;
	double** countCMData = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		countCMData[i] = new double [N];
	}

	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			countCMData[i][j] = 0;
		}
	}
	getCMCoutData(inputfile, countCMData);
	//计算概率
	double* sum = new double [M];
	for (int i = 0; i < M; ++i)
	{
		sum[i] = 0.0;
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			sum[i] += countCMData[i][j];
		}		
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			CMatrix[i][j] = (double)countCMData[i][j] / sum[i] ;
		}
	}
	//cout<<"the confuse probality matrix is:"<<endl;
	//disPlay2D(CMatrix, M,N);
	return ;
}
//end**********************************************************************
//保存训练之后的HMM模型参数
void SplitModel::saveHMMModel(const char* outputfile)
{
	ofstream outfile;
	outfile.open(outputfile);
	outfile << M <<endl;
	outfile << N <<endl;
	for (int i = 0; i < M; ++i)
	{
		outfile << Pi[i] << "   ";
		//cout << Pi[i] << "   ";
	}
	//cout << endl;
	outfile << endl;
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			outfile << TMatrix[i][j] << "   ";
			//cout << TMatrix[i][j] <<"   ";
		}
		//cout <<endl;
		outfile << endl;
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			outfile << CMatrix[i][j] << "   ";
			//cout << CMatrix[i][j] << "   ";
		}
		//cout <<endl;
		outfile << endl;
	}
	outfile.close();
	return ;
}
//保存观测集合的Map表数据
void SplitModel::saveMapData(const char* outputfile)
{
	// mapdata 为全局变量
	// MapData 为类的私有变量
	MapData = mapdata;
	ofstream outfile;
	outfile.open(outputfile);
	std::map<string,int>::iterator m_it;
	for ( m_it = MapData.begin(); m_it != MapData.end(); m_it++ )
	{
		outfile << m_it->first <<" "<< m_it->second << endl;
	}
	return ;
}