#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "lex_analyze.h"


#define   MAXBUF    100

using namespace std;

struct node keywords[MAX];

/*�Թؼ��ֱ���г�ʼ����div,mod,and,orҲ��Ϊ�ؼ��ִ���*/
/*��С��token��30*/
void init()
{
    int j;
    for (j = 0; j < MAX; j++){
        strcpy(keywords[j].lexptr, key[j]);
        keywords[j].token = j + 30;
    }
}

/***************�Թؼ��ֽ�������**************/
int Iskeyword(node keyword[],char* ch) {
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

/*****************�ж��Ƿ�Ϊ��ĸ*****************/
bool IsLetter(char c)
{
    if ((c <= 'z') && (c >= 'a') || (c <= 'Z') && (c >= 'A')) 
        return 1;
    else 
        return 0;
}

/*************�ж��Ƿ�Ϊ����**************/
bool IsDigit(char c) {
    if (c >= '0' && c <= '9') 
        return 1;
    else 
        return 0;
}

/*�ж��Ƿ�Ϊ���������*/
string IsOp(char ch, int Line_No) {
    stringstream ss;
    int i = 0;
    for (; i < MAX; i++) {
        cout << ch << endl;
        if (op[i].myoperator[0] == ch)
            break;
    }
    if (i < MAX) {
        ss << op[i].myoperator << "\t\t" << op[i].number << "\t\t" << op[i].type << endl;
    }
    else {
        ss << "�ڵ�" << Line_No << "���޷�ʶ����ַ�\t" << ch << endl;
    }
    return ss.str();
}


/***************��������**************/
void text_analyse(string input, string output) {

    fstream fin, fout;
    fin.open(input.c_str(), ios::in | ios::binary);
    fout.open(output.c_str(), ios::out | ios::binary);
    if (!fin.is_open()) {
        cout << "�޷�����Ҫ��ȡ���ļ�" << endl;
        return;
    }

    char arr[MAXBUF];
    int j = 0;
    char ch = ' ';             /*��Ŷ��뵱ǰ�������ַ�*/
    int Line_NO = 0;                /*��¼�к�*/
    while ((ch = (fin).get()) != EOF) {
        /*�ո�tab*/
        if (ch == ' ' || ch == '\t') {
            continue;
        }
        else if (ch == '\n') { Line_NO++; }
        /*�ַ���*/
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
            if (Iskeyword(keywords, arr)){
                fout << arr << "\t\t" << Iskeyword(keywords, arr) << "\t\t" << "�ؼ���" << endl;
            }
            else
                fout << arr << "\t\t" << 1 << "\t\t" << "��ʶ��" << endl;
        }
        /*����*/
        else if (IsDigit(ch)) {
            int s = 0;
            while (IsDigit(ch) | IsLetter(ch)) {
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
            }
            (fin).seekg(-1, ios::cur);
            arr[j] = '\0';
            j = 0;
            if (s == 0)
                fout << arr << "\t\t" << 2 << "\t\t" << "�޷�������" << endl;
            else if (s == 1)
                fout << arr << "\t\t" << 3 << "\t\t" << "�޷��Ա�" << endl;
        }
        else {
            /*>,>=*/
            if (ch == '>') {
                ch = fin.get();
                if (ch == '=')
                    fout << ">=" << "\t\t" << 41 << "\t\t" << "�����" << endl;
                else {
                    fout << ">" << "\t\t" << 41 << "\t\t" << "�����" << endl;
                    fin.seekg(-1, ios::cur);
                }
            }
            /*<,<=,<>*/
            else if (ch == '<') {
                ch = (fin).get();
                if (ch == '=')
                    fout << "<=" << "\t\t" << 41 << "\t\t" << "�����" << endl;
                else if (ch == '>')
                    fout << "<>" << "\t\t" << 41 << "\t\t" << "�����" << endl;
                else {
                    fout << "<" << "\t\t" << 41 << "\t\t" << "�����" << endl;
                    (fin).seekg(-1, ios::cur);
                }
            }
            /*������*/
            else {
                IsOp(ch, Line_NO);
            }
            
        }
    }

    fin.close();
    fout.close();

}