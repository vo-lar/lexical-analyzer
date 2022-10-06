#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "lex_analyze.h"


#define   MAXBUF    100

using namespace std;

struct node keywords[MAX];

/*最小的token是OP_MAX + 3 + 1*/
void init()
{
    int j;
    for (j = 0; j < MAX; j++) {
        strcpy(keywords[j].lexptr, key[j]);
        keywords[j].token = j + OP_MAX + 3 + 1;
    }
}

/***************对关键字进行搜索**************/
int Iskeyword(node keyword[], char* ch) {
    int i;
    for (i = 0; i < MAX; i++) {
        if ((strcmp(keyword[i].lexptr, ch)) == 0)
            break;
    }
    if (i < MAX)
        return  keyword[i].token;
    else
        return 0;
}

/*****************判断是否为字母*****************/
bool IsLetter(char c)
{
    if ((c <= 'z') && (c >= 'a') || (c <= 'Z') && (c >= 'A'))
        return 1;
    else
        return 0;
}

/*************判断是否为数字**************/
bool IsDigit(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}
/*************判断是否为点**************/
bool IsPoint(char c)
{
    if (c == '.')
        return true;
    else
        return false;
}
/*************判断注释符是否结束**************/
void check_notes(fstream &fin, fstream &fout)
{
    char ch;
    while (1)
    {
        ch = fin.get();
        if (ch == '*')
        {
            ch = fin.get();
            if (ch == '\/')
            {
                fout << "*/" << "\t\t" << 36 << "\t\t" << "界符" << endl;
                break;
            }
        }
        else if (ch == EOF)
        {
            cout << "wrong! 多行注释没有结束！" << endl;
            fout << "wrong! 多行注释没有结束！程序关闭" << endl;
            break;
        }
        else
            continue;
    }
}

/*判断是否为单个运算符*/
string IsOp(char ch, int Line_No) {
    stringstream ss;
    int i = 0;
    for (; i < OP_MAX; i++) {

        if (op[i].myoperator[0] == ch)
            break;
    }
    if (i < OP_MAX) {
        ss << op[i].myoperator << "\t\t" << op[i].number << "\t\t" << op[i].type << endl;
    }
    else {
        ss << "在第" << Line_No << "行无法识别的字符\t" << ch << endl << endl;
    }
    return ss.str();
}


