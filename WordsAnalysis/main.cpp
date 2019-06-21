#include<stdio.h>
#include<iostream>
using namespace std;
#include"head.h"


DUAL dual[200];


int main()
{

	int flag;
	flag = scanTest(dual);


	if (flag == 0) {
		grammerAnalyze(dual);
	}
	else
	{
		cout << endl << "词法分析出现错误，不进行语法分析" << endl;
	}

	getchar();
	getchar();
	return 0;
}