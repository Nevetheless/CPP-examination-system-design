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

	// 上一题/下一题对应按钮的信号与槽
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
	std::ofstream os; // 创建文件输出流对象
	os.open(filename, std::ios::app);
	os << std::string(output.toLocal8Bit());
	os.close();
}

void ProgramSurface::updateError() {
	error = QString::fromLocal8Bit(process.readAllStandardError());
	ui->console->setPlainText(ui->console->toPlainText() + error);
	process.terminate();

	// 若运行失败，将此题的ableToRun变量设为false，表示学生的代码不能正常运行
	Question** qs = ExaminationSystem::paper->getQuestions();
	ProgramQuestion* pq = dynamic_cast<ProgramQuestion*>(qs[25 + curr_id]);
	pq->setRunInfo(false);

	std::string filename = "program" + std::to_string(curr_id) + "_error.txt";
	std::ofstream os; // 创建文件输出流对象
	os.open(filename, std::ios::app);
	os << std::string(output.toLocal8Bit());
	os.close();
}

void ProgramSurface::showAnyQuestion(const int& id) {
	saveCurrAns(curr_id);

	ui->label->clear();

	// 从临时保存的用户答案中取出当前题答案，填充到答题区域
	std::string str = ans[id];	
	ui->codeEditor->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

	// 切换当前题的题目并填充到界面中
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

		// 从临时保存的用户答案中取出上一题答案，填充到答题区域
		std::string str = ans[curr_id - 1];
		ui->codeEditor->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

		// 切换上一题的题目并填充到界面中
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

		// 从临时保存的用户答案中取出上一题答案，填充到答题区域
		std::string str = ans[curr_id + 1];
		ui->codeEditor->setPlainText(QString(QString::fromLocal8Bit(str.c_str())));

		// 切换下一题的题目并填充到界面中
		Question** qs = ExaminationSystem::paper->getQuestions();
		ProgramQuestion* sq = dynamic_cast<ProgramQuestion*>(qs[25 + curr_id + 1]);
		ui->label->setText(QString(QString::fromLocal8Bit(sq->getDescription().c_str())));
		curr_id++;
	}
}