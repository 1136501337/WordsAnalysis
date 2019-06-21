#include<stdio.h>
#include<iostream>
#include"head.h"
using namespace std;
DUAL *tem;
int flag = 0;
int index = 0;

void statement();

void statementTable();



/*
���ִ�������һЩ�ַ��������ֺź�����һ��
*/
void errorDelete()
{
	int x = tem[index - 1].x;
	while (x == tem[index].x && tem[index].type != 150)
	{
		index++;
	}
}

/*
<������>��<����>|<����>,<������>
*/
void variableTable()
{
	if (tem[index].type == 11)
	{
		index++;
		if (tem[index].type == 153)
		{
			index++;
			variableTable();
		}
	}
}

/*
<����˵����>��<������>:<����>;|<������>:<����>;<����˵����>
*/
void variableInstruction()
{

	variableTable();//������
	if (tem[index].type == 108)//  :
	{
		index++;
		if (tem[index].type == 10)//  INTEGER
		{
			index++;
			if (tem[index].type == 150)//  ;
			{
				index++;
				if (tem[index].type != 2) //  begin   δ��begin����Ȼ�Ǳ�������
				{
					variableInstruction();//�ݹ����
				}

			}
			else
			{//ȱ�ٷֺ�
				cout << "ȱ��\";\"��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
				flag = 1;
			}
		}
		else
		{//ȱ�ٶ�������
			cout << "ȱ�ٶ������ͣ�λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
			flag = 1;
		}

	}
	else
	{//
		cout << "ȱ��\"��\"��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
	}
}


/*
<����˵��>��VAR<����˵����>
*/
void declarationVariable()
{
	if (tem[index].type == 4)
	{
		index++;
		variableInstruction();
	}
}


/*
<��ʽ>��<����>|<����>
*/
void factor()
{

	if (tem[index].type == 12 || tem[index].type == 11)//Ϊ��ʶ����������ʱ������һ�����ʷ���
	{
		index++;
	}
	else
	{
		cout << "��ʽ����λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		errorDelete();
		flag = 1;
	}

}

/*
<��>��<��ʽ>|<��>*<��ʽ>|<��>/<��ʽ>
*/
void term()
{
	factor();//��ʽ
	if (tem[index].type == 103 || tem[index].type == 104)//  '*' or  '/'
	{
		index++;
		factor();
	}
}

