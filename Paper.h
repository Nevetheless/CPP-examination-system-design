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

#define CHOICE_NUM 10  // ѡ��������
#define FILL_NUM 10  // ���������
#define SHORT_NUM 5  // ���������
#define PROGRAM_NUM 3  // ���������
#define BUFFER_SIZE 1024 // socket���ݴ��仺������С

class Paper {  // Paper�� ���û���¼��ʵ����һ��ȫ�ֶ������ڴ洢�˴ο����У�������Ϣ��������Ϣ�Լ�������Ϣ
private:
	std::string id;  // ѧ��ѧ��
	std::string name;  // ѧ������
	Question* q[CHOICE_NUM + FILL_NUM + SHORT_NUM + PROGRAM_NUM];  // Question*����, ���ڴ洢������Ŀ

public:
	Paper(const std::string& id, const std::string& name) :
		id(id), name(name) {}
	~Paper() { delete[] q; }

	void initialization();  // ��ʼ���������ڿ��Կ�ʼʱ����������Ϣ��ʵ����Ϣ��ʼ��
	void calAllScores() const;  // �зֺ���������ѧ������ĵ÷����
	std::vector<std::string> saveAsFiles() const;  // ����ѧ�����Ĵ𰸣�������ļ���ʽ
	void uploadAllFiles(const std::vector<std::string>& filenames) const;  // ������õ��ļ�ͳһ�ϴ����Ʒ�������

	Question** getQuestions() { return q; }
};

inline 
void Paper::initialization() {
	// ͨ��ODBC������MySQL����
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");    

	db.setHostName("127.0.0.1");  // ������ip         
	db.setDatabaseName("examinationdb");  // �������ݿ� --> examinationdb 
	db.setUserName("root");  // ���ݿ��û���         
	db.setPassword("your password");   // ���ݿ�����
	db.setPort(3306);

	if (db.open()) {
		qDebug() << "connection success...";
	} else {
		qDebug() << "connection failed...";
		return;
	}

	std::cout << "===================���ڶ�ȡѡ����====================" << std::endl << std::endl;

	for (int i = 0; i < CHOICE_NUM; i++) {
		ChoiceQuestion* cq = new ChoiceQuestion(i + 1, "ѡ����");
		cq->readFromSQL();
		q[i] = cq;
	}

	std::cout << "===================���ڶ�ȡ�����====================" << std::endl << std::endl;

	for (int i = 0; i < FILL_NUM; i++) {
		FillQuestion* fq = new FillQuestion(i + 1, "�����");
		fq->readFromSQL();
		q[CHOICE_NUM + i] = fq;
	}

	std::cout << "===================���ڶ�ȡ�����====================" << std::endl << std::endl;

	for (int i = 0; i < SHORT_NUM; i++) {
		ShortQuestion* sq = new ShortQuestion(i + 1, "�����");
		sq->readFromSQL();
		q[CHOICE_NUM + FILL_NUM + i] = sq;
	}

	std::cout << "===================���ڶ�ȡ�����====================" << std::endl << std::endl;

	for (int i = 0; i < PROGRAM_NUM; i++) {
		ProgramQuestion* pq = new ProgramQuestion(i + 1, "�����");
		pq->readFromSQL();
		q[CHOICE_NUM + FILL_NUM + SHORT_NUM + i] = pq;
	}

	db.close();
}

inline
void Paper::calAllScores() const {

	std::vector<double> choice_score;  // ѡ����÷� 
	std::vector<double> fill_score;  // �����÷�
	std::vector<double> short_score;  // �����÷�
	std::vector<bool> program_ableToRun;  // ������Ƿ�����������

	std::cout << "====================���ڽ����з�====================" << std::endl << std::endl;

	std::cout << "==================����Ϊѡ�����з�===================" << std::endl << std::endl;

	for (int i = 0; i < CHOICE_NUM; i++) {
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(q[i]);
		choice_score.push_back(cq->calScore());
	}

	std::cout << "==================����Ϊ������з�===================" << std::endl << std::endl;

	for (int i = 0; i < FILL_NUM; i++) {
		FillQuestion* fq = dynamic_cast<FillQuestion*>(q[CHOICE_NUM + i]);
		fill_score.push_back(fq->calScore());
	}

	std::cout << "==================����Ϊ������з�===================" << std::endl << std::endl;

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

	std::cout << "==================���ڱ���ѧ���ɼ�===================" << std::endl << std::endl;

	std::ofstream os;
	os.open("scores.csv", std::ios::out | std::ios::trunc);

	os << std::string("ѧ��") << "," << "����" << ",";
	for (int i = 0; i < choice_score.size(); i++) {
		os << "ѡ����" + std::to_string(i + 1) << ",";
	}

	for (int i = 0; i < fill_score.size(); i++) {
		os << "�����" + std::to_string(i + 1) << ",";
	}

	for (int i = 0; i < short_score.size(); i++) {
		os << "�����" + std::to_string(i + 1) << ",";
	}

	for (int i = 0; i < program_ableToRun.size(); i++) {
		os << "�����" + std::to_string(i + 1) + "(�Ƿ�������)" << ",";
	}

	os << "ѡ����÷�" << "," << "�����÷�" << "," << "�����÷�" << "," << "�ܷ�" << std::endl;

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
		os << (program_ableToRun[i] == 1 ? "��" : "��") << ",";
	}

	sum = choice_sum + fill_sum + short_sum;

	os << choice_sum << "," << fill_sum << "," << short_sum << "," << sum << std::endl;

	os.close();

	std::cout << "==================����ѧ���ɼ��ɹ�===================" << std::endl << std::endl;
}

