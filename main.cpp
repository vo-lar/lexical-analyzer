/*�ʷ������� ����ԭ�����ҵ1*/
#define _CRT_SECURE_NO_WARNINGS
#include "lex_analyze.h"
#include<string.h>
#include<stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    string input, output;
    fstream fin, fout;  
    cout << "                 �ʷ�������               " << endl;
    cout << "�����������:" << endl;
    cin >> input;
    cout << "��������Ҫ���������Ŀ���ļ���: " << endl;
    cin >> output;
    
    init();
    text_analyse(input, output);
    fin.close();
    fout.close();
    cout << "�����ѷ�����ɷ�����������Ŀ���ļ�" << endl;

    return 0;
}
