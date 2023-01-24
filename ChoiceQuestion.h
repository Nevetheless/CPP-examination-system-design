#ifndef __CHOICEQUESTION__
#define __CHOICEQUESTION__

#include "Question.h"

#include <string>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <iostream>

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

class ChoiceQuestion : public Question {  // ѡ������Ķ���
private: 
	std::string A;  // ѡ��A  
	std::string B;  // ѡ��B
	std::string C;  // ѡ��C
	std::string D;  // ѡ��D
	char ans;  // ��
	char student_ans; // ѧ������Ĵ�
public:
	ChoiceQuestion(const int& id, const std::string& type) : 
		Question(id, type) {}  
	~ChoiceQuestion() {}

	std::string getA() const { return A; }
	std::string getB() const { return B; }
	std::string getC() const { return C; }
	std::string getD() const { return D; }
	char getAns() const { return ans; }
	char getStudentAns() const { return student_ans; }
	void setStudentAns(const char c) { student_ans = c; }

	int readFromSQL();
	double calScore() const;
};

inline 
int ChoiceQuestion::readFromSQL() {
	QSqlQuery query("select * from choicebank where ��� = '" + QString::number(id) + "'");
	while (query.next()) {
		description = std::string(query.value(2).toString().toLocal8Bit());  
		A = std::string(query.value(3).toString().toLocal8Bit());
		B = std::string(query.value(4).toString().toLocal8Bit());
		C = std::string(query.value(5).toString().toLocal8Bit());
		D = std::string(query.value(6).toString().toLocal8Bit());
		ans = std::string(query.value(7).toString().toLocal8Bit())[0];
		score = atoi(std::string(query.value(8).toString().toLocal8Bit()).c_str());
	}
	if (description == "") return 0;
	return 1;
}

inline
double ChoiceQuestion::calScore() const {
	return ans == student_ans ? score : 0.0;
}

#endif
