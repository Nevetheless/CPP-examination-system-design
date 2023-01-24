#ifndef __PAPER__
#define __PAPER__

#define _CRT_SECURE_NO_DEPRECATE 

#include "Question.h"
#include "ChoiceQuestion.h"
#include "FillQuestion.h"
#include "ShortQuestion.h"
#include "ProgramQuestion.h"

#include <string>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <fstream>

#include <winsock2.h>
#include <string.h>
#include <Windows.h>
#include <assert.h>
#include <WS2tcpip.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <numeric>
#pragma comment(lib,"ws2_32.lib")

#define CHOICE_NUM 10  // 选择题题数
#define FILL_NUM 10  // 填空题题数
#define SHORT_NUM 5  // 简答题题数
#define PROGRAM_NUM 3  // 编程题题数
#define BUFFER_SIZE 1024 // socket数据传输缓冲区大小

class Paper {  // Paper类 在用户登录后实例化一个全局对象，用于存储此次考试中，考生信息、试题信息以及答题信息
private:
	std::string id;  // 学生学号
	std::string name;  // 学生姓名
	Question* q[CHOICE_NUM + FILL_NUM + SHORT_NUM + PROGRAM_NUM];  // Question*数组, 用于存储所有题目

public:
	Paper(const std::string& id, const std::string& name) :
		id(id), name(name) {}
	~Paper() { delete[] q; }

	void initialization();  // 初始化函数，在考试开始时，将考生信息，实体信息初始化
	void calAllScores() const;  // 判分函数，计算学生作答的得分情况
	std::vector<std::string> saveAsFiles() const;  // 整理学生最后的答案，保存成文件形式
	void uploadAllFiles(const std::vector<std::string>& filenames) const;  // 将保存好的文件统一上传到云服务器中

	Question** getQuestions() { return q; }
};

inline 
void Paper::initialization() {
	// 通过ODBC来创建MySQL连接
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");    

	db.setHostName("127.0.0.1");  // 服务器ip         
	db.setDatabaseName("examinationdb");  // 连接数据库 --> examinationdb 
	db.setUserName("root");  // 数据库用户名         
	db.setPassword("your password");   // 数据库密码
	db.setPort(3306);

	if (db.open()) {
		qDebug() << "connection success...";
	} else {
		qDebug() << "connection failed...";
		return;
	}

	std::cout << "===================正在读取选择题====================" << std::endl << std::endl;

	for (int i = 0; i < CHOICE_NUM; i++) {
		ChoiceQuestion* cq = new ChoiceQuestion(i + 1, "选择题");
		cq->readFromSQL();
		q[i] = cq;
	}

	std::cout << "===================正在读取填空题====================" << std::endl << std::endl;

	for (int i = 0; i < FILL_NUM; i++) {
		FillQuestion* fq = new FillQuestion(i + 1, "填空题");
		fq->readFromSQL();
		q[CHOICE_NUM + i] = fq;
	}

	std::cout << "===================正在读取简答题====================" << std::endl << std::endl;

	for (int i = 0; i < SHORT_NUM; i++) {
		ShortQuestion* sq = new ShortQuestion(i + 1, "简答题");
		sq->readFromSQL();
		q[CHOICE_NUM + FILL_NUM + i] = sq;
	}

	std::cout << "===================正在读取编程题====================" << std::endl << std::endl;

	for (int i = 0; i < PROGRAM_NUM; i++) {
		ProgramQuestion* pq = new ProgramQuestion(i + 1, "编程题");
		pq->readFromSQL();
		q[CHOICE_NUM + FILL_NUM + SHORT_NUM + i] = pq;
	}

	db.close();
}

