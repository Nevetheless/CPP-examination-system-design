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

class ShortQuestion : public Question {  // 简答题类的定义
private:
	std::string ans;  // 答案
	std::string student_ans;  // 学生作答的答案
public:
	ShortQuestion(const int& id, const std::string& type) :
		Question(id, type) {}
	~ShortQuestion() {}

	std::string getStudentAns() { return student_ans; }
	void setStudentAns(const std::string& ans) { student_ans = ans; }
	int readFromSQL();
	std::vector<std::string> returnPara() const;  // 返回简答题的答案以及学生作答的答案用于判分
};

inline
int ShortQuestion::readFromSQL() {
	QSqlQuery query("select * from shortbank where 题号 = '" + QString::number(id) + "'");
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
