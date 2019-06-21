#include<iostream>
#include<stdio.h>
#include <ctype.h>
#include"head.h"
using namespace std;
#pragma warning(disable:4996)

//保留字
char keyword[11][8] = {
	"PROGRAM",
	"BEGIN",
	"END",
	"VAR",
	"IF",
	"THEN",
	"ELSE",
	"WHILE",
	"DO",
	"INTEGER" };

//单符号位
const char singleWord[10] = "+-*/;=(),";

//双符号位
const char doubleWord[8] = "<>:=";



int scanTest(DUAL *dual)
{
	int flag = 0;//0表示为出错，出错赋值1
	int num = 0;//二元组下标

	FILE *fin;
	//char filePath[100] = "D:\\大学课程\\编译原理\\text3_error.txt";//常数越界
	//char filePath[100] = "D:\\大学课程\\编译原理\\text2_error.txt";//字符超8位
	//char filePath[100] = "D:\\大学课程\\编译原理\\text1_correct.txt";//正确用例
	char filePath[100] = "D:\\大学课程\\编译原理\\if.txt";
	
	
	int j = 0;//用于存储字符时的下标
	char ch;//每次读取的字符
	errno_t err;
	err = fopen_s(&fin, filePath, "r");

	if (fin == NULL) {
		printf("打开文件出错\n");
		return ++flag;
	}

	ch = getc(fin);


	int row = 1, clumn = 1;//row行号。clomn列号
	while (ch != EOF) {
		j = 0;
		//换行处理，行自加一次，列置1
		while (ch == '\n') {
			row++;
			clumn = 1;
			ch = getc(fin);
		}

		//空格处理，列自加，行不变
		while (ch == ' ' || ch == '\t') {
			clumn++;
			ch = getc(fin);
		}

		//关键字和标识符的检查
		if (isalpha(ch)) {
			dual[num].x = row;
			dual[num].y = clumn;
			dual[num].lexeme.lexeme_text[j++] = ch;
			int k = 1;
			clumn++;

			
			//临时数组 字符超8位时 只识别前8位
			char tem[10];
			tem[0] = ch;//第一位
			tem[8] = NULL;//末两位空
			tem[9] = NULL;

			ch = getc(fin);
			
			while (isgraph(ch) && ch != ' '&&ch != ';'&&ch != ','&&ch != '\t'&&ch != ':') {//判断是否为字母或数字
				dual[num].lexeme.lexeme_text[j++] = ch;
				k++;
				
				
				if (k>1&&k <= 8) {
					tem[k-1] = ch;
				}
	
				if (ispunct(ch)) {//存在非法字符,ispunct检查字符是否为标点符
					flag = 1;
					dual[num].type = -8;
					ch = getc(fin);
					clumn++;
					while (ch != ' ') {//该单词存在非法字符，
						ch = getc(fin);
						clumn++;
					}
					break;
				}
				ch = getc(fin);
				clumn++;
			}
			dual[num].lexeme.lexeme_text[j] = '\0';

			if (dual[num].type != -8) {
				dual[num].type = 11;//标识符，ID，

				/*if (k > 8) {
					flag = 1;
					dual[num].type = -2;//表示单词超长
					dual[num].x = row;
					dual[num].y = clumn;
					strcpy_s(dual[num].lexeme.lexeme_text, "err_w_len");

				}*/


				if (k > 8) {
					dual[num].type = 11;//标识符，ID，
					dual[num].x = row;
					dual[num].y = clumn;
					//只取前八位
					strcpy_s(dual[num].lexeme.lexeme_text,tem);
				}



				//查找保留字
				for (int asd = 0; asd < 11; asd++) {
					if (strcmp(dual[num].lexeme.lexeme_text, keyword[asd]) == 0) {
						dual[num].type = asd + 1;
						break;
					}
				}

			}
			else {
				strcpy_s(dual[num].lexeme.lexeme_text, "error");

			}
			num++;
		}

		//数字的处理
		else if (isdigit(ch)) {
			dual[num].lexeme.lexeme_text[0] = ch;
			j = 1;
			dual[num].x = row;
			dual[num].y = clumn++;
			ch = getc(fin);
			while (isgraph(ch) && ch != ' '&&ch != ';'&&ch != ','&&ch != '\t') {
				//ch是字母或数字字符或不包含空格、逗号、分号的标点符号
				dual[num].lexeme.lexeme_text[j++] = ch;
				clumn++;
				if (!isdigit(ch)) {//存在非法字符
					flag = 1;
					dual[num].type = -8;
					ch = getc(fin);
					clumn++;
					while (ch != ' ') {//该单词存在非法字符
						ch = getc(fin);
						clumn++;
					}
					break;
				}
				ch = getc(fin);
			}
			if (dual[num].type != -8 || j == 1) {
				dual[num].lexeme.lexeme_num = atoi(dual[num].lexeme.lexeme_text);
				if (dual[num].lexeme.lexeme_num > 65535) {
					flag = 1;
					dual[num].type = -7;//type=-7标志数字超过最大值
					strcpy_s(dual[num].lexeme.lexeme_text, "err_d_len");
				}
				else
					dual[num].type = 12;
			}
			else {
				flag = 1;
				strcpy_s(dual[num].lexeme.lexeme_text, "error");
			}


			num++;
		}

		//单符号的检查
		else if (strchr(singleWord, ch)) {
			int zhushi = 0;//判断 '/'，
			switch (ch)
			{
			case '+':
				dual[num].type = 101;
				break;
			case '-':
				dual[num].type = 102;
				break;
			case '*':
				dual[num].type = 103;
				break;
			case '/':
				zhushi = 1;
				break;
			case ';':
				dual[num].type = 150;
				break;
			case '=':
				dual[num].type = 115;
				break;
			case '(':
				dual[num].type = 151;
				break;
			case ')':
				dual[num].type = 152;
				break;
			case ',':
				dual[num].type = 153;
				break;

			default:
				break;
			}
			if (zhushi == 0) {
				dual[num].lexeme.lexeme_text[0] = ch;
				dual[num].lexeme.lexeme_text[1] = '\0';
				dual[num].x = row;
				dual[num].y = clumn;
				ch = getc(fin);
			}
			else {//判断'/'所表示的为除还是注释
				char temp = ch;
				ch = getc(fin);
				clumn++;
				if (ch == '*') {//注释识别
					char ch_1 = ch;
					ch = getc(fin);
					do {

						ch_1 = ch;
						ch = getc(fin);
						if (ch == EOF) {
							flag = 1;
							cout << "注释缺少终结符*/，" << endl;
						}
					} while ((ch != '/' || ch_1 != '*') && ch != EOF);
					ch = getc(fin);
					strcpy_s(dual[num].lexeme.lexeme_text, "/**/");
					dual[num].type = 106;

					//未把注释单词添加到数组，为后续语法分析做准备
					//如果加入，则需要在语法分析多处判断单词是否否注释符号，是则需要跳过
					num--;
				}
				else if (ch == '/') {
					ch = getc(fin);
					while (ch != '\n') {
						ch = getc(fin);
						clumn++;
					}
					strcpy_s(dual[num].lexeme.lexeme_text, "//");
					dual[num].type = 105;

					//未把注释单词添加到数组，为后续语法分析做准备
					//如果加入，则需要在语法分析多处判断单词是否否注释符号，是则需要跳过
					num--;

				}
				else {
					dual[num].type = 104;
					dual[num].lexeme.lexeme_text[0] = temp;
					dual[num].lexeme.lexeme_text[1] = '\0';
					dual[num].x = row;
					dual[num].y = clumn;
				}
			}
			num++;
		}

		//双符号的检查
		else if (strchr(doubleWord, ch) > 0) {

			dual[num].lexeme.lexeme_text[0] = ch;
			dual[num].x = row;
			dual[num].y = clumn;
			char ch_1 = ch;
			ch = getc(fin);
			clumn++;
			switch (ch_1) {

			case ':':
				if (ch == '=') {//双分界符
					dual[num].lexeme.lexeme_text[1] = ch;
					dual[num].lexeme.lexeme_text[2] = '\0';
					dual[num].type = 109;
					clumn++;
					ch = getc(fin);
				}
				else {//单分界符
					dual[num].lexeme.lexeme_text[1] = '\0';
					dual[num].type = 108;
				}
				break;

			case '>':
				if (ch == '=') {//双分界符
					dual[num].lexeme.lexeme_text[1] = ch;
					dual[num].lexeme.lexeme_text[2] = '\0';

					dual[num].type = 113;
					clumn++;
					ch = getc(fin);
				}
				else {//单分界符
					dual[num].lexeme.lexeme_text[1] = '\0';

					dual[num].type = 112;
				}
				break;
			case '<':
				if (ch == '=') {//双分界符
					dual[num].lexeme.lexeme_text[1] = ch;
					dual[num].lexeme.lexeme_text[2] = '\0';

					dual[num].type = 111;
					clumn++;
					ch = getc(fin);
				}
				else if (ch == '>') {//双分界符
					dual[num].lexeme.lexeme_text[1] = ch;
					dual[num].lexeme.lexeme_text[2] = '\0';

					dual[num].type = 114;
					clumn++;
					ch = getc(fin);

				}
				else {
					dual[num].lexeme.lexeme_text[1] = '\0';

					dual[num].type = 110;
				}
				break;
			}
			num++;

		}
	}

	//结束标志
	dual[num + 1].type = 10086;
	output(dual);
	return flag;
}


