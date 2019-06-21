//头文件只被编译一次
#pragma once


struct DUAL
{
	int type;//类别码
	union {
		char lexeme_text[10];//存放字符串类型
		int lexeme_num;//存放整数类型
	}lexeme;
	int x;//行号
	int y;//列号
};


int scanTest(DUAL *dual);

void output(DUAL *dual);

void grammerAnalyze(DUAL *dual);
