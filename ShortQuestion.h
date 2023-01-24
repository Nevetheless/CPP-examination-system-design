#ifndef __SHORTQUESTION__
#define __SHORTQUESTION__

#include "Question.h"

#include <string>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

class ShortQuestion : public Question {  // �������Ķ���
private:
	std::string ans;  // ��
	std::string student_ans;  // ѧ������Ĵ�
public:
	ShortQuestion(const int& id, const std::string& type) :
		Question(id, type) {}
	~ShortQuestion() {}

	std::string getStudentAns() { return student_ans; }
	void setStudentAns(const std::string& ans) { student_ans = ans; }
	int readFromSQL();
	std::vector<std::string> returnPara() const;  // ���ؼ����Ĵ��Լ�ѧ������Ĵ������з�
};

inline
int ShortQuestion::readFromSQL() {
	QSqlQuery query("select * from shortbank where ��� = '" + QString::number(id) + "'");
	while (query.next()) {
		description = std::string(query.value(2).toString().toLocal8Bit());
		ans = std::string(query.value(3).toString().toLocal8Bit());
		score = atoi(std::string(query.value(4).toString().toLocal8Bit()).c_str());
	}
	if (description == "") return 0;
	return 1;
}

inline
std::vector<std::string> ShortQuestion::returnPara() const {
	std::vector<std::string> vec;
	vec.push_back(ans);
	vec.push_back(student_ans);
	return vec;
}

#endif
