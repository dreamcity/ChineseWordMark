// ChineseWordMark.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CWMHMM.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	const char* inputdata = "../iodata/datatest.txt";
	const char* segresult = "../iodata/segresult.txt";
	const char* markresult = "../iodata/markresult.txt";
	CWMHMM CWM;
	string splitHMM = CWM.getMarkHMMfile();
	string markHMM = CWM.getSplitHMMfile();
	if(splitHMM.empty())
	{
		CWM.getSplitHMMModel();
	}
	if(markHMM.empty())
	{
		CWM.getMarkHMMModel();
	}
	CWM.splitStentes(inputdata, segresult);
	CWM.markWords(segresult, markresult);
	return 0;
}

