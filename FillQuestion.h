#ifndef __FILLQUESTION__
#define __FILLQUESTION__

#include "Question.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class FillQuestion : public Question{  // 填空题类的定义
private:
	int numofblanks;  // 空的个数
	std::vector<std::string> ans;  // 答案
	std::vector<std::string> student_ans;  // 学生作答的答案
	bool disorder;  // 填空题答案是否有序(用于后续判分)
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
	QSqlQuery query("select * from fillbank where 题号 = '" + QString::number(id) + "'");
	while (query.next()) {
		description = std::string(query.value(2).toString().toLocal8Bit());
		std::string str = std::string(query.value(3).toString().toLocal8Bit());
		std::string buff{ "" };
		// 分割str得到各个空的答案
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
double FillQuestion::calScore() const {  // 填空题判分方法
	double sum = 0.0;
	if (disorder) {  // 若填空题答案和空无关，是无序的，则一般为概念题，答案一般不会重复，利用哈希表进行计分
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
	} else {  // 若填空题答案有序，则每个空进行答案对比
		for (int i = 0; i < student_ans.size(); i++) {
			if (ans[i].find("/") == std::string::npos) {  // 如果该空没有多个答案(标准答案中，若有多个答案，则用"/"分隔), 则直接比较二者答案
				if (ans[i] == student_ans[i]) sum += 1.0;  
			} else {  // 如果该空有多个可行答案，则先根据"/"分隔得到所有可行答案，再与学生作答进行比较

				/* 分割str得到所有可行的答案 */
				std::vector<std::string> vec;  // vec用于存储当前空所有可行答案
				std::string buff{ "" };
				char c = '/';
				for (auto n : ans[i])
				{
					if (n != c) buff += n; else
						if (n == c && buff != "") { vec.push_back(buff); buff = ""; }
				}
				if (buff != "") vec.push_back(buff);

				/* 将所有可行答案与学生作答进行对比 */
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

