#ifndef __QUESTION__
#define __QUESTION__

#include <string>

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>

class Question {  // 题目类的定义
protected:
	int id;  // 题目的标号
	std::string type;  // 题目的类型
	std::string description;  // 题目的具体描述
	int score;  // 题目的分值
public:
	Question(const int id, const std::string& type, const std::string& description = "") :
		id(id), type(type), description(description) {}
	~Question() {}

	int getId() const { return id; }
	std::string getType() const { return type; }
	std::string getDescription() const { return description; }
	int getScore() const { return score; }

	virtual int readFromSQL() = 0;  // 从MySQL中读取题目信息的函数 要求子类必须重写
	virtual double calScore() const { return 0.0; }  // 计算每个题目的得分 子类可以进行重写
};

#endif // ! __QUESTION__
