#include"create-table.h"

bool LR_1::read_wenfa(string filename)
{
	ifstream file;
	file.open(FILE_NAME, ios::in);
	if (!file.is_open())
	{
		cout << "File open fail";
		return 0;
	}
	char c;
	string temp;
	getline(file, temp);        //读取终结符
	for (int i = 0; i < temp.length(); ++i)
	{
		string t(1,temp[i]);
		VT.insert(t);
	}


	while (getline(file,temp))  //读取产生式
	{
		products[p_num].start = temp[0];
		for (int i = 3; i < temp.length(); ++i)
		{
			products[p_num].next.push_back(temp.substr(i, 1));
		}
		VN.insert(products[p_num].start);
		++p_num;
	}
	return 1;
}

void LR_1::init()
{
	output.open(OUTPUT_FILE_NAME, ios::out | ios::app);
	this->print_products();
	this->LR1_Analyse();
	this->print_ACTION_GOTO();//打印ACTION GOTO表 
	this->Input_Analyse();//输入句子，开始分析 
	output.close();
}

void LR_1::print_products()
{
	output << "输入的文法为：" << endl;
	for (int i = 0; i < p_num; ++i)
	{
		output << products[i].start << "->";
		for (auto iter = products[i].next.begin(); iter != products[i].next.end(); ++iter)
		{
			output << *iter;
		}
		output << endl;

	}
}


bool cmp_vector(vector<CSS_LR1>& v1, vector<CSS_LR1>& v2)
{
	if (v1.size() != v2.size()) return false;
	for (int i = 0; i < v2.size(); i++)
	{
		CSS_LR1 t;
		t = v2[i];
		vector<CSS_LR1>::iterator result = find(v1.begin(), v1.end(), t); //查找3
		if (result == v1.end()) //没找到
			return false;
	}
	return true;
}


bool find_in_vector(vector<CSS_LR1> T, CSS_LR1 p) {
	vector<CSS_LR1>::iterator it;
	for (it = T.begin(); it != T.end(); it++) {
		if (*it == p) {
			return true;
		}
	}
	return false;
}

void LR_1::print_date()
{
	for (int i = 0; i < p_num; ++i)
	{
		output << products[i].start << "->";
		for (auto iter = products[i].next.begin(); iter != products[i].next.end(); ++iter)
		{
			output << *iter;
		}
		output << endl;
		
	}

	for (auto iter = VT.begin(); iter != VT.end(); ++iter) 
	{
		output << *iter;
	}
	output << endl;
	for (auto iter = VN.begin(); iter != VN.end(); ++iter)
	{
		output << *iter;
	}
	return;
}

LR_1::LR_1()
{
	read_wenfa(FILE_NAME);

}


set<string> LR_1::get_first(string x)
{
	set<string> temp;
	for (int i = 0; i < p_num; ++i)
	{
		if (products[i].start == x)   //是x推导的产生式
		{
			for (int k = 0; k < products[i].next[k].length(); ++k)  //对该产生式的后半部分进行分析
			{
				if (VT.find(products[i].next[k]) != VT.end())  //是终结符开头，直接插入
				{
					temp.insert(products[i].next[k]);
					break;
				}
				else   //不是终结符开头
				{
					if (products[i].next[k] == products[i].start) {   //产生式开始=推导式中的某一非终结符，即存在左递归
						break;
					}
					set<string> U = get_first(products[i].next[k]);  //找该非终结符的first集
					temp.insert(U.begin(), U.end());  //插入

					if (U.find("$") != U.end()) { //U中含有伊布西龙,继续查下个的first
						if (k != products[i].next.size() - 1)
							temp.erase("$");
					}
					else {
							break;
					}
				}
			}
		}
	}
	return temp;
}

