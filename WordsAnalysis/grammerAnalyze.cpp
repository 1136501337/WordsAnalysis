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
出现错误，摒弃一些字符，读到分号后者下一行
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
<变量表>→<变量>|<变量>,<变量表>
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
<变量说明表>→<变量表>:<类型>;|<变量表>:<类型>;<变量说明表>
*/
void variableInstruction()
{

	variableTable();//变量表
	if (tem[index].type == 108)//  :
	{
		index++;
		if (tem[index].type == 10)//  INTEGER
		{
			index++;
			if (tem[index].type == 150)//  ;
			{
				index++;
				if (tem[index].type != 2) //  begin   未到begin，依然是变量声明
				{
					variableInstruction();//递归调用
				}

			}
			else
			{//缺少分号
				cout << "缺少\";\"，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
				flag = 1;
			}
		}
		else
		{//缺少定义类型
			cout << "缺少定义类型，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
			flag = 1;
		}

	}
	else
	{//
		cout << "缺少\"：\"，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
	}
}


/*
<变量说明>→VAR<变量说明表>
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
<因式>→<变量>|<常数>
*/
void factor()
{

	if (tem[index].type == 12 || tem[index].type == 11)//为标识符或整常数时，读下一个单词符号
	{
		index++;
	}
	else
	{
		cout << "因式错误，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		errorDelete();
		flag = 1;
	}

}

/*
<项>→<因式>|<项>*<因式>|<项>/<因式>
*/
void term()
{
	factor();//因式
	if (tem[index].type == 103 || tem[index].type == 104)//  '*' or  '/'
	{
		index++;
		factor();
	}
}