inline
void Paper::calAllScores() const {

	std::vector<double> choice_score;  // 选择题得分 
	std::vector<double> fill_score;  // 填空题得分
	std::vector<double> short_score;  // 简答题得分
	std::vector<bool> program_ableToRun;  // 编程题是否能正常运行

	std::cout << "====================正在进行判分====================" << std::endl << std::endl;

	std::cout << "==================正在为选择题判分===================" << std::endl << std::endl;

	for (int i = 0; i < CHOICE_NUM; i++) {
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(q[i]);
		choice_score.push_back(cq->calScore());
	}

	std::cout << "==================正在为填空题判分===================" << std::endl << std::endl;

	for (int i = 0; i < FILL_NUM; i++) {
		FillQuestion* fq = dynamic_cast<FillQuestion*>(q[CHOICE_NUM + i]);
		fill_score.push_back(fq->calScore());
	}

	std::cout << "==================正在为简答题判分===================" << std::endl << std::endl;

	std::string str = "python ./word2vec/main.py";

	for (int i = 0; i < SHORT_NUM; i++) {
		ShortQuestion* sq = dynamic_cast<ShortQuestion*>(q[CHOICE_NUM + FILL_NUM + i]);
		std::vector<std::string> para = sq->returnPara();

		std::string arg1 = "ans" + std::to_string(i + 1) + " ";
		std::string arg2 = "student_ans" + std::to_string(i + 1) + " ";

		str += " --" + arg1 + para[0] + " --" + arg2 + para[1];
	}

	str += ">temp.txt";

	std::system(str.c_str());

	std::ifstream fp("temp.txt");
	if (!fp.is_open())
	{
		std::cout << "open error" << std::endl;
	}
	std::string read_str;
	while (std::getline(fp, read_str))
	{
		std::stringstream s(read_str);
		double similarity;
		s >> std::fixed >> std::setprecision(1) >> similarity;
		short_score.push_back(similarity * 5);
	}
	fp.close();

	for (int i = 0; i < PROGRAM_NUM; i++) {
		ProgramQuestion* pq = dynamic_cast<ProgramQuestion*>(q[CHOICE_NUM + FILL_NUM + SHORT_NUM + i]);
		program_ableToRun.push_back(pq->getRunInfo());
	}

	std::cout << "==================正在保存学生成绩===================" << std::endl << std::endl;

	std::ofstream os;
	os.open("scores.csv", std::ios::out | std::ios::trunc);

	os << std::string("学号") << "," << "姓名" << ",";
	for (int i = 0; i < choice_score.size(); i++) {
		os << "选择题" + std::to_string(i + 1) << ",";
	}

	for (int i = 0; i < fill_score.size(); i++) {
		os << "填空题" + std::to_string(i + 1) << ",";
	}

	for (int i = 0; i < short_score.size(); i++) {
		os << "简答题" + std::to_string(i + 1) << ",";
	}

	for (int i = 0; i < program_ableToRun.size(); i++) {
		os << "编程题" + std::to_string(i + 1) + "(是否能运行)" << ",";
	}

	os << "选择题得分" << "," << "填空题得分" << "," << "简答题得分" << "," << "总分" << std::endl;

	double choice_sum = 0.0;
	double fill_sum = 0.0;
	double short_sum = 0.0;
	double sum = 0.0;

	std::cout << "----------name: " << name << std::endl;

	os << id << "," << name << ",";
	for (int i = 0; i < choice_score.size(); i++) {
		os << choice_score[i] << ",";
		choice_sum += choice_score[i];
	}

	for (int i = 0; i < fill_score.size(); i++) {
		os << fill_score[i] << ",";
		fill_sum += fill_score[i];
	}

	for (int i = 0; i < short_score.size(); i++) {
		os << short_score[i] << ",";
		short_sum += short_score[i];
	}

	for (int i = 0; i < program_ableToRun.size(); i++) {
		os << (program_ableToRun[i] == 1 ? "是" : "否") << ",";
	}

	sum = choice_sum + fill_sum + short_sum;

	os << choice_sum << "," << fill_sum << "," << short_sum << "," << sum << std::endl;

	os.close();

	std::cout << "==================保存学生成绩成功===================" << std::endl << std::endl;
}