set<string> LR_1::get_follow(string x)
{
	set<string> T, temp;

	mark_Follow[x]++;
	if (mark_Follow[x] >= 2) {
		return T;
	}

	if (x == products[0].start) {        //起始符号压入#
		T.insert("#");
	}
	for (int i = 0; i < p_num; ++i)
	{
		for (int k = 0; k < products[i].next.size(); ++k)
		{
			if (x == products[i].next[k] && VT.find(x) == VT.end()) {
				if (k == products[i].next.size() - 1 && x != products[i].start) {   //S->...x 
					set<string> tt = get_follow(products[i].start);
					T.insert(tt.begin(), tt.end());
				}
				for (int m = k + 1; m < products[i].next.size(); ++m)
				{
					if (VT.find(products[i].next[m]) != VT.end())//后面一个是终结符  S->..xv.. 
					{
						T.insert(products[i].next[m]);
						break;
					}
					else  //非初始符，x后面是非终结符的情况：
					{
						temp = get_first(products[i].next[m]);
						if (temp.find("$") != temp.end()) {//有$ S->..x B.. 
							T.insert(temp.begin(), temp.end());
							T.erase("$");
							if (m == products[i].next.size() - 1) { //S->..x B
								set<string> tt = get_follow(products[i].start);
								T.insert(tt.begin(), tt.end());
								break;
							}
						}
						else {
							T.insert(temp.begin(), temp.end());
							break;
						}
					}

				}
			}
		}
	}
	mark_Follow[x] = 0;
	return T;
}

vector<CSS_LR1> LR_1::CLOSURE(CSS_LR1 I)
{
	vector<CSS_LR1> T;
	if (I.num >= I.next.size()) { //规约项目A->α.或者接受项目 
		return T;
	}
	else
	{
		string temp = I.next[I.num];  //点后边的符号
		if (VT.find(temp) != VT.end()) {     //点后面的是终结符 ,移进项目 A→α.aβ  
			return T;
		}
		else
		{
			for (int i = 0; i < p_num; ++i)
			{
				if (products[i].start == temp) {   //点后的非终结符为起始的产生式进行判断
					CSS_LR1 p;
					p.start = products[i].start;
					p.num = 0;						//点在最前面 
					p.next = products[i].next;

					set<string> f1;
					for (int j = I.num + 1; j < I.next.size(); j++) {  //找非终结符后的符号
						set<string> f2;//用于暂存first 

						if (VT.find(I.next[j]) != VT.end()) {   //是终结符

							f2.insert(I.next[j]);
						}
						else {
							f2 = get_first(I.next[j]);
						}  //找first（beta/a）集

						f1.insert(f2.begin(), f2.end());
						if (f2.find("$") == f2.end()) {  //没有伊布西龙

							break;
						}
					}

					if (f1.size() == 0) {  //无
						p.tail = I.tail;
					}
					else {
						vector<string> first_tail;
						if (f1.find("$") != f1.end()) {
							f1.erase("$");
							copy(f1.begin(), f1.end(), back_inserter(first_tail));
							first_tail.insert(first_tail.end(), I.tail.begin(), I.tail.end());
						}
						else {
							copy(f1.begin(), f1.end(), back_inserter(first_tail));
						}
						p.tail = first_tail;
					}
					if (!find_in_vector(T, p)) {

						T.push_back(p);
						vector<CSS_LR1> ol = CLOSURE(p);
						vector<CSS_LR1>::iterator z;
						for (z = ol.begin(); z != ol.end(); z++) {
							if (find_in_vector(T, *z)) {
							}
							else {
								T.push_back(*z);
							}
						}

					}

				}
			}
		}
	}
	return T;
}