/*
<�������ʽ>��<��>|<�������ʽ>+<��>|<�������ʽ>-<��>
*/
void expression()
{
	term();//��

	if (tem[index].type == 101 || tem[index].type == 102)//  '+'  or  '-'
	{
		index++;
		term();
	}

	//���� + ���� -  ��ʱ����ζ���������ʽ����������﷨����ȱʧ '+' or  '-'  ����Ҫ����
	//���� '+'  or  '-'  ʱ��Ӧ����  ';'�����������
	//����ǹ�ϵ�������������������ʽ��������
	//����� DO  THEN  Ҳ������
	if (tem[index].type != 150 && !(tem[index].type >= 110 && tem[index].type <= 115) &&
		tem[index].type != 9 && tem[index].type != 6)
	{
		flag = 1;
		if (tem[index].x == tem[index - 1].x)
		{
			cout << "ȱ��\'+\' or \'-\'��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		}
		else
		{
			cout << "ȱ��\'+\' or \'-\'��λ�ã�[" << tem[index - 1].x << "," << tem[index - 1].y << "];" << endl;
		}
		errorDelete();
	}
}



/*
<��ֵ���>��<����>:=<�������ʽ>;
*/
void assignmentStatement()
{
	if (tem[index].type == 11)//��ʶ��
	{
		index++;
		if (tem[index].type == 109)//  :=
		{
			index++;
			expression();//�������ʽ
			if (tem[index].type == 150)//	;
			{
				index++;
			}
			else
			{
				flag = 1;
				if (tem[index].x == tem[index - 1].x)
					cout << "ȱ��\";\"��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
				else
					cout << "ȱ��\";\"��λ�ã�[" << tem[index - 1].x << "," << tem[index - 1].y << "];" << endl;
				//ȱ�ٷֺţ�����Ŀǰλ������ַ���һ���ַ��Ƿ���ͬһ�У�
				//����ڣ��������һ���ֺŻ�����һ�м������������������ж�
				//����ͬһ������Ҫ����
				if (tem[index].x == tem[index - 1].x)
				{//��ͬһ��
					errorDelete();
				}

			}
		}
		else
		{
			//��ֵ������
			flag = 1;
			cout << "��ֵ������λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
			//���ִ�����Ҫ�Ѹ����ȥ��������һ����䣬������һ���ֺŻ�����һ��
			if (tem[index].x == tem[index - 1].x)
			{//��ͬһ��
				errorDelete();
			}
			if (tem[index].type == 150)
				index++;
		}
	}


}


/*
<��ϵ���ʽ>��<�������ʽ><��ϵ��><�������ʽ>
*/
void relationalExpression()
{

	expression();
	if (tem[index].type >= 110 && tem[index].type <= 115)
	{
		index++;
		expression();
	}
	else
	{
		cout << "��ϵ���ʽ����λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		errorDelete();
		flag = 1;
	}
}
/*
<�������>��IF<��ϵ���ʽ>THEN<���>ELSE<���>
*/
void conditionalStatements()
{
	if (tem[index].type == 5)//  IF,   �������IF����ص�statement();
	{
		index++;
		relationalExpression();//��ϵ���ʽ
		if (tem[index].type == 6)//  THEN
		{
			index++;
			statement();//���

			if (tem[index].type == 7)//ELSE
			{
				index++;
				statement();
			}
			else
			{
				cout << "ȱ��\"ELSE\"��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
				flag = 1;
				statement();
			}
		}
		else
		{//ȱ��THEN
			cout << "ȱ��\"THEN\"��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
			flag = 1;
			statement();

			if (tem[index].type == 6)//ELSE
			{
				index++;
				statement();
			}
			else
			{
				cout << "ȱ��\"ELSE\"��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
				flag = 1;
				statement();
			}
		}

	}
}

/*
<�������>��BEGIN<����>END
*/
void complexStatements()
{
	if (tem[index].type == 2)//   begin
	{
		index++;
		statementTable();
		if (tem[index].type == 3)
		{
			index++;
			//���������������
		}
		else
		{//ȱ��end
			flag = 1;
			cout << "ȱ��\"end\"������־��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		}
	}

}

/*
<WHILE���>��WHILE<��ϵ���ʽ>DO<���>
*/
void while_statement()
{
	if (tem[index].type == 8)//   while
	{
		index++;
		relationalExpression();//��ϵ���ʽ
		if (tem[index].type == 9)//do
		{
			index++;
			statement();
		}
		else
		{//ȱ��do
			flag = 1;
			cout << "ȱ��\"end\"������־��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
			statement();
		}
	}
}

/*
���,�������֣�һ�ָ�ֵ��䣬һ��������䣬һ��ѭ����䣬����һ�ָ������
<���>��<��ֵ���>|<�������>|<WHILE���>|<�������>
*/
void statement()
{
	while_statement();//ѭ�����
	complexStatements();//�������
	conditionalStatements();//�������
	assignmentStatement();//��ֵ��䣬��������������
}


/*
<����>��<���>; | <���>;<����>
*/
void statementTable()
{
	statement();//���
	if (tem[index].type != 3 && tem[index].type != 0)//  ����end����������,=0ʱ��ʾ�Ѿ��������Ե��ʣ�
	{
		statementTable();
	}

}

/*
<�ֳ���>��<����˵��>BEGIN<����>END
*/
void lump()
{

	declarationVariable();//����˵������

	if (tem[index].type == 2)//begin
	{
		index++;

		statementTable();//����

		if (tem[index].type == 3)//end
		{
			//�������
		}
		else
		{//ȱ��end������־
			cout << "ȱ��\"end\"������־��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
			flag = 1;
		}
	}
	else
	{//ȱ�� begin
		cout << "ȱ��\"begin\"��ʼ��־��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
		statementTable();//����

		if (tem[index].type == 2)//end
		{
			//�������
		}
		else
		{//ȱ��end������־
			cout << "ȱ��\"end\"������־��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
			flag = 1;
		}
	}
}

void checkSemi()//�ж��Ƿ�ȱʧ ";"
{

	if (tem[index].type == 150) {//�ֺ�
		index++;
		lump();
	}
	else
	{//ȱ�ٷֺ�  ��
		cout << "ȱ��\";\"��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
		lump();
	}
}

void checkFileName()//�ж��Ƿ�ȱʧ�ļ���
{
	if (tem[index].type == 11 && tem[index + 1].type == 150) {//����program������Ҫ���ļ���
		index++;
		checkSemi();
	}
	else
	{//ȱ���ļ���
		cout << "ȱ���ļ�����λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
		checkSemi();
	}
}

/*
<����>��PROGRAM<��ʶ��>;<�ֳ���>
*/
void syntaxAnalysis()
{
	if (tem[index].type == 1)
	{//program   ����ʼ
		index++;
		checkFileName();
	}
	else
	{
		//����ʼȱ��"program"��ʼ��־
		cout << "ȱ��\"program\"��ʼ��־��λ�ã�[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
		checkFileName();
	}
}


void grammerAnalyze(DUAL *dual)
{
	cout << endl << endl << endl;
	cout << "********************�﷨�������**********************" << endl;
	tem = dual;
	syntaxAnalysis();
	cout << endl;
	if (flag == 0) {
		cout << "�﷨��ȷ" << endl;
	}
	else
	{
		cout << "�﷨���ڴ���" << endl;
	}
	cout << "********************�﷨�������**********************" << endl;

}