inline
std::vector<std::string> Paper::saveAsFiles() const {

	std::cout << "==================正在保存学生作答===================" << std::endl << std::endl;

	std::vector<std::string> filenames(2 + SHORT_NUM + PROGRAM_NUM);

	std::string c_filename = std::string("choiceQuestion.txt");
	for (int i = 0; i < CHOICE_NUM; i++) {
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(q[i]);
		char c = cq->getStudentAns();
		std::ofstream os; // 创建文件输出流对象
		os.open(c_filename, std::ios::app);
		os << i + 1 << ". " << c << "\n";  // 将学生作答的内容写入txt文件中
		os.close();
	}

	filenames[0] = c_filename;

	std::string f_filename = std::string("fillQuestion.txt");
	for (int i = 0; i < FILL_NUM; i++) {
		FillQuestion* fq = dynamic_cast<FillQuestion*>(q[CHOICE_NUM + i]);
		std::vector<std::string> vec = fq->getStudentAns();
		std::ofstream os; // 创建文件输出流对象
		os.open(f_filename, std::ios::app);
		os << i + 1 << ". ";  // 将学生作答的内容写入txt文件中
		for (auto str : vec) {
			os << str << " ";
		}
		os << "\n";
		os.close();
	}

	filenames[1] = f_filename;

	for (int i = 0; i < SHORT_NUM; i++) {
		ShortQuestion* sq = dynamic_cast<ShortQuestion*>(q[CHOICE_NUM + FILL_NUM + i]);
		std::string str = sq->getStudentAns();
		std::string filename = std::string("shortQuestion_") + std::to_string(i + 1) + std::string(".txt");
		std::ofstream os; // 创建文件输出流对象
		os.open(filename, std::ios::app);
		os << str;  // 将学生作答的内容写入txt文件中
		os.close();
		filenames[2 + i] = filename;
 	}

	for (int i = 0; i < PROGRAM_NUM; i++) {
		ProgramQuestion* pq = dynamic_cast<ProgramQuestion*>(q[CHOICE_NUM + FILL_NUM + SHORT_NUM + i]);
		std::string str = pq->getStudentAns();
		std::string filename = std::string("programQuestion_") + std::to_string(i + 1) + std::string(".cpp");
		std::ofstream os; // 创建文件输出流对象
		os.open(filename, std::ios::app);
		os << str;  // 将学生作答的内容写入cpp文件中
		os.close();
		filenames[2 + SHORT_NUM + i] = filename;
	}

	filenames.push_back("scores.csv");
	filenames.push_back("program0_output.txt");
	filenames.push_back("program1_output.txt");
	filenames.push_back("program2_output.txt");

	std::cout << "==================保存学生作答成功===================" << std::endl << std::endl;

	return filenames;
}

inline 
void Paper::uploadAllFiles(const std::vector<std::string>& filenames) const {

	std::cout << "==================正在上传学生作答===================" << std::endl << std::endl;

	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);

	const char* ip = "127.0.0.1";
	int port = 8080;
	int backlog = 5;

	/* 创建一个IPv4地址 */
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);  // 将ip转换成网络字节序
	address.sin_port = htons(port);  // 将端口号转换成网络字节序

	int cnt = 0;

	while (cnt < filenames.size()) {
		// 创建socket
		int sock = socket(PF_INET, SOCK_STREAM, 0);

		if (sock < 0) {
			perror("[-]Error in socket");
			exit(1);
		}

		printf("[+]Server socket created successfully.\n");

		// 绑定socket和地址
		int ret = connect(sock, (struct sockaddr*)&address, sizeof(address));  // 客户端不需要绑定 只需要连接 

		if (ret == -1) {
			perror("[-]Error in socket");
			exit(1);
		}

		printf("[+]Connected to Server.\n");	

		FILE* fp = fopen(filenames[cnt].c_str(), "r");
		if (fp == NULL) {
			perror("[-]Error in reading file.");
			exit(1);
		}

		char data[BUFFER_SIZE] = { 0 };

		while (fgets(data, BUFFER_SIZE, fp) != NULL) {
			if (send(sock, data, sizeof(data), 0) == -1) {
				perror("[-]Error in sending file.");
				exit(1);
			}
			memset(data, 0, BUFFER_SIZE);
		}

		std::cout << "sending " << filenames[cnt] << std::endl;
		printf("[+]File data sent successfully.\n");

		printf("[+]Closing the connection.\n");
		closesocket(sock);

		cnt++;
	}

	std::cout << "==================上传学生作答成功===================" << std::endl << std::endl;
}

#endif // ! __PAPER__

