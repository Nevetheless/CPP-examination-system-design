#ifndef __PROGRAMQUESTION__
#define __PROGRAMQUESTION__

#include "Question.h"
#include <iostream>

class ProgramQuestion : public Question {  // 编程题类的定义
private:
	bool ableToRun;  // 此变量用于记录每道编程题是否能够直接运行
	std::string student_ans;  // 学生作答的答案
public:
	ProgramQuestion(const int& id, const std::string& type) :
		Question(id, type) {}
	~ProgramQuestion() {}

	std::string getStudentAns() { return student_ans; }
	bool getRunInfo() { return ableToRun; }
	void setStudentAns(const std::string& ans) { student_ans = ans; }
	void setRunInfo(const bool value) { ableToRun = value; }

	int readFromSQL();
};

inline 
int ProgramQuestion::readFromSQL() {
	QSqlQuery query("select * from programbank where 题号 = '" + QString::number(id) + "'");
	while (query.next()) {
		description = std::string(query.value(2).toString().toLocal8Bit());
		score = atoi(std::string(query.value(3).toString().toLocal8Bit()).c_str());
	}
	if (description == "") return 0;
	return 1;
}

#endif // !__PROGRAMQUESTION__

