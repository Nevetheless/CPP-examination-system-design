#ifndef __PROGRAMQUESTION__
#define __PROGRAMQUESTION__

#include "Question.h"
#include <iostream>

class ProgramQuestion : public Question {  // �������Ķ���
private:
	bool ableToRun;  // �˱������ڼ�¼ÿ��������Ƿ��ܹ�ֱ������
	std::string student_ans;  // ѧ������Ĵ�
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
	QSqlQuery query("select * from programbank where ��� = '" + QString::number(id) + "'");
	while (query.next()) {
		description = std::string(query.value(2).toString().toLocal8Bit());
		score = atoi(std::string(query.value(3).toString().toLocal8Bit()).c_str());
	}
	if (description == "") return 0;
	return 1;
}

#endif // !__PROGRAMQUESTION__