void output(DUAL *dual)
{
	cout << "**************词法分析结果*****************" << endl;
	cout << "单词\t\t编码\t\t行号\t\t列号" << endl;
	for (int i = 0; dual[i].type != 10086; i++) {
		//整数
		if (dual[i].type == 12) {
			cout << dual[i].lexeme.lexeme_num << "\t\t" << 12 << "\t\t" << dual[i].x << "\t\t" << dual[i].y << endl ;
			
		}
		//其他字符串
		else
		{
			
			if (dual[i].type < 0) {
				switch (dual[i].type) {
				case -8:
					cout << "----------------------------------------------------------------------------" << endl;
					cout << dual[i].lexeme.lexeme_text << "\t" << dual[i].type << "\t\t" << dual[i].x << "\t\t" << dual[i].y ;
					cout << "\tERROR:存在非法字符" << endl;
					cout << "----------------------------------------------------------------------------" << endl;
					break;
				case -7:
					cout << "----------------------------------------------------------------------------" << endl;
					cout << dual[i].lexeme.lexeme_text << "\t" << dual[i].type << "\t\t" << dual[i].x << "\t\t" << dual[i].y;
					cout << "\tERROR:数字超过最大值" << endl;
					cout << "----------------------------------------------------------------------------" << endl;
					break;
				default:
					break;
				}
			}
			else {
				cout << dual[i].lexeme.lexeme_text << "\t\t" << dual[i].type << "\t\t" << dual[i].x << "\t\t" << dual[i].y << endl;
			}
		}
	}
	cout << "**************词法分析结果*****************" << endl;
}