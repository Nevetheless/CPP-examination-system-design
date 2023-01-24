#ifndef __QUESTION__
#define __QUESTION__

#include <string>

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>

class Question {  // ��Ŀ��Ķ���
protected:
	int id;  // ��Ŀ�ı��
	std::string type;  // ��Ŀ������
	std::string description;  // ��Ŀ�ľ�������
	int score;  // ��Ŀ�ķ�ֵ
public:
	Question(const int id, const std::string& type, const std::string& description = "") :
		id(id), type(type), description(description) {}
	~Question() {}

	int getId() const { return id; }
	std::string getType() const { return type; }
	std::string getDescription() const { return description; }
	int getScore() const { return score; }

	virtual int readFromSQL() = 0;  // ��MySQL�ж�ȡ��Ŀ��Ϣ�ĺ��� Ҫ�����������д
	virtual double calScore() const { return 0.0; }  // ����ÿ����Ŀ�ĵ÷� ������Խ�����д
};

#endif // ! __QUESTION__
