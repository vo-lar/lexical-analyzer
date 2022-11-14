#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<sstream> 
#include<iomanip>
#include<vector>
#include<set>
#include<map>


using namespace std;
#define MAX_NUM 20
#define MAX_Count 100
#define FILE_NAME "products.txt"
#define OUTPUT_FILE_NAME "result.txt"
// # �� ��������
struct CSS {
	string start;//���ս��
	vector<string> next; //First���� 
};


struct CSS_LR1 {
	string start;//���ս��
	vector<string> next; //First���� 
	int num;
	vector<string> tail;

	bool operator==(const CSS_LR1& rhs) {
		return (start == rhs.start &&
			next == rhs.next &&
			num == rhs.num &&
			tail == rhs.tail);
	}
};


template <typename CSS_LR1>
vector<CSS_LR1>& operator +(vector<CSS_LR1>& v1, vector<CSS_LR1>& v2)
{
	v1.insert(v1.end(), v2.begin(), v2.end());
	return v1;
}


struct VN_set {
     string VN_name;    //���ս��
     set<string> FIRST; //First���� 
     set<string> FOLLOW;
};


bool cmp_vector(vector<CSS_LR1>& v1, vector<CSS_LR1>& v2);
bool find_in_vector(vector<CSS_LR1> T, CSS_LR1 p);
string vectTrancStr(int i, vector<int> status, vector<string> sign);


class LR_1
{
	CSS products[MAX_NUM];           //����ʽ
	set<string> VT;                  //�ս��
	set<string> VN;                  //���ս��
	map<string, int> mark_Follow;    //���ڱ��Follow ��ֹ���� 
	int p_num = 0;                   //����ʽ����
	int I_count = 0;                 //��¼LR1��Ŀ�� 
	vector<CSS_LR1> I[MAX_Count];    //��Ŀ�� 
	map<string, int> GOTO[MAX_Count];
	map<string, string> ACTION[MAX_Count];

	fstream output;                  //����ļ�

public:
	LR_1();
	void init();
	bool read_wenfa(string filename);          //�����ķ�

	//���ߺ���
	void print_date();   //������
	void print_products();

	//���̺���
	set<string> get_first(string x);        //��ȡ���ս����FIRST����
	set<string> get_follow(string x);        //��ȡ���ս����FPLLOW����
	vector<CSS_LR1> CLOSURE(CSS_LR1 I);      //����closure��
	void LR1_Analyse();
	//IO����
	void showI(vector<CSS_LR1> I);           //չʾ��Ŀ��
	void print_ACTION_GOTO();                //��ӡgoto��
	void Input_Analyse();                    //�����ַ���������
	void print_line();                       //����ָ�����
};