void LR_1::LR1_Analyse() {
	CSS_LR1 p;
	//初始项目 S’->.S ,# 
	p.start = products[0].start + "^";
	p.num = 0;//点在最前面 
	p.tail.push_back("#");
	p.next.push_back(products[0].start);

	I[0] = CLOSURE(p);//求闭包后的I[0]
	I[0].insert(I[0].begin(), p);
	I_count = 1;

	//计算项目集 
	for (int i = 0; i < I_count; i++) {//每个项目集  项目集I(i) 

		output << "===================" << endl;
		output << "现在在计算项目集I" << i << endl;
		showI(I[i]);//展示项目集 
		output << "===================" << endl;

		//---------求ACTION的r部分-------------- 
		vector<CSS_LR1>::iterator t;
		for (t = I[i].begin(); t != I[i].end(); t++) {
			CSS_LR1 t2 = *t;
			if (t2.num == t2.next.size()) {
				int num = 0;
				for (int xp = 0; xp < p_num; xp++) {
					if (products[xp].start == t2.start && products[xp].next == t2.next) {
						num = xp;
						break;
					}
				}

				std::stringstream ss;
				ss << num;
				string s = ss.str();
				for (int q = 0; q < t2.tail.size(); q++) {
					ACTION[i][t2.tail[q]] = "r" + s;
				}
				if (t2.num == 1 && t2.next[0] == products[0].start) {
					ACTION[i]["#"] = "acc";
				}
			}
		} 



		set<string>::iterator it;
		for (it = VN.begin(); it != VN.end(); it++) {  //每个非终结符
			vector<CSS_LR1> temp;
			for (int j = 0; j < I[i].size(); j++) {
				CSS_LR1 lr = I[i][j];
				if (lr.num < lr.next.size() && lr.next[lr.num] == *it) {
					//cout<<*it<<endl; 
					vector<CSS_LR1> t2;
					lr.num++;
					t2 = CLOSURE(lr);
					t2.push_back(lr);

					temp = temp + t2;
				}
			}
			//cout<<"temp.size"<< temp.size()<<endl;

			if (temp.size() > 0) {
				int k;
				for (k = 0; k < I_count; k++) {//找一找项目集是否已经存在 
					if (cmp_vector(I[k], temp)) {
						break;
					}
				}
				if (k == I_count) {
					//产生了新的项目集 
					I[I_count] = temp;
					output << "  I" << i << " -- " << *it << "->" << "I" << I_count << endl << endl;
					GOTO[i][*it] = I_count;//更新goto表 
					I_count++;
				}
				else {
					//项目集已经存在，需要自己指向自己
					output << "  I" << i << " -- " << *it << "->" << "I" << k << endl << endl;
					GOTO[i][*it] = k;

				}

			}

		}
		for (it = VT.begin(); it != VT.end(); it++) {  //每个终结符
			vector<CSS_LR1> temp;

			for (int j = 0; j < I[i].size(); j++) {
				CSS_LR1 lr = I[i][j];

				if (lr.num < lr.next.size() && lr.next[lr.num] == *it) {
					vector<CSS_LR1> t2;
					lr.num++;
					t2 = CLOSURE(lr);//闭包求出的结果不包含本身 
					t2.insert(t2.begin(), lr);
					//showI(t2);
					temp = temp + t2;
				}
			}
			if (temp.size() > 0) {
				int k;
				for (k = 0; k < I_count; k++) {//找一找项目集是否已经存在 
					if (cmp_vector(I[k], temp)) {
						break;
					}
				}
				if (k == I_count) {
					//产生了新的项目集 
					I[I_count] = temp;
					output << "  I" << i << " -- " << *it << "->" << "I" << I_count << endl << endl;
					std::stringstream ss;
					ss << I_count;
					string s = ss.str();
					ACTION[i][*it] = "S" + s;//更新AVTION表 
					I_count++;
				}
				else {
					//项目集已经存在，需要自己指向自己
					output << "  I" << i << " -- " << *it << "->" << "I" << k << endl << endl;
					std::stringstream ss;
					ss << k;
					string s = ss.str();
					ACTION[i][*it] = "S" + s;

				}

			}
		}


	}


}

void LR_1::showI(vector<CSS_LR1> I) {//展示项目集 
	vector<CSS_LR1>::iterator it;
	for (it = I.begin(); it != I.end(); it++) {
		CSS_LR1 p = *it;
		output << p.start << "->";
		vector<string>::iterator s;
		for (int j = 0; j < p.next.size(); j++) {
			if (j == p.num) output << ".";
			output << p.next[j];
		}if (p.num == p.next.size())output << ".";
		output << ",";
		for (int k = 0; k < p.tail.size(); k++) {
			output << p.tail[k];
		}output << endl;
	}
}

void LR_1::print_line() {
	output << "-----------------------------------------------------------------------------" << endl;
}



void LR_1::print_ACTION_GOTO() {
	set<string>::iterator it;
	print_line();
	output << setw(27) << setiosflags(ios::right) << "ACTION";
	output << setw(20) << setiosflags(ios::left) << "  GOTO" << endl;
	print_line();
	output << setw(8) << "项目集" << "|";

	for (it = VT.begin(); it != VT.end(); it++) {
		output << setw(8) <<  *it << "|";
	}
	output << setw(8) << "#" << "|";
	for (it = VN.begin(); it != VN.end(); it++) {
		output << setw(8) << *it << "|";
	}
	output << endl;
	for (int j = 0; j < I_count; j++) {
		output << setw(6) << "I" << setw(2) << j << "|";
		for (it = VT.begin(); it != VT.end(); it++) {
			output << setw(8) << ACTION[j][*it] << "|";
		}
		output << setw(8) << ACTION[j]["#"] << "|";
		for (it = VN.begin(); it != VN.end(); it++) {

			if (GOTO[j][*it])//GOTO表为0 
				output << setw(8) << GOTO[j][*it] << "|";
			else {
				output << setw(8) << " " << "|";
			}
		}
		output << endl;
	}
	print_line();

}



