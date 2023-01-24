#include "ChoiceSurface.h"
#include "ExaminationSystem.h"
#include "ChoiceQuestion.h"

#include "Paper.h"
#include <qdebug.h>

using namespace Ui;

ChoiceSurface::ChoiceSurface(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::ChoiceSurfaceClass())
{
	ui->setupUi(this);


	curr_id = 0;
	for (int i = 0; i < 10; i++) {
		isChanged[i] = false;
	}

	// 上一题/下一题对应按钮的信号与槽
	connect(ui->pre, &QPushButton::clicked, this, &ChoiceSurface::preQuestion);
	connect(ui->next, &QPushButton::clicked, this, &ChoiceSurface::nextQuestion);

	// 各个选项的信号与槽
	connect(ui->A, &QRadioButton::clicked, [=]() { 
		ans[curr_id] = 'A';
		Question** qs = ExaminationSystem::paper->getQuestions();
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(qs[curr_id]);
		cq->setStudentAns('A');
		isChanged[curr_id] = true;
	});
	connect(ui->B, &QRadioButton::clicked, [=]() { 
		ans[curr_id] = 'B'; 
		Question** qs = ExaminationSystem::paper->getQuestions();
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(qs[curr_id]);
		cq->setStudentAns('B');
		isChanged[curr_id] = true;
	});
	connect(ui->C, &QRadioButton::clicked, [=]() { 
		ans[curr_id] = 'C'; 
		Question** qs = ExaminationSystem::paper->getQuestions();
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(qs[curr_id]);
		cq->setStudentAns('C');
		isChanged[curr_id] = true;
	});
	connect(ui->D, &QRadioButton::clicked, [=]() { 
		ans[curr_id] = 'D'; 
		Question** qs = ExaminationSystem::paper->getQuestions();
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(qs[curr_id]);
		cq->setStudentAns('D');
		isChanged[curr_id] = true;
	});
}

ChoiceSurface::~ChoiceSurface()
{
	delete ui;
}

void ChoiceSurface::preQuestion() {

	if (isChanged) {
		emit changeCurrentColor(curr_id);
	}

	if (curr_id != 0) {

		ui->A->setAutoExclusive(false);
		ui->A->setChecked(false);
		ui->A->setAutoExclusive(true);

		ui->B->setAutoExclusive(false);
		ui->B->setChecked(false);
		ui->B->setAutoExclusive(true);

		ui->C->setAutoExclusive(false);
		ui->C->setChecked(false);
		ui->C->setAutoExclusive(true);

		ui->D->setAutoExclusive(false);
		ui->D->setChecked(false);
		ui->D->setAutoExclusive(true);

		// 从临时保存的用户答案中取出上一题答案，将对应的选项check上
		char c = ans[curr_id - 1];
		switch (c) {
		case 'A':
			ui->A->setChecked(1);
			break;
		case 'B':
			ui->B->setChecked(1);
			break;
		case 'C':
			ui->C->setChecked(1);
			break;
		case 'D':
			ui->D->setChecked(1);
			break;
		default:
			break;
		}

		// 切换上一题的题目并填充到界面中
		Question** qs = ExaminationSystem::paper->getQuestions();
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(qs[curr_id - 1]);
		ui->label->setText(QString(QString::fromLocal8Bit(cq->getDescription().c_str())));
		ui->A->setText(QString(QString::fromLocal8Bit(cq->getA().c_str())));
		ui->B->setText(QString(QString::fromLocal8Bit(cq->getB().c_str())));
		ui->C->setText(QString(QString::fromLocal8Bit(cq->getC().c_str())));
		ui->D->setText(QString(QString::fromLocal8Bit(cq->getD().c_str())));
		curr_id--;
	}
}

void ChoiceSurface::nextQuestion() {

	if (isChanged) {
		emit changeCurrentColor(curr_id);
	}

	if (curr_id != 9) {

		ui->A->setAutoExclusive(false);
		ui->A->setChecked(false);
		ui->A->setAutoExclusive(true);

		ui->B->setAutoExclusive(false);
		ui->B->setChecked(false);
		ui->B->setAutoExclusive(true);

		ui->C->setAutoExclusive(false);
		ui->C->setChecked(false);
		ui->C->setAutoExclusive(true);

		ui->D->setAutoExclusive(false);
		ui->D->setChecked(false);
		ui->D->setAutoExclusive(true);

		// 从临时保存的用户答案中取出下一题答案，将对应的选项check上
		char c = ans[curr_id + 1];
		switch (c) {
		case 'A':
			ui->A->setChecked(1);
			break;
		case 'B':
			ui->B->setChecked(1);
			break;
		case 'C':
			ui->C->setChecked(1);
			break;
		case 'D':
			ui->D->setChecked(1);
			break;
		default:
			break;
		}

		// 切换下一题的题目并填充到界面中
		Question** qs = ExaminationSystem::paper->getQuestions();
		ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(qs[curr_id + 1]);
		ui->label->setText(QString(QString::fromLocal8Bit(cq->getDescription().c_str())));
		ui->A->setText(QString(QString::fromLocal8Bit(cq->getA().c_str())));
		ui->B->setText(QString(QString::fromLocal8Bit(cq->getB().c_str())));
		ui->C->setText(QString(QString::fromLocal8Bit(cq->getC().c_str())));
		ui->D->setText(QString(QString::fromLocal8Bit(cq->getD().c_str())));
		curr_id++;

	} else {
		this->close();
		emit changeToFill();
	}
}

void ChoiceSurface::showAnyQuestion(const int& id) {

	ui->A->setAutoExclusive(false);
	ui->A->setChecked(false);
	ui->A->setAutoExclusive(true);

	ui->B->setAutoExclusive(false);
	ui->B->setChecked(false);
	ui->B->setAutoExclusive(true);

	ui->C->setAutoExclusive(false);
	ui->C->setChecked(false);
	ui->C->setAutoExclusive(true);

	ui->D->setAutoExclusive(false);
	ui->D->setChecked(false);
	ui->D->setAutoExclusive(true);

	// 从临时保存的用户答案中取出当前题的答案，将对应的选项check上
	char c = ans[id];
	switch (c) {
		case 'A':
			ui->A->setChecked(1);
			break;
		case 'B':
			ui->B->setChecked(1);
			break;
		case 'C':
			ui->C->setChecked(1);
			break;
		case 'D':
			ui->D->setChecked(1);
			break;
		default:
			break;
	}

	// 切换当前题的题目并填充到界面中
	Question** qs = ExaminationSystem::paper->getQuestions();
	ChoiceQuestion* cq = dynamic_cast<ChoiceQuestion*>(qs[id]);
	ui->label->setText(QString(QString::fromLocal8Bit(cq->getDescription().c_str())));
	ui->A->setText(QString(QString::fromLocal8Bit(cq->getA().c_str())));
	ui->B->setText(QString(QString::fromLocal8Bit(cq->getB().c_str())));
	ui->C->setText(QString(QString::fromLocal8Bit(cq->getC().c_str())));
	ui->D->setText(QString(QString::fromLocal8Bit(cq->getD().c_str())));

	curr_id = id;
}