/***************分析程序**************/
void text_analyse(string input, string output) {

    fstream fin, fout;
    fin.open(input.c_str(), ios::in | ios::binary);
    fout.open(output.c_str(), ios::out | ios::binary);
    if (!fin.is_open()) {
        cout << "无法打开需要读取的文件" << endl;
        return;
    }
    string str;
    char arr[MAXBUF];
    int j = 0;
    char ch = ' ';             /*存放读入当前的输入字符*/
    int Line_NO = 0;                /*纪录行号*/
    while ((ch = (fin).get()) != EOF) {
        /*空格、tab*/
        if (ch == ' ' || ch == '\t' || ch == '\r') {
            continue;
        }
        else if (ch == '\n') { Line_NO++; }
        /*字符串*/
        else if (IsLetter(ch)) {
            while (IsLetter(ch) | IsDigit(ch) | (ch == '_')) {
                if ((ch <= 'Z') && (ch >= 'A'))
                    ch = ch + 32;
                arr[j] = ch;
                j++;
                ch = (fin).get();
            }
            fin.seekg(-1, ios::cur);
            arr[j] = '\0';
            j = 0;
            if (Iskeyword(keywords, arr)) {
                fout << arr << "\t\t" << Iskeyword(keywords, arr) << "\t\t" << "关键字" << endl;
            }
            else
                fout << arr << "\t\t" << 1 << "\t\t" << "标识符" << endl;
        }
        /*数字*/
        else if (IsDigit(ch)) {
            int s = 0;
            int Point_number = 0;
            while (IsDigit(ch) | IsLetter(ch) | IsPoint(ch)) {
                if (IsLetter(ch)) {
                    arr[j] = ch;
                    j++;
                    ch = (fin).get();
                    s = 1;
                }
                else if (IsDigit(ch)) {
                    arr[j] = ch;
                    j++;
                    ch = (fin).get();
                }
                else if (IsPoint(ch))
                {
                    ++Point_number;
                    arr[j] = ch;
                    j++;
                    ch = (fin).get();
                    if (Point_number == 1)
                        s = 2;
                    else
                    {
                        s = 1;
                    }
                }
            }
            (fin).seekg(-1, ios::cur);
            arr[j] = '\0';
            j = 0;
            if (s == 0)
                fout << arr << "\t\t" << 2 << "\t\t" << "无符号整数" << endl;
            else if (s == 1)
                fout << arr << "\t\t" << 3 << "\t\t" << "无法试别" << endl;
            else if (s == 2)
                fout << arr << "\t\t" << 2 << "\t\t" << "浮点数" << endl;
        }
        else {
            /*>,>=*/
            if (ch == '>') {
                ch = fin.get();
                if (ch == '=')
                    fout << ">=" << "\t\t" << 20 << "\t\t" << "算符" << endl;
                else {
                    fout << ">" << "\t\t" << 19 << "\t\t" << "算符" << endl;
                    fin.seekg(-1, ios::cur);
                }
            }
            /*<,<=,<>，<<*/
            else if (ch == '<') {
                ch = (fin).get();
                if (ch == '=')
                    fout << "<=" << "\t\t" << 22 << "\t\t" << "算符" << endl;
                else if (ch == '>')
                    fout << "<>" << "\t\t" << 24 << "\t\t" << "其他" << endl;
                else if (ch == '<') {
                    fout << "<<" << "\t\t" << 25 << "\t\t" << "算符" << endl;
                }
                else {
                    fout << "<" << "\t\t" << 21 << "\t\t" << "算符" << endl;
                    (fin).seekg(-1, ios::cur);
                }
            }
            /*=,==*/
            else if (ch == '=') {
                ch = (fin).get();
                if (ch == '=')
                    fout << "==" << "\t\t" << 29 << "\t\t" << "算符" << endl;
                else {
                    fout << "=" << "\t\t" << 18 << "\t\t" << "算符" << endl;
                    (fin).seekg(-1, ios::cur);
                }
            }
            /*!,!=*/
            else if (ch == '!') {
                ch = (fin).get();
                if (ch == '=')
                    fout << "!=" << "\t\t" << 24 << "\t\t" << "算符" << endl;
                else {
                    fout << "!" << "\t\t" << 28 << "\t\t" << "算符" << endl;
                    (fin).seekg(-1, ios::cur);
                }
            }
            /*||,|*/
            else if (ch == '|') {
                ch = (fin).get();
                if (ch == '|')
                    fout << "||" << "\t\t" << 30 << "\t\t" << "算符" << endl;
                else {
                    fout << "|" << "\t\t" << 31 << "\t\t" << "算符" << endl;
                    (fin).seekg(-1, ios::cur);
                }
            }
            /*||,|*/
            else if (ch == '&') {
                ch = (fin).get();
                if (ch == '&')
                    fout << "&&" << "\t\t" << 32 << "\t\t" << "算符" << endl;
                else {
                    fout << "&" << "\t\t" << 33 << "\t\t" << "算符" << endl;
                    (fin).seekg(-1, ios::cur);
                }
            }
            /*    // /*  * /   */
            else if (ch == '\/')
            {
                ch = fin.get();
                if (ch == '\/')
                {
                    fout << "//" << "\t\t" << 34 << "\t\t" << "界符" << endl;
                    getline(fin, str, '\n');
                }
                else if (ch == '*')
                {
                    fout << "/*" << "\t\t" << 35 << "\t\t" << "界符" << endl;
                    check_notes(fin, fout);
                }
            }
            /*单符号*/
            else {
                fout << IsOp(ch, Line_NO);
            }

        }
    }

    fin.close();
    fout.close();

}
