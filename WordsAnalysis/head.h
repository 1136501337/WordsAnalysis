//ͷ�ļ�ֻ������һ��
#pragma once


struct DUAL
{
	int type;//�����
	union {
		char lexeme_text[10];//����ַ�������
		int lexeme_num;//�����������
	}lexeme;
	int x;//�к�
	int y;//�к�
};


int scanTest(DUAL *dual);

void output(DUAL *dual);

void grammerAnalyze(DUAL *dual);