//对栈容器进行输出,i=0,返回status中的字符串,i=1,返回sign中的字符串，i=2返回inputStr中的字符串
string vectTrancStr(int i, vector<int> status, vector<string> sign) {
	string buf;
	int count = 0;
	//输出状态栈
	if (i == 0) {
		vector<int>::iterator it = status.begin();
		//将数字转化为字符串
		string str, tempStr;
		for (it; it != status.end(); it++) {
			stringstream ss;
			ss << *it;
			ss >> tempStr;
			str += tempStr;
		}
		return str;
	}
	//输出符号栈
	else if (i == 1) {
		vector<string>::iterator it = sign.begin();
		for (it; it != sign.end(); it++) {
			buf += *it;
			count++;
		}
	}


	string str(buf);
	return str;
}



void LR_1::Input_Analyse() {//输入句子，开始分析

	vector<int>  status;//定义状态栈
	vector<string> sign;//定义符号栈

	int step = 1;  //步骤
	string input;
	cout << "请输入分析的字符串(请以#结尾)：";
	cin >> input;//输入待分析的句子 
	input = input + "#";

	status.push_back(0);//把状态0入栈
	//把#加入符号栈
	sign.push_back("#");
	//输出初始栈状态
	cout << setw(10) << "步骤" << setw(10) << "状态栈" << setw(10) << "符号栈" << setw(10) << "输入串" << setw(25) << "动作说明" << endl;

	int s = 0;//初始状态

	int oldStatus;//保存之前的状态


	string input_s;  //获取初始符号
	input_s = input.substr(0, 1);

	while (ACTION[s][input_s] != "acc") {//如果action[s][input_s] =="acc" ，则分析成功
		//获取字符串
		string str = ACTION[s][input_s];
		//如果str为空，报错并返回
		if (str.size() == 0) {
			cout << "出错";
			return;
		}
		//获取S或r后面的数字 
		stringstream ss;
		ss << str.substr(1);
		ss >> s;//新的状态号 
		//如果是移进 
		if (str.substr(0, 1) == "S") {
			cout << setw(10) << step << setw(10) << vectTrancStr(0, status, sign) << setw(10) << vectTrancStr(1, status, sign) << setw(10) << input << setw(10) << "A" << "CTION[" << status.back() << "," << input_s << "]=S" << s << "," << "状态" << s << "入栈" << endl;
			sign.push_back(input_s); //输入符号入栈 
			input.erase(0, 1);
			status.push_back(s);//将状态数字入栈
		}
		//如果是规约 
		else if (str.substr(0, 1) == "r") {
			string kaitou;//产生式的头部 
			kaitou = products[s].start;
			int pop_num = products[s].next.size();//获取符号栈的出栈次数

			string r;
			stringstream ss;
			ss << s;
			ss >> r;
			int oldStatus;//保存之前的状态
			int status_size = status.size();
			oldStatus = status[status_size - 1 - pop_num];
			s = GOTO[oldStatus][kaitou];
			cout << setw(10) << step << setw(10) << vectTrancStr(0, status, sign) << setw(10) << vectTrancStr(1, status, sign) << setw(10) << input << setw(10) << (string)":产生式" + r + (string)"归约,GOTO(" << oldStatus << "," << kaitou << ")=" << s << "入栈" << endl;
			//对符号栈进行出栈和状态栈进行出栈
			while (pop_num--) {
				sign.pop_back();
				status.pop_back();
			}

			sign.push_back(kaitou);//再对产生式的开始符号入栈
			status.push_back(s);//再把新的状态入栈
		}
		else {
			//nothing 
		}

		step++; //步骤数加1

		s = status.back();//获取栈顶状态

		input_s = input.substr(0, 1);//获取输入的字符
	}
	cout << setw(10) << step << setw(10) << vectTrancStr(0, status, sign) << setw(10) << vectTrancStr(1, status, sign) << setw(10) << input << setw(10) << "A" << "cc:分析成功" << endl;


}