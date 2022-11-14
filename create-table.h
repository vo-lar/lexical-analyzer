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
// # 是 伊布西龙
struct CSS {
	string start;//非终结符
	vector<string> next; //First集合 
};


struct CSS_LR1 {
	string start;//非终结符
	vector<string> next; //First集合 
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
     string VN_name;    //非终结符
     set<string> FIRST; //First集合 
     set<string> FOLLOW;
};


bool cmp_vector(vector<CSS_LR1>& v1, vector<CSS_LR1>& v2);
bool find_in_vector(vector<CSS_LR1> T, CSS_LR1 p);
string vectTrancStr(int i, vector<int> status, vector<string> sign);


class LR_1
{
	CSS products[MAX_NUM];           //产生式
	set<string> VT;                  //终结符
	set<string> VN;                  //非终结符
	map<string, int> mark_Follow;    //用于标记Follow 防止套娃 
	int p_num = 0;                   //产生式数量
	int I_count = 0;                 //记录LR1项目数 
	vector<CSS_LR1> I[MAX_Count];    //项目集 
	map<string, int> GOTO[MAX_Count];
	map<string, string> ACTION[MAX_Count];

	fstream output;                  //输出文件

public:
	LR_1();
	void init();
	bool read_wenfa(string filename);          //读入文法

	//工具函数
	void print_date();   //测试用
	void print_products();

	//过程函数
	set<string> get_first(string x);        //获取非终结符的FIRST集合
	set<string> get_follow(string x);        //获取非终结符的FPLLOW集合
	vector<CSS_LR1> CLOSURE(CSS_LR1 I);      //生成closure集
	void LR1_Analyse();
	//IO函数
	void showI(vector<CSS_LR1> I);           //展示项目集
	void print_ACTION_GOTO();                //打印goto表
	void Input_Analyse();                    //输入字符串并分析
	void print_line();                       //输出分隔符号
};
