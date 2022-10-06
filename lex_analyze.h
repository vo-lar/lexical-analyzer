#include <fstream>
#include <iostream>
#include <cstring>
#define   MAX 22             /*分析表的最大容量*/
#define OP_MAX 34

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
	char type[15];
};
/*1-3为其他符号，从4至29为op，30+为关键字*/
const oplist op[OP_MAX] =
{

	{"(",4,"界符/左括号"},
	{")",5,"界符/右括号"},
	{"[",6,"界符/中括号"},
	{"]",7,"界符/中括号"},
	{"{",8,"界符/大括号"},
	{"}",9,"界符/大括号"},
	{";",10,"界符"},
	{":",11,"界符"},
	{",",12,"界符/分隔符"},
	{".",13,"界符"},
	{"+",14,"算符"},
	{"-",15,"算符"},
	{"*",16,"算符"},
	{"/",17,"算符"},
	{"=",18,"赋值号"},
	{">",19,"算符"},
	{">=",20,"算符"},
	{"<",21,"算符"},
	{"<=",22,"算符"},
	{"#",23,"结束符/注释符"},
	{"!=",24,"算符"},
	{"<<",25,"运算符"},
	{"\"",26,"界符"},
	{"\'",27,"界符"},
	{"!",28,"算符"},
	{"==",29,"算符"},
	{"||",30,"算符"},
	{"|",31,"算符"},
	{"&",32,"算符"},
	{"&&",33,"算符"},
	{"<>",34,"其他"},
	{"//",35,"界符"},
	{"/*",36,"界符"},
	{"*/",37,"界符"}
};

/*tool*/
void init();
void text_analyse(std::string input, std::string output);
int Iskeyword(node keyword[], char* is_res);
bool IsLetter(char c);
bool IsDigit(char c);
bool IsPoint(char c);
