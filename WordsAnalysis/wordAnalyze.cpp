#include<iostream>
#include<stdio.h>
#include <ctype.h>
#include"head.h"
using namespace std;
#pragma warning(disable:4996)

//������
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

//������λ
const char singleWord[10] = "+-*/;=(),";

//˫����λ
const char doubleWord[8] = "<>:=";



int scanTest(DUAL *dual)
{
	int flag = 0;//0��ʾΪ��������ֵ1
	int num = 0;//��Ԫ���±�

	FILE *fin;
	//char filePath[100] = "D:\\��ѧ�γ�\\����ԭ��\\text3_error.txt";//����Խ��
	//char filePath[100] = "D:\\��ѧ�γ�\\����ԭ��\\text2_error.txt";//�ַ���8λ
	//char filePath[100] = "D:\\��ѧ�γ�\\����ԭ��\\text1_correct.txt";//��ȷ����
	char filePath[100] = "D:\\��ѧ�γ�\\����ԭ��\\if.txt";
	
	
	int j = 0;//���ڴ洢�ַ�ʱ���±�
	char ch;//ÿ�ζ�ȡ���ַ�
	errno_t err;
	err = fopen_s(&fin, filePath, "r");

	if (fin == NULL) {
		printf("���ļ�����\n");
		return ++flag;
	}

	ch = getc(fin);


	int row = 1, clumn = 1;//row�кš�clomn�к�
	while (ch != EOF) {
		j = 0;
		//���д������Լ�һ�Σ�����1
		while (ch == '\n') {
			row++;
			clumn = 1;
			ch = getc(fin);
		}

		//�ո������Լӣ��в���
		while (ch == ' ' || ch == '\t') {
			clumn++;
			ch = getc(fin);
		}

		//�ؼ��ֺͱ�ʶ���ļ��
		if (isalpha(ch)) {
			dual[num].x = row;
			dual[num].y = clumn;
			dual[num].lexeme.lexeme_text[j++] = ch;
			int k = 1;
			clumn++;

			
			//��ʱ���� �ַ���8λʱ ֻʶ��ǰ8λ
			char tem[10];
			tem[0] = ch;//��һλ
			tem[8] = NULL;//ĩ��λ��
			tem[9] = NULL;

			ch = getc(fin);
			
			while (isgraph(ch) && ch != ' '&&ch != ';'&&ch != ','&&ch != '\t'&&ch != ':') {//�ж��Ƿ�Ϊ��ĸ������
				dual[num].lexeme.lexeme_text[j++] = ch;
				k++;
				
				
				if (k>1&&k <= 8) {
					tem[k-1] = ch;
				}
	
				if (ispunct(ch)) {//���ڷǷ��ַ�,ispunct����ַ��Ƿ�Ϊ����
					flag = 1;
					dual[num].type = -8;
					ch = getc(fin);
					clumn++;
					while (ch != ' ') {//�õ��ʴ��ڷǷ��ַ���
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
				dual[num].type = 11;//��ʶ����ID��

				/*if (k > 8) {
					flag = 1;
					dual[num].type = -2;//��ʾ���ʳ���
					dual[num].x = row;
					dual[num].y = clumn;
					strcpy_s(dual[num].lexeme.lexeme_text, "err_w_len");

				}*/


				if (k > 8) {
					dual[num].type = 11;//��ʶ����ID��
					dual[num].x = row;
					dual[num].y = clumn;
					//ֻȡǰ��λ
					strcpy_s(dual[num].lexeme.lexeme_text,tem);
				}



				//���ұ�����
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

		//���ֵĴ���
		else if (isdigit(ch)) {
			dual[num].lexeme.lexeme_text[0] = ch;
			j = 1;
			dual[num].x = row;
			dual[num].y = clumn++;
			ch = getc(fin);
			while (isgraph(ch) && ch != ' '&&ch != ';'&&ch != ','&&ch != '\t') {
				//ch����ĸ�������ַ��򲻰����ո񡢶��š��ֺŵı�����
				dual[num].lexeme.lexeme_text[j++] = ch;
				clumn++;
				if (!isdigit(ch)) {//���ڷǷ��ַ�
					flag = 1;
					dual[num].type = -8;
					ch = getc(fin);
					clumn++;
					while (ch != ' ') {//�õ��ʴ��ڷǷ��ַ�
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
					dual[num].type = -7;//type=-7��־���ֳ������ֵ
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

		//�����ŵļ��
		else if (strchr(singleWord, ch)) {
			int zhushi = 0;//�ж� '/'��
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
			else {//�ж�'/'����ʾ��Ϊ������ע��
				char temp = ch;
				ch = getc(fin);
				clumn++;
				if (ch == '*') {//ע��ʶ��
					char ch_1 = ch;
					ch = getc(fin);
					do {

						ch_1 = ch;
						ch = getc(fin);
						if (ch == EOF) {
							flag = 1;
							cout << "ע��ȱ���ս��*/��" << endl;
						}
					} while ((ch != '/' || ch_1 != '*') && ch != EOF);
					ch = getc(fin);
					strcpy_s(dual[num].lexeme.lexeme_text, "/**/");
					dual[num].type = 106;

					//δ��ע�͵�����ӵ����飬Ϊ�����﷨������׼��
					//������룬����Ҫ���﷨�����ദ�жϵ����Ƿ��ע�ͷ��ţ�������Ҫ����
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

					//δ��ע�͵�����ӵ����飬Ϊ�����﷨������׼��
					//������룬����Ҫ���﷨�����ദ�жϵ����Ƿ��ע�ͷ��ţ�������Ҫ����
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

		//˫���ŵļ��
		else if (strchr(doubleWord, ch) > 0) {

			dual[num].lexeme.lexeme_text[0] = ch;
			dual[num].x = row;
			dual[num].y = clumn;
			char ch_1 = ch;
			ch = getc(fin);
			clumn++;
			switch (ch_1) {

			case ':':
				if (ch == '=') {//˫�ֽ��
					dual[num].lexeme.lexeme_text[1] = ch;
					dual[num].lexeme.lexeme_text[2] = '\0';
					dual[num].type = 109;
					clumn++;
					ch = getc(fin);
				}
				else {//���ֽ��
					dual[num].lexeme.lexeme_text[1] = '\0';
					dual[num].type = 108;
				}
				break;

			case '>':
				if (ch == '=') {//˫�ֽ��
					dual[num].lexeme.lexeme_text[1] = ch;
					dual[num].lexeme.lexeme_text[2] = '\0';

					dual[num].type = 113;
					clumn++;
					ch = getc(fin);
				}
				else {//���ֽ��
					dual[num].lexeme.lexeme_text[1] = '\0';

					dual[num].type = 112;
				}
				break;
			case '<':
				if (ch == '=') {//˫�ֽ��
					dual[num].lexeme.lexeme_text[1] = ch;
					dual[num].lexeme.lexeme_text[2] = '\0';

					dual[num].type = 111;
					clumn++;
					ch = getc(fin);
				}
				else if (ch == '>') {//˫�ֽ��
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

	//������־
	dual[num + 1].type = 10086;
	output(dual);
	return flag;
}


void output(DUAL *dual)
{
	cout << "**************�ʷ��������*****************" << endl;
	cout << "����\t\t����\t\t�к�\t\t�к�" << endl;
	for (int i = 0; dual[i].type != 10086; i++) {
		//����
		if (dual[i].type == 12) {
			cout << dual[i].lexeme.lexeme_num << "\t\t" << 12 << "\t\t" << dual[i].x << "\t\t" << dual[i].y << endl ;
			
		}
		//�����ַ���
		else
		{
			
			if (dual[i].type < 0) {
				switch (dual[i].type) {
				case -8:
					cout << "----------------------------------------------------------------------------" << endl;
					cout << dual[i].lexeme.lexeme_text << "\t" << dual[i].type << "\t\t" << dual[i].x << "\t\t" << dual[i].y ;
					cout << "\tERROR:���ڷǷ��ַ�" << endl;
					cout << "----------------------------------------------------------------------------" << endl;
					break;
				case -7:
					cout << "----------------------------------------------------------------------------" << endl;
					cout << dual[i].lexeme.lexeme_text << "\t" << dual[i].type << "\t\t" << dual[i].x << "\t\t" << dual[i].y;
					cout << "\tERROR:���ֳ������ֵ" << endl;
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
	cout << "**************�ʷ��������*****************" << endl;
}