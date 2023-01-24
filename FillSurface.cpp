#include "FillSurface.h"
#include "ExaminationSystem.h"

FillSurface::FillSurface(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::FillSurfaceClass())
{
	ui->setupUi(this);

	curr_id = 0;
	for (int i = 0; i < 10; i++) {
		isChanged[i] = false;
	}

	// 上一题/下一题对应按钮的信号与槽
	connect(ui->pre, &QPushButton::clicked, this, &FillSurface::preQuestion);
	connect(ui->next, &QPushButton::clicked, this, &FillSurface::nextQuestion);
	connect(ui->blank1, &QLineEdit::textChanged, [=]() { isChanged[curr_id] = true; });
	connect(ui->blank2, &QLineEdit::textChanged, [=]() { isChanged[curr_id] = true; });
	connect(ui->blank3, &QLineEdit::textChanged, [=]() { isChanged[curr_id] = true; });
}

FillSurface::~FillSurface()
{
	delete ui;
}

void FillSurface::saveCurrAns() {
	Question** qs = ExaminationSystem::paper->getQuestions();
	FillQuestion* fq = dynamic_cast<FillQuestion*>(qs[10 + curr_id]);

	int num = fq->getNumOfBlanks();

	QList<QLineEdit*> blkList = ui->blankWidget->findChildren<QLineEdit*>();
	ans[curr_id].resize(num);
	for (int i = 0; i < num; i++) {
		ans[curr_id][i] = std::string(blkList[i]->text().toLocal8Bit());
	}
	fq->setStudentAns(ans[curr_id]);
}

void FillSurface::showAnyQuestion(const int& id) {
	saveCurrAns();

	ui->blank1->clear();
	ui->blank2->clear();
	ui->blank3->clear();

	// 获取所有blank
	QList<QLineEdit*> blkList = ui->blankWidget->findChildren<QLineEdit*>();
	for (int i = 0; i < blkList.size(); i++) {
		blkList[i]->setMinimumSize(QSize(110, 40));
		blkList[i]->setMaximumSize(QSize(110, 40));
	}

	Question** qs = ExaminationSystem::paper->getQuestions();
	FillQuestion* fq = dynamic_cast<FillQuestion*>(qs[10 + id]);
	
	int num = fq->getNumOfBlanks();
 
	for (int i = blkList.size() - 1; i > num - 1; i--) {
		blkList[i]->setMinimumSize(QSize(0, 0));
		blkList[i]->setMaximumSize(QSize(0, 0));
	}


	// 从临时保存的用户答案中取出当前题答案，填充到答题区域
	std::vector<std::string> vec = ans[id];
	for (int i = 0; i < vec.size(); i++) {
		blkList[i]->setText(QString(QString::fromLocal8Bit(vec[i].c_str())));
	}

	// 切换当前题的题目并填充到界面中
	ui->label->setText(QString(QString::fromLocal8Bit(fq->getDescription().c_str())));
	curr_id = id;
}


void FillSurface::preQuestion() {
	saveCurrAns();

	if (isChanged) { emit changeCurrentColor(curr_id); }

	if (curr_id != 0) {

		ui->blank1->clear();
		ui->blank2->clear();
		ui->blank3->clear();

		// 获取所有blank
		QList<QLineEdit*> blkList = ui->blankWidget->findChildren<QLineEdit*>();
		for (int i = 0; i < blkList.size(); i++) {
			blkList[i]->setMinimumSize(QSize(110, 40));
			blkList[i]->setMaximumSize(QSize(110, 40));
		}

		Question** qs = ExaminationSystem::paper->getQuestions();
		FillQuestion* fq = dynamic_cast<FillQuestion*>(qs[10 + curr_id - 1]);

		int num = fq->getNumOfBlanks();

		for (int i = blkList.size() - 1; i > num - 1; i--) {
			blkList[i]->setMinimumSize(QSize(0, 0));
			blkList[i]->setMaximumSize(QSize(0, 0));
		}

		// 从临时保存的用户答案中取出当前题答案，填充到答题区域
		std::vector<std::string> vec = ans[curr_id - 1];
		for (int i = 0; i < vec.size(); i++) {
			blkList[i]->setText(QString(QString::fromLocal8Bit(vec[i].c_str())));
		}

		// 切换上一题的题目并填充到界面中
		ui->label->setText(QString(QString::fromLocal8Bit(fq->getDescription().c_str())));
		curr_id--;
	} else {
		this->close();
		emit changeToChoice();
	}
}

void FillSurface::nextQuestion() {
	saveCurrAns();

	if (isChanged) { emit changeCurrentColor(curr_id); }

	if (curr_id != 9) {

		ui->blank1->clear();
		ui->blank2->clear();
		ui->blank3->clear();

		// 获取所有blank
		QList<QLineEdit*> blkList = ui->blankWidget->findChildren<QLineEdit*>();
		for (int i = 0; i < blkList.size(); i++) {
			blkList[i]->setMinimumSize(QSize(110, 40));
			blkList[i]->setMaximumSize(QSize(110, 40));
		}

		Question** qs = ExaminationSystem::paper->getQuestions();
		FillQuestion* fq = dynamic_cast<FillQuestion*>(qs[10 + curr_id + 1]);

		int num = fq->getNumOfBlanks();

		for (int i = blkList.size() - 1; i > num - 1; i--) {
			blkList[i]->setMinimumSize(QSize(0, 0));
			blkList[i]->setMaximumSize(QSize(0, 0));
		}

		// 从临时保存的用户答案中取出当前题答案，填充到答题区域
		std::vector<std::string> vec = ans[curr_id + 1];
		for (int i = 0; i < vec.size(); i++) {
			blkList[i]->setText(QString(QString::fromLocal8Bit(vec[i].c_str())));
		}

		// 切换下一题的题目并填充到界面中
		ui->label->setText(QString(QString::fromLocal8Bit(fq->getDescription().c_str())));
		curr_id++;
	} else {
		this->close();
		emit changeToShort();
	}
}
