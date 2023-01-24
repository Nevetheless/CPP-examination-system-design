#ifndef __FILLQUESTION__
#define __FILLQUESTION__

#include "Question.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class FillQuestion : public Question{  // �������Ķ���
private:
	int numofblanks;  // �յĸ���
	std::vector<std::string> ans;  // ��
	std::vector<std::string> student_ans;  // ѧ������Ĵ�
	bool disorder;  // �������Ƿ�����(���ں����з�)
public:
	FillQuestion(const int& id, const std::string& type) :
		Question(id, type) {}
	~FillQuestion() {}

	int getNumOfBlanks() { return numofblanks; }
	std::vector<std::string> getStudentAns() { return student_ans; }
	void setStudentAns(const std::vector<std::string>& ans);
	int readFromSQL();
	double calScore() const;
};

inline
int FillQuestion::readFromSQL() {
	QSqlQuery query("select * from fillbank where ��� = '" + QString::number(id) + "'");
	while (query.next()) {
		description = std::string(query.value(2).toString().toLocal8Bit());
		std::string str = std::string(query.value(3).toString().toLocal8Bit());
		std::string buff{ "" };
		// �ָ�str�õ������յĴ�
		char c = ' ';
		for (auto n : str)
		{
			if (n != c) buff += n; else
				if (n == c && buff != "") { ans.push_back(buff); buff = ""; }
		}
		if (buff != "") ans.push_back(buff);

		numofblanks = atoi(std::string(query.value(4).toString().toLocal8Bit()).c_str());
		score = atoi(std::string(query.value(5).toString().toLocal8Bit()).c_str());
		disorder = std::string(query.value(6).toString().toLocal8Bit()) == "1" ? true : false;
	}
	if (description == "") return 0;
	return 1;
}

inline 
void FillQuestion::setStudentAns(const std::vector<std::string>& ans) {
	student_ans.resize(ans.size());
	for (int i = 0; i < student_ans.size(); i++) {
		student_ans[i] = ans[i];
	}
}

inline
double FillQuestion::calScore() const {  // ������зַ���
	double sum = 0.0;
	if (disorder) {  // �������𰸺Ϳ��޹أ�������ģ���һ��Ϊ�����⣬��һ�㲻���ظ������ù�ϣ����мƷ�
		std::unordered_map<std::string, bool> hash;
		for (auto str : ans) hash[str] = true;
		for (auto str : student_ans) {
			auto p = hash.find(str);
			if (p != hash.end() && p->second) {
				p->second = false;
				sum += 1.0;
			}
		}
		return sum;
	} else {  // ��������������ÿ���ս��д𰸶Ա�
		for (int i = 0; i < student_ans.size(); i++) {
			if (ans[i].find("/") == std::string::npos) {  // ����ÿ�û�ж����(��׼���У����ж���𰸣�����"/"�ָ�), ��ֱ�ӱȽ϶��ߴ�
				if (ans[i] == student_ans[i]) sum += 1.0;  
			} else {  // ����ÿ��ж�����д𰸣����ȸ���"/"�ָ��õ����п��д𰸣�����ѧ��������бȽ�

				/* �ָ�str�õ����п��еĴ� */
				std::vector<std::string> vec;  // vec���ڴ洢��ǰ�����п��д�
				std::string buff{ "" };
				char c = '/';
				for (auto n : ans[i])
				{
					if (n != c) buff += n; else
						if (n == c && buff != "") { vec.push_back(buff); buff = ""; }
				}
				if (buff != "") vec.push_back(buff);

				/* �����п��д���ѧ��������жԱ� */
				for (auto str : vec) {
					if (str == student_ans[i]) {  
						sum += 1.0;
						break;
					}
				}
			}
		}
		return sum;
	}
}

#endif // !__FILLQUESTION__