/*
<算术表达式>→<项>|<算术表达式>+<项>|<算术表达式>-<项>
*/
void expression()
{
	term();//项

	if (tem[index].type == 101 || tem[index].type == 102)//  '+'  or  '-'
	{
		index++;
		term();
	}

	//不是 + 或者 -  的时候意味着算术表达式结束，如果语法错误缺失 '+' or  '-'  则需要报错，
	//不是 '+'  or  '-'  时，应该是  ';'代表该语句结束
	//如果是关系运算符，后面跟算术表达式，不出错
	//如果是 DO  THEN  也不出错
	if (tem[index].type != 150 && !(tem[index].type >= 110 && tem[index].type <= 115) &&
		tem[index].type != 9 && tem[index].type != 6)
	{
		flag = 1;
		if (tem[index].x == tem[index - 1].x)
		{
			cout << "缺少\'+\' or \'-\'，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		}
		else
		{
			cout << "缺少\'+\' or \'-\'，位置：[" << tem[index - 1].x << "," << tem[index - 1].y << "];" << endl;
		}
		errorDelete();
	}
}



/*
<赋值语句>→<变量>:=<算术表达式>;
*/
void assignmentStatement()
{
	if (tem[index].type == 11)//标识符
	{
		index++;
		if (tem[index].type == 109)//  :=
		{
			index++;
			expression();//算术表达式
			if (tem[index].type == 150)//	;
			{
				index++;
			}
			else
			{
				flag = 1;
				if (tem[index].x == tem[index - 1].x)
					cout << "缺少\";\"，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
				else
					cout << "缺少\";\"，位置：[" << tem[index - 1].x << "," << tem[index - 1].y << "];" << endl;
				//缺少分号，看看目前位置与该字符上一个字符是否处在同一行，
				//如果在，则读到下一个分号或者下一行继续，如果不在则继续判断
				//不再同一行则不需要摒弃
				if (tem[index].x == tem[index - 1].x)
				{//在同一行
					errorDelete();
				}

			}
		}
		else
		{
			//赋值语句错误
			flag = 1;
			cout << "赋值语句错误，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
			//出现错误，需要把该语句去除，看下一条语句，读到下一个分号或者下一行
			if (tem[index].x == tem[index - 1].x)
			{//在同一行
				errorDelete();
			}
			if (tem[index].type == 150)
				index++;
		}
	}


}


/*
<关系表达式>→<算术表达式><关系符><算术表达式>
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
		cout << "关系表达式错误，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		errorDelete();
		flag = 1;
	}
}
/*
<条件语句>→IF<关系表达式>THEN<语句>ELSE<语句>
*/
void conditionalStatements()
{
	if (tem[index].type == 5)//  IF,   如果不是IF，则回到statement();
	{
		index++;
		relationalExpression();//关系表达式
		if (tem[index].type == 6)//  THEN
		{
			index++;
			statement();//语句

			if (tem[index].type == 7)//ELSE
			{
				index++;
				statement();
			}
			else
			{
				cout << "缺少\"ELSE\"，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
				flag = 1;
				statement();
			}
		}
		else
		{//缺少THEN
			cout << "缺少\"THEN\"，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
			flag = 1;
			statement();

			if (tem[index].type == 6)//ELSE
			{
				index++;
				statement();
			}
			else
			{
				cout << "缺少\"ELSE\"，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
				flag = 1;
				statement();
			}
		}

	}
}

/*
<复合语句>→BEGIN<语句表>END
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
			//复合语句正常结束
		}
		else
		{//缺少end
			flag = 1;
			cout << "缺少\"end\"结束标志，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		}
	}

}

/*
<WHILE语句>→WHILE<关系表达式>DO<语句>
*/
void while_statement()
{
	if (tem[index].type == 8)//   while
	{
		index++;
		relationalExpression();//关系表达式
		if (tem[index].type == 9)//do
		{
			index++;
			statement();
		}
		else
		{//缺少do
			flag = 1;
			cout << "缺少\"end\"结束标志，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
			statement();
		}
	}
}

/*
语句,语句分四种，一种赋值语句，一种条件语句，一种循环语句，还有一种复合语句
<语句>→<赋值语句>|<条件语句>|<WHILE语句>|<复合语句>
*/
void statement()
{
	while_statement();//循环语句
	complexStatements();//复合语句
	conditionalStatements();//条件语句
	assignmentStatement();//赋值语句，放在三个语句最后
}


/*
<语句表>→<语句>; | <语句>;<语句表>
*/
void statementTable()
{
	statement();//语句
	if (tem[index].type != 3 && tem[index].type != 0)//  不是end，还是语句表,=0时表示已经读完所以单词，
	{
		statementTable();
	}

}

/*
<分程序>→<变量说明>BEGIN<语句表>END
*/
void lump()
{

	declarationVariable();//变量说明分析

	if (tem[index].type == 2)//begin
	{
		index++;

		statementTable();//语句表

		if (tem[index].type == 3)//end
		{
			//程序结束
		}
		else
		{//缺少end结束标志
			cout << "缺少\"end\"结束标志，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
			flag = 1;
		}
	}
	else
	{//缺少 begin
		cout << "缺少\"begin\"开始标志，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
		statementTable();//语句表

		if (tem[index].type == 2)//end
		{
			//程序结束
		}
		else
		{//缺少end结束标志
			cout << "缺少\"end\"结束标志，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
			flag = 1;
		}
	}
}

void checkSemi()//判断是否缺失 ";"
{

	if (tem[index].type == 150) {//分号
		index++;
		lump();
	}
	else
	{//缺少分号  ；
		cout << "缺少\";\"，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
		lump();
	}
}

void checkFileName()//判断是否缺失文件名
{
	if (tem[index].type == 11 && tem[index + 1].type == 150) {//跟在program后面需要有文件名
		index++;
		checkSemi();
	}
	else
	{//缺少文件名
		cout << "缺少文件名，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
		checkSemi();
	}
}

/*
<程序>→PROGRAM<标识符>;<分程序>
*/
void syntaxAnalysis()
{
	if (tem[index].type == 1)
	{//program   程序开始
		index++;
		checkFileName();
	}
	else
	{
		//程序开始缺少"program"开始标志
		cout << "缺少\"program\"开始标志，位置：[" << tem[index].x << "," << tem[index].y << "];" << endl;
		flag = 1;
		checkFileName();
	}
}


void grammerAnalyze(DUAL *dual)
{
	cout << endl << endl << endl;
	cout << "********************语法分析结果**********************" << endl;
	tem = dual;
	syntaxAnalysis();
	cout << endl;
	if (flag == 0) {
		cout << "语法正确" << endl;
	}
	else
	{
		cout << "语法存在错误" << endl;
	}
	cout << "********************语法分析结果**********************" << endl;

}