#include "ShortSurface.h"
#include "ExaminationSystem.h"

using namespace Ui;

ShortSurface::ShortSurface(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::ShortSurfaceClass())
{
	ui->setupUi(this);

	curr_id = 0;
	for (int i = 0; i < 5; i++) {
		isChanged[i] = false;
	}

	// ��һ��/��һ���Ӧ��ť���ź����
	connect(ui->pre, &QPushButton::clicked, this, &ShortSurface::preQuestion);
	connect(ui->next, &QPushButton::clicked, this, &ShortSurface::nextQuestion);
	connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, [=]() { isChanged[curr_id] = true; });
}

ShortSurface::~ShortSurface()
{
	delete ui;
}

void ShortSurface::preQuestion() {

	saveCurrAns(curr_id);

	if (isChanged) { emit changeCurrentColor(curr_id); }

	if (curr_id != 0) {

		ui->plainTextEdit->clear();

		// ����ʱ������û�����ȡ����һ��𰸣���䵽��������
		std::string str = ans[curr_id - 1];
		ui->plainTextEdit->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

		// �л���һ�����Ŀ����䵽������
		Question** qs = ExaminationSystem::paper->getQuestions();
		ShortQuestion* sq = dynamic_cast<ShortQuestion*>(qs[20 + curr_id - 1]);
		ui->label->setText(QString(QString::fromLocal8Bit(sq->getDescription().c_str())));
		curr_id--;
	} else {
		this->close();
		emit changeToFill();
	}
}

void ShortSurface::nextQuestion() {

	saveCurrAns(curr_id);

	if (isChanged) { emit changeCurrentColor(curr_id); }

	if (curr_id != 4) {

		ui->plainTextEdit->clear();

		// ����ʱ������û�����ȡ����һ��𰸣���䵽��������
		std::string str = ans[curr_id + 1];
		ui->plainTextEdit->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

		// �л���һ�����Ŀ����䵽������
		Question** qs = ExaminationSystem::paper->getQuestions();
		ShortQuestion* sq = dynamic_cast<ShortQuestion*>(qs[20 + curr_id + 1]);
		ui->label->setText(QString(QString::fromLocal8Bit(sq->getDescription().c_str())));
		curr_id++;
	}
	else {
		this->close();
		emit changeToProgram();
	} 
}

void ShortSurface::saveCurrAns(const int& id) {
	if (ui->plainTextEdit->toPlainText() != QString("")) {
		ans[id] = std::string(ui->plainTextEdit->toPlainText().toLocal8Bit());
		Question** qs = ExaminationSystem::paper->getQuestions();
		ShortQuestion* sq = dynamic_cast<ShortQuestion*>(qs[20 + id]);
		sq->setStudentAns(ans[id]);
	}
}

void ShortSurface::showAnyQuestion(const int& id) {

	saveCurrAns(curr_id);

	ui->plainTextEdit->clear();

	// ����ʱ������û�����ȡ����ǰ��𰸣���䵽��������
	std::string str = ans[id];
	ui->plainTextEdit->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

	// �л���ǰ�����Ŀ����䵽������
	Question** qs = ExaminationSystem::paper->getQuestions();
	ShortQuestion* sq = dynamic_cast<ShortQuestion*>(qs[20 + id]);
	ui->label->setText(QString(QString::fromLocal8Bit(sq->getDescription().c_str())));
	curr_id = id;
}