inline
std::vector<std::string> Paper::saveAsFiles() const {

	std::cout << "==================���ڱ���ѧ������===================" << std::endl << std::endl;

	std::vector<std::string> filenames(2 + SHORT_NUM + PROGRAM_NUM);

	std::string c_filename = std::string("choiceQuestion.txt");
	for (int i = 0; i < CHOICE_NUM; i++) {
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(q[i]);
		char c = cq->getStudentAns();
		std::ofstream os; // �����ļ����������
		os.open(c_filename, std::ios::app);
		os << i + 1 << ". " << c << "\n";  // ��ѧ�����������д��txt�ļ���
		os.close();
	}

	filenames[0] = c_filename;

	std::string f_filename = std::string("fillQuestion.txt");
	for (int i = 0; i < FILL_NUM; i++) {
		FillQuestion* fq = dynamic_cast<FillQuestion*>(q[CHOICE_NUM + i]);
		std::vector<std::string> vec = fq->getStudentAns();
		std::ofstream os; // �����ļ����������
		os.open(f_filename, std::ios::app);
		os << i + 1 << ". ";  // ��ѧ�����������д��txt�ļ���
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
		std::ofstream os; // �����ļ����������
		os.open(filename, std::ios::app);
		os << str;  // ��ѧ�����������д��txt�ļ���
		os.close();
		filenames[2 + i] = filename;
 	}

	for (int i = 0; i < PROGRAM_NUM; i++) {
		ProgramQuestion* pq = dynamic_cast<ProgramQuestion*>(q[CHOICE_NUM + FILL_NUM + SHORT_NUM + i]);
		std::string str = pq->getStudentAns();
		std::string filename = std::string("programQuestion_") + std::to_string(i + 1) + std::string(".cpp");
		std::ofstream os; // �����ļ����������
		os.open(filename, std::ios::app);
		os << str;  // ��ѧ�����������д��cpp�ļ���
		os.close();
		filenames[2 + SHORT_NUM + i] = filename;
	}

	filenames.push_back("scores.csv");
	filenames.push_back("program0_output.txt");
	filenames.push_back("program1_output.txt");
	filenames.push_back("program2_output.txt");

	std::cout << "==================����ѧ������ɹ�===================" << std::endl << std::endl;

	return filenames;
}

inline 
void Paper::uploadAllFiles(const std::vector<std::string>& filenames) const {

	std::cout << "==================�����ϴ�ѧ������===================" << std::endl << std::endl;

	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);

	const char* ip = "127.0.0.1";
	int port = 8080;
	int backlog = 5;

	/* ����һ��IPv4��ַ */
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);  // ��ipת���������ֽ���
	address.sin_port = htons(port);  // ���˿ں�ת���������ֽ���

	int cnt = 0;

	while (cnt < filenames.size()) {
		// ����socket
		int sock = socket(PF_INET, SOCK_STREAM, 0);

		if (sock < 0) {
			perror("[-]Error in socket");
			exit(1);
		}

		printf("[+]Server socket created successfully.\n");

		// ��socket�͵�ַ
		int ret = connect(sock, (struct sockaddr*)&address, sizeof(address));  // �ͻ��˲���Ҫ�� ֻ��Ҫ���� 

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

	std::cout << "==================�ϴ�ѧ������ɹ�===================" << std::endl << std::endl;
}

#endif // ! __PAPER__

