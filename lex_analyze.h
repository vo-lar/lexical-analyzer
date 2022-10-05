#include <fstream>
#include <iostream>
#include <cstring>
#define   MAX 22             /*��������������*/


struct node {
	char lexptr[30];
	int token;
};

const char key[MAX][10] = {
	"int",
	"char",
	"float",
	"main",
	"double",
	"case",
	"for",
	"if",
	"auto",
	"else",
	"do",
	"while",
	"void",
	"static",
	"return",
	"break",
	"struct",
	"const",
	"union",
	"switch",
	"typedef",
	"enum"
};

struct oplist {
	char myoperator[3];
	int number;
	char type[10];
};
/*1-3Ϊ�������ţ���4��25Ϊop��26-30������30+Ϊ�ؼ���*/
const oplist op[MAX] =
{
	{"(",4,"�ֽ��"},
	{")",5,"�ֽ��"},
	{"[",6,"�ֽ��"},
	{"]",7,"�ֽ��"},
	{"{",8,"�ֽ��"},
	{"}",9,"�ֽ��"},
	{";",10,"�ֽ��"},
	{":",11,"�ֽ��"},
	{",",12,"�ֽ��"},
	{".",13,"�ֽ��"},
	{"+",14,"�����"},
	{"-",15,"�����"},
	{"*",16,"�����"},
	{"/",17,"�����"},
	{"=",18,"�����"},
	{">",19,"�����"},
	{">=",20,"�����"},
	{"<",21,"�����"},
	{"<=",22,"�����"},
	{"==",23,"�����"},
	{"<>",24,"����"},
	{"!",25,"�����"}
};

/*tool*/
void init();
void text_analyse(std::string input, std::string output);
int Iskeyword(node keyword[], char* is_res);
bool IsLetter(char c);
bool IsDigit(char c);