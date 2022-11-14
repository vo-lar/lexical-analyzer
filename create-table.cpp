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
	getline(file, temp);        //��ȡ�ս��
	for (int i = 0; i < temp.length(); ++i)
	{
		string t(1,temp[i]);
		VT.insert(t);
	}


	while (getline(file,temp))  //��ȡ����ʽ
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
	this->print_ACTION_GOTO();//��ӡACTION GOTO�� 
	this->Input_Analyse();//������ӣ���ʼ���� 
	output.close();
}

void LR_1::print_products()
{
	output << "������ķ�Ϊ��" << endl;
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
		vector<CSS_LR1>::iterator result = find(v1.begin(), v1.end(), t); //����3
		if (result == v1.end()) //û�ҵ�
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
		if (products[i].start == x)   //��x�Ƶ��Ĳ���ʽ
		{
			for (int k = 0; k < products[i].next[k].length(); ++k)  //�Ըò���ʽ�ĺ�벿�ֽ��з���
			{
				if (VT.find(products[i].next[k]) != VT.end())  //���ս����ͷ��ֱ�Ӳ���
				{
					temp.insert(products[i].next[k]);
					break;
				}
				else   //�����ս����ͷ
				{
					if (products[i].next[k] == products[i].start) {   //����ʽ��ʼ=�Ƶ�ʽ�е�ĳһ���ս������������ݹ�
						break;
					}
					set<string> U = get_first(products[i].next[k]);  //�Ҹ÷��ս����first��
					temp.insert(U.begin(), U.end());  //����

					if (U.find("$") != U.end()) { //U�к�����������,�������¸���first
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

	if (x == products[0].start) {        //��ʼ����ѹ��#
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
					if (VT.find(products[i].next[m]) != VT.end())//����һ�����ս��  S->..xv.. 
					{
						T.insert(products[i].next[m]);
						break;
					}
					else  //�ǳ�ʼ����x�����Ƿ��ս���������
					{
						temp = get_first(products[i].next[m]);
						if (temp.find("$") != temp.end()) {//��$ S->..x B.. 
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
	if (I.num >= I.next.size()) { //��Լ��ĿA->��.���߽�����Ŀ 
		return T;
	}
	else
	{
		string temp = I.next[I.num];  //���ߵķ���
		if (VT.find(temp) != VT.end()) {     //���������ս�� ,�ƽ���Ŀ A����.a��  
			return T;
		}
		else
		{
			for (int i = 0; i < p_num; ++i)
			{
				if (products[i].start == temp) {   //���ķ��ս��Ϊ��ʼ�Ĳ���ʽ�����ж�
					CSS_LR1 p;
					p.start = products[i].start;
					p.num = 0;						//������ǰ�� 
					p.next = products[i].next;

					set<string> f1;
					for (int j = I.num + 1; j < I.next.size(); j++) {  //�ҷ��ս����ķ���
						set<string> f2;//�����ݴ�first 

						if (VT.find(I.next[j]) != VT.end()) {   //���ս��

							f2.insert(I.next[j]);
						}
						else {
							f2 = get_first(I.next[j]);
						}  //��first��beta/a����

						f1.insert(f2.begin(), f2.end());
						if (f2.find("$") == f2.end()) {  //û����������

							break;
						}
					}

					if (f1.size() == 0) {  //��
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
	//��ʼ��Ŀ S��->.S ,# 
	p.start = products[0].start + "^";
	p.num = 0;//������ǰ�� 
	p.tail.push_back("#");
	p.next.push_back(products[0].start);

	I[0] = CLOSURE(p);//��հ����I[0]
	I[0].insert(I[0].begin(), p);
	I_count = 1;

	//������Ŀ�� 
	for (int i = 0; i < I_count; i++) {//ÿ����Ŀ��  ��Ŀ��I(i) 

		output << "===================" << endl;
		output << "�����ڼ�����Ŀ��I" << i << endl;
		showI(I[i]);//չʾ��Ŀ�� 
		output << "===================" << endl;

		//---------��ACTION��r����-------------- 
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
		for (it = VN.begin(); it != VN.end(); it++) {  //ÿ�����ս��
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
				for (k = 0; k < I_count; k++) {//��һ����Ŀ���Ƿ��Ѿ����� 
					if (cmp_vector(I[k], temp)) {
						break;
					}
				}
				if (k == I_count) {
					//�������µ���Ŀ�� 
					I[I_count] = temp;
					output << "  I" << i << " -- " << *it << "->" << "I" << I_count << endl << endl;
					GOTO[i][*it] = I_count;//����goto�� 
					I_count++;
				}
				else {
					//��Ŀ���Ѿ����ڣ���Ҫ�Լ�ָ���Լ�
					output << "  I" << i << " -- " << *it << "->" << "I" << k << endl << endl;
					GOTO[i][*it] = k;

				}

			}

		}
		for (it = VT.begin(); it != VT.end(); it++) {  //ÿ���ս��
			vector<CSS_LR1> temp;

			for (int j = 0; j < I[i].size(); j++) {
				CSS_LR1 lr = I[i][j];

				if (lr.num < lr.next.size() && lr.next[lr.num] == *it) {
					vector<CSS_LR1> t2;
					lr.num++;
					t2 = CLOSURE(lr);//�հ�����Ľ������������ 
					t2.insert(t2.begin(), lr);
					//showI(t2);
					temp = temp + t2;
				}
			}
			if (temp.size() > 0) {
				int k;
				for (k = 0; k < I_count; k++) {//��һ����Ŀ���Ƿ��Ѿ����� 
					if (cmp_vector(I[k], temp)) {
						break;
					}
				}
				if (k == I_count) {
					//�������µ���Ŀ�� 
					I[I_count] = temp;
					output << "  I" << i << " -- " << *it << "->" << "I" << I_count << endl << endl;
					std::stringstream ss;
					ss << I_count;
					string s = ss.str();
					ACTION[i][*it] = "S" + s;//����AVTION�� 
					I_count++;
				}
				else {
					//��Ŀ���Ѿ����ڣ���Ҫ�Լ�ָ���Լ�
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

void LR_1::showI(vector<CSS_LR1> I) {//չʾ��Ŀ�� 
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
	output << setw(8) << "��Ŀ��" << "|";

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

			if (GOTO[j][*it])//GOTO��Ϊ0 
				output << setw(8) << GOTO[j][*it] << "|";
			else {
				output << setw(8) << " " << "|";
			}
		}
		output << endl;
	}
	print_line();

}



//��ջ�����������,i=0,����status�е��ַ���,i=1,����sign�е��ַ�����i=2����inputStr�е��ַ���
string vectTrancStr(int i, vector<int> status, vector<string> sign) {
	string buf;
	int count = 0;
	//���״̬ջ
	if (i == 0) {
		vector<int>::iterator it = status.begin();
		//������ת��Ϊ�ַ���
		string str, tempStr;
		for (it; it != status.end(); it++) {
			stringstream ss;
			ss << *it;
			ss >> tempStr;
			str += tempStr;
		}
		return str;
	}
	//�������ջ
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



void LR_1::Input_Analyse() {//������ӣ���ʼ����

	vector<int>  status;//����״̬ջ
	vector<string> sign;//�������ջ

	int step = 1;  //����
	string input;
	cout << "������������ַ���(����#��β)��";
	cin >> input;//����������ľ��� 
	input = input + "#";

	status.push_back(0);//��״̬0��ջ
	//��#�������ջ
	sign.push_back("#");
	//�����ʼջ״̬
	cout << setw(10) << "����" << setw(10) << "״̬ջ" << setw(10) << "����ջ" << setw(10) << "���봮" << setw(25) << "����˵��" << endl;

	int s = 0;//��ʼ״̬

	int oldStatus;//����֮ǰ��״̬


	string input_s;  //��ȡ��ʼ����
	input_s = input.substr(0, 1);

	while (ACTION[s][input_s] != "acc") {//���action[s][input_s] =="acc" ��������ɹ�
		//��ȡ�ַ���
		string str = ACTION[s][input_s];
		//���strΪ�գ���������
		if (str.size() == 0) {
			cout << "����";
			return;
		}
		//��ȡS��r��������� 
		stringstream ss;
		ss << str.substr(1);
		ss >> s;//�µ�״̬�� 
		//������ƽ� 
		if (str.substr(0, 1) == "S") {
			cout << setw(10) << step << setw(10) << vectTrancStr(0, status, sign) << setw(10) << vectTrancStr(1, status, sign) << setw(10) << input << setw(10) << "A" << "CTION[" << status.back() << "," << input_s << "]=S" << s << "," << "״̬" << s << "��ջ" << endl;
			sign.push_back(input_s); //���������ջ 
			input.erase(0, 1);
			status.push_back(s);//��״̬������ջ
		}
		//����ǹ�Լ 
		else if (str.substr(0, 1) == "r") {
			string kaitou;//����ʽ��ͷ�� 
			kaitou = products[s].start;
			int pop_num = products[s].next.size();//��ȡ����ջ�ĳ�ջ����

			string r;
			stringstream ss;
			ss << s;
			ss >> r;
			int oldStatus;//����֮ǰ��״̬
			int status_size = status.size();
			oldStatus = status[status_size - 1 - pop_num];
			s = GOTO[oldStatus][kaitou];
			cout << setw(10) << step << setw(10) << vectTrancStr(0, status, sign) << setw(10) << vectTrancStr(1, status, sign) << setw(10) << input << setw(10) << (string)":����ʽ" + r + (string)"��Լ,GOTO(" << oldStatus << "," << kaitou << ")=" << s << "��ջ" << endl;
			//�Է���ջ���г�ջ��״̬ջ���г�ջ
			while (pop_num--) {
				sign.pop_back();
				status.pop_back();
			}

			sign.push_back(kaitou);//�ٶԲ���ʽ�Ŀ�ʼ������ջ
			status.push_back(s);//�ٰ��µ�״̬��ջ
		}
		else {
			//nothing 
		}

		step++; //��������1

		s = status.back();//��ȡջ��״̬

		input_s = input.substr(0, 1);//��ȡ������ַ�
	}
	cout << setw(10) << step << setw(10) << vectTrancStr(0, status, sign) << setw(10) << vectTrancStr(1, status, sign) << setw(10) << input << setw(10) << "A" << "cc:�����ɹ�" << endl;


}