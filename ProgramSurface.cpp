#include "ProgramSurface.h"
#include "ExaminationSystem.h"

using namespace Ui;

ProgramSurface::ProgramSurface(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::ProgramSurfaceClass()), isSaved(false)
{
	ui->setupUi(this);

	curr_id = 0;
	for (int i = 0; i < 3; i++) {
		isChanged[i] = false;
	}
	std::string str = "#include <iostream>\n\nint main() {\n  //your code\n}";
	for (int i = 0; i < 3; i++) { ans[i] = str; }

	connect(ui->codeEditor, &QPlainTextEdit::textChanged, this, [=] { isSaved = false; isChanged[curr_id] = true; });
	connect(ui->run, &QPushButton::clicked, this, &ProgramSurface::runCode);
	connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
	connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(updateError()));

	// ��һ��/��һ���Ӧ��ť���ź����
	connect(ui->pre, &QPushButton::clicked, this, &ProgramSurface::preQuestion);
	connect(ui->next, &QPushButton::clicked, this, &ProgramSurface::nextQuestion);
}

ProgramSurface::~ProgramSurface()
{
	delete ui;
}

void ProgramSurface::save() {
	QString savePath = "temp.cpp";
	QFile out(savePath);
	out.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream str(&out);
	str << ui->codeEditor->toPlainText();
	out.close();
	isSaved = true;
}

void ProgramSurface::runCode() {
	if (!isSaved) {
		save();
	}
	if (isSaved) {
		ui->console->clear();
		output.clear();
		error.clear();
		process.start("cmd");
		process.write("D:/MinGW/mingw64/bin/g++.exe -O2 temp.cpp -o output.exe\n");
		process.write("output.exe\n");
		process.waitForStarted();
		ui->console->setFocus();
		ui->statusBar->showMessage(tr("Run over!"));
	}
}

void ProgramSurface::updateOutput() {
	output = QString::fromLocal8Bit(process.readAllStandardOutput());
	ui->console->setPlainText(ui->console->toPlainText() + output);

	std::string filename = "program" + std::to_string(curr_id) + "_output.txt";
	std::ofstream os; // �����ļ����������
	os.open(filename, std::ios::app);
	os << std::string(output.toLocal8Bit());
	os.close();
}

void ProgramSurface::updateError() {
	error = QString::fromLocal8Bit(process.readAllStandardError());
	ui->console->setPlainText(ui->console->toPlainText() + error);
	process.terminate();

	// ������ʧ�ܣ��������ableToRun������Ϊfalse����ʾѧ���Ĵ��벻����������
	Question** qs = ExaminationSystem::paper->getQuestions();
	ProgramQuestion* pq = dynamic_cast<ProgramQuestion*>(qs[25 + curr_id]);
	pq->setRunInfo(false);

	std::string filename = "program" + std::to_string(curr_id) + "_error.txt";
	std::ofstream os; // �����ļ����������
	os.open(filename, std::ios::app);
	os << std::string(output.toLocal8Bit());
	os.close();
}

void ProgramSurface::showAnyQuestion(const int& id) {
	saveCurrAns(curr_id);

	ui->label->clear();

	// ����ʱ������û�����ȡ����ǰ��𰸣���䵽��������
	std::string str = ans[id];	
	ui->codeEditor->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

	// �л���ǰ�����Ŀ����䵽������
	Question** qs = ExaminationSystem::paper->getQuestions();
	ProgramQuestion* pq = dynamic_cast<ProgramQuestion*>(qs[25 + id]);
	ui->label->setText(QString(QString::fromLocal8Bit(pq->getDescription().c_str())));
	curr_id = id;
}

void ProgramSurface::saveCurrAns(const int& id) {
	if (ui->codeEditor->toPlainText() != QString("")) {
		ans[id] = std::string(ui->codeEditor->toPlainText().toLocal8Bit());
		Question** qs = ExaminationSystem::paper->getQuestions();
		ProgramQuestion* pq = dynamic_cast<ProgramQuestion*>(qs[25 + id]);
		pq->setStudentAns(ans[id]);
	}
}

void ProgramSurface::preQuestion() {
	saveCurrAns(curr_id);

	if (isChanged[curr_id]) { emit changeCurrentColor(curr_id); }

	if (curr_id != 0) {

		ui->label->clear();

		// ����ʱ������û�����ȡ����һ��𰸣���䵽��������
		std::string str = ans[curr_id - 1];
		ui->codeEditor->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

		// �л���һ�����Ŀ����䵽������
		Question** qs = ExaminationSystem::paper->getQuestions();
		ProgramQuestion* sq = dynamic_cast<ProgramQuestion*>(qs[25 + curr_id - 1]);
		ui->label->setText(QString(QString::fromLocal8Bit(sq->getDescription().c_str())));
		curr_id--;
	} else {
		this->close();
		emit changeToShort();
	}
}

void ProgramSurface::nextQuestion() {
	saveCurrAns(curr_id);

	if (isChanged[curr_id]) { emit changeCurrentColor(curr_id); }

	if (curr_id != 2) {

		// ����ʱ������û�����ȡ����һ��𰸣���䵽��������
		std::string str = ans[curr_id + 1];
		ui->codeEditor->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

		// �л���һ�����Ŀ����䵽������
		Question** qs = ExaminationSystem::paper->getQuestions();
		ProgramQuestion* sq = dynamic_cast<ProgramQuestion*>(qs[25 + curr_id + 1]);
		ui->label->setText(QString(QString::fromLocal8Bit(sq->getDescription().c_str())));
		curr_id++;
	}
}