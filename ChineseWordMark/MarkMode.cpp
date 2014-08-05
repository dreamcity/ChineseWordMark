#include "MarkModel.h"
using namespace std;
MarkModel::MarkModel()
{
}
MarkModel::~MarkModel()
{
}
map<string , int> MarkModel::getStateMap()
{
	return statemap;
}
map<string , int> MarkModel::getWordMap()
{
	return wordmap;
}
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
//字符串分隔
vector<string>mmSplit(string str,string pattern)
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

 //******************************************************************
 //对数据文件预处理，对机构名词处理
//eg [中国/n 工商/n 银行/n]nt ――> 中国工商银行/nt
void fileClean(const char* inputfile, const char* outputfile)
{
	string strtmp;
	ifstream infile(inputfile);
	ofstream outfile(outputfile);
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue;
		}
		while(strtmp.find('[') != string::npos )
		{
			string str = "";
			int len = strtmp.length();
			int i = strtmp.find('[');
			int j = strtmp.find(']');
			string str1 = strtmp.substr(i,j-i+1);

			std::vector<string> vstr = mmSplit(str1," ");
			for (int i = 0; i < vstr.size(); ++i)
			{
				string str2 = vstr[i].c_str();
				unsigned char ch0 =str2[0];
				unsigned char ch1 =str2[1];
				if ((ch0 ==161 &&ch1==176) || (ch0 ==161 &&ch1==177))
				{
					str2 = "";
					continue ;
				}
				//if(str2 == "”/w" || str2 == "“/w")
				//{
				//	str2 = "";
				//	continue ;
				//}
				if (str2.find('/') != string::npos)
				{
					int i = str2.find('/');
					str2 = str2.substr(0,i);
				}
				str += str2;
			}
			int l =str.length();
			strtmp = strtmp.substr(0,i)+str.substr(1,l-1)+"/"+strtmp.substr(j+1, len-j-1);	
		}
	outfile<<strtmp<<endl;
	}
	return ;
}
//清除文件每一行最前面的时间戳 
void cleanTimemark(const char* inputfile, const char* outputfile)
{

	string regString("(\\d{8})\\-(\\d{2})\\-(\\d{3})\\-(\\d{3})\/(\[a-z]+)");	//定义匹配规则
	regex regExpress(regString); // 编译一个正则表达式语句      
	smatch ms; // 保存查找的结果 

	string strtmp;
	ifstream infile ;
	infile.open(inputfile);
	string strout = "";
	ofstream outfile;
	outfile.open(outputfile);
	 //for (int i = 0; i<500; i++)
	while(getline(infile, strtmp, '\n'))
	{
		 //getline(infile, strtmp, '\n');
		if (strtmp.empty())
		{
			continue ;	
		}
		int len = strtmp.length();   
        if(std::regex_search(strtmp, ms, regExpress))    
        {    
			int index = ms.position(0); //匹配到的结果的最初索引号
			int l = ms.length(0); //匹配的结果长度
			strout = strtmp.substr(0,index-0) + strtmp.substr(l+index,len-l);			
        }    
        else    
        {    
			strout = strtmp; 
        }
		outfile << strout <<endl;
	}
	return ;
}
//对输入的测试文本进行处理，得到按照标点符号分隔之后的句子集合
//每一句话，单独写入一行
void getSplitSegFile(const char* inputfile, const char* outputfile)
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
			int index = -1;
			for (int i = 0; i < len; )
			{
				unsigned char ch0 = strtmp[i];
				//为英文字符，索引加1，继续索引
				if (ch0 < 128)
				{
					i++;
				}
				else
				{
					unsigned char ch1 = strtmp[i+1];
					//当索引到的字符为，。；：！-?""时，结束索引
					//163162，  161163。 163187； 163191？  161170―― "161176  "161177
					if ((ch0 ==163 && ch1==161)||(ch0 ==161 && ch1==163)||(ch0 ==163 && ch1==187)
						||(ch0 ==163 && ch1==172)||(ch0 ==163 && ch1==191)
						||(ch0 ==163 && ch1==186)||(ch0 ==161 && ch1==176)||(ch0 ==161 && ch1==177))
					{
						index = i+2;
						break;
					}
					//为其他的中文字符，索引加2，继续索引
					else
					{
						unsigned char ch3 = strtmp[i+2];
						unsigned char ch4 = strtmp[i+3];
						if ((ch0 ==161 && ch1==170)&&(ch3 ==161 && ch4 ==170))
						{
							int j =i;
							while(ch3 ==161 && ch4 ==170)
							{
								j+=2;
								ch3 = strtmp[j+2];
								ch4 = strtmp[j+3];
							}
							index = j+2;
							break;
						}
						i+=2;
					}
				}
			}
			//如果结束索引时，字符串中，没用相应的标点，直接整个字符串赋值给str2
			if (index == -1)
			{
				str2 = strtmp.substr(0, len);
				strtmp = "";
			}
			//将字符串strtmp按索引号分隔（将标点号以及后面的标签包含），同时加上str2，即上一个字符串的末尾
			else
			{
				str1 = str2 + strtmp.substr(0, index+2);
				str2 = "";
				strtmp =strtmp.substr(index+2, len-index-2);
			}

			if (str2.empty() && str1.length()>30)
			{
				outfile << str1 <<endl;
				
			}
		}
		if(str2.length()>30)
		{
			outfile << str2 <<endl;
		}
	}
	return ;
}

void MarkModel::filePreprocess(const char* inputfile, const char* outputfile)
{
	const char* tempfile0 = "./tempdata/tempfile0.txt";
	const char* tempfile1 = "./tempdata/tempfile1.txt";
	fileClean(inputfile, tempfile0);
	cleanTimemark(tempfile0, tempfile1);
	getSplitSegFile(tempfile1, outputfile);
}
//inputfile : 输入预处理之后的带标签文件markfile.txt
//outputfile : 提取之后的状态文件statefile.txt
//同时提取到statemap与wordmap
void MarkModel::getMapData(const char* inputfile, const char* outputfile)
{
	map<string, int>::iterator m_it_s;
	map<string, int>::iterator m_it_w;

	string strtmp; //用于保存从语料库中读入的每一行
	ifstream infile(inputfile);
	ofstream outfile(outputfile);
	int valueS =0;
	int valueW =0;
	while(getline(infile, strtmp, '\n'))
 	{
		if (strtmp.empty())
		{
			continue ;
			break;
		}
     	string pattern = " ";
     	vector<std::string> result=mmSplit(strtmp,pattern);
		
		for( int i=0; i<result.size(); i++)
 	    {
			string tmp = result[i].c_str();
			if (tmp == "\n" || tmp == "\r")
			{
				continue ;
			}
			int index = tmp.find("/");
			if (index == -1)
			{
				cout <<"error"<<endl;
				continue ;
			}
			string strw = tmp.substr(0, index);
			m_it_w = wordmap.find(strw);
			if (m_it_w == wordmap.end())
			{
				wordmap.insert(pair<string,int>(strw, valueW));
				valueW++;
			}

			string strs = tmp.substr(index+1,tmp.length()-index-1);
			m_it_s = statemap.find(strs);
			if (m_it_s == statemap.end())
			{
				statemap.insert(pair<string,int>(strs, valueS));
				valueS++;
			}

			outfile <<"/"<<strs ;
		}
		outfile<<endl;
	}
	return ;
}
//初始化模型参数
void MarkModel::initialModel()
{
	M = statemap.size();
	N = wordmap.size();
	Pi = new double [M];
	TranMatrix = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		TranMatrix[i] = new double [M];
	}
	ConMatrix = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		ConMatrix[i] = new double [N];
	}
	for (int i = 0; i < M; ++i)
	{
		Pi[i] = 0;
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			TranMatrix[i][j] = 0;
		}
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			ConMatrix[i][j] = 0;
		}
	}
}
//求解初始概率矩阵
void MarkModel::getIniPro(const char* inputfile)
{
	string strtmp;
	ifstream infile(inputfile);
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue;
		}
		string pattern = "/";
     	vector<std::string> result=mmSplit(strtmp,pattern);
     	string str = result[0].c_str();
     	int index = statemap[str];
     	Pi[index] += 1;
	}
	double sum =0.0;
	for (int i = 0; i < M; ++i)
	{
		sum += Pi[i];
	}
	for (int i = 0; i < M; ++i)
	{
		Pi[i] = (double)Pi[i]/sum;
	}
	cout<<"the inilital probality matrix is:"<<endl;
	disPlay1D(Pi, M);
	return;
}
// 求解转移概率矩阵
void MarkModel::getTranPro(const char* inputfile)
{
	string strtmp;
	ifstream infile(inputfile);
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue;
		}
		string pattern = "/";
     	vector<std::string> result=mmSplit(strtmp,pattern);

     	for (int i = 0; i < result.size()-1; ++i)
     	{
     		string str1 = result[i].c_str();
     		string str2 = result[i+1].c_str();
     		int row = statemap[str1];
     		int col = statemap[str2];
     		TranMatrix[row][col] +=1;
     	}
	}
	double* sum = new double [M];
	for (int i = 0; i < M; ++i)
	{
		sum[i] = 0.0;
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			sum[i] += TranMatrix[i][j];
		}
	}

	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			TranMatrix[i][j] = (double)TranMatrix[i][j]/sum[i];
		}
	}
	cout<<"the transfrom probality matrix is:"<<endl;
	disPlay2D(TranMatrix,M,M);
	return;
}
//求解混淆概率矩阵
void MarkModel::getConPro(const char* inputfile)
{
	string strtmp; //用于保存从语料库中读入的每一行
	ifstream infile(inputfile);
	while(getline(infile, strtmp, '\n'))
 	{
		if (strtmp.empty())
		{
			continue ;
		}
     	string pattern = " ";
     	vector<std::string> result=mmSplit(strtmp,pattern);
     	for( int i=0; i<result.size(); i++)
 	    {
			string tmp = result[i].c_str();
			if (tmp == "\n" || tmp == "\r")
			{
				continue ;
			}
			int index = tmp.find("/");
			if (index == -1)
			{
				cout <<"error"<<endl;
				break ;
			}
			string strw = tmp.substr(0, index);
			string strs = tmp.substr(index+1,tmp.length()-index-1);
			
     		int row = statemap[strs];
			int col = wordmap[strw];
     		ConMatrix[row][col] +=1;
		}
	}

	double* sum = new double [M];
	for (int i = 0; i < M; ++i)
	{
		sum[i] = 0.0;
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			sum[i] += ConMatrix[i][j];
		}
	}

	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			ConMatrix[i][j] = (double)ConMatrix[i][j]/sum[i];
		}
	}
	return ;	
}
//保存map信息
void MarkModel::saveMapData(const char* outputfile, map<string, int> MapData)
{
	ofstream outfile;
	outfile.open(outputfile);
	std::map<string,int>::iterator m_it;
	for ( m_it = MapData.begin(); m_it != MapData.end(); m_it++ )
	{
		outfile << m_it->first <<" "<< m_it->second << endl;
	}
	return ;
}
//保存HMM模型信息
void MarkModel::saveModelData(const char* outputfile)
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
			outfile << TranMatrix[i][j] << "   ";
			//cout << TranMatrix[i][j] <<"   ";
		}
		//cout <<endl;
		outfile << endl;
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			outfile << ConMatrix[i][j] << "   ";
			//cout << ConMatrix[i][j] << "   ";
		}
		//cout <<endl;
		outfile << endl;
	}
	outfile.close();
	return ;
}