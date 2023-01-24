#include "StackWidget.h"
#include "ExaminationSystem.h"

using namespace Ui;

StackWidget::StackWidget(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::StackWidgetClass())
{
	ui->setupUi(this);

	choice_ui = new ChoiceSurface();
	fill_ui = new FillSurface();
	short_ui = new ShortSurface();
	program_ui = new ProgramSurface();

	ui->stackedWidget->insertWidget(ui1, choice_ui);
	ui->stackedWidget->insertWidget(ui2, fill_ui);
	ui->stackedWidget->insertWidget(ui3, short_ui);
	ui->stackedWidget->insertWidget(ui4, program_ui);

	connect(choice_ui, &ChoiceSurface::changeCurrentColor, this, &StackWidget::changeColor);
	connect(fill_ui, &FillSurface::changeCurrentColor, this, &StackWidget::changeColor);
	connect(short_ui, &ShortSurface::changeCurrentColor, this, &StackWidget::changeColor);
	connect(program_ui, &ProgramSurface::changeCurrentColor, this, &StackWidget::changeColor);

	// 选择题界面 --> 填空题界面 
	connect(choice_ui, &ChoiceSurface::changeToFill, [=]() {
		fill_ui->showAnyQuestion(0);
		ui->stackedWidget->setCurrentIndex(ui2);
		setWindowTitle(QString("填空题作答"));
	});

	// 填空题界面 --> 简答题界面 
	connect(fill_ui, &FillSurface::changeToShort, [=]() {
		short_ui->showAnyQuestion(0);
		ui->stackedWidget->setCurrentIndex(ui3);
		setWindowTitle(QString("简答题作答"));
	});

	// 填空题界面 --> 选择题界面
	connect(fill_ui, &FillSurface::changeToChoice, [=]() {
		choice_ui->showAnyQuestion(9);
		ui->stackedWidget->setCurrentIndex(ui1);
		setWindowTitle(QString("选择题作答"));
	});

	// 简答题界面 --> 编程题界面
	connect(short_ui, &ShortSurface::changeToProgram, [=]() {
		ui->stackedWidget->setMinimumSize(QSize(712, 690));
		ui->stackedWidget->setMaximumSize(QSize(712, 690));
		this->setMinimumSize(QSize(990, 700));
		this->setMaximumSize(QSize(990, 700));
		program_ui->showAnyQuestion(0);
		ui->stackedWidget->setCurrentIndex(ui4);
		setWindowTitle(QString("编程题作答"));
	});

	// 简答题界面 --> 填空题界面
	connect(short_ui, &ShortSurface::changeToFill, [=]() {
		fill_ui->showAnyQuestion(9);
		ui->stackedWidget->setCurrentIndex(ui2);
		setWindowTitle(QString("填空题作答"));
	});

	// 编程题界面 --> 简答题界面
	connect(program_ui, &ProgramSurface::changeToShort, [=]() {
		ui->stackedWidget->setMinimumSize(QSize(712, 500));
		ui->stackedWidget->setMaximumSize(QSize(712, 500));
		this->setMinimumSize(QSize(990, 560));
		this->setMaximumSize(QSize(990, 560));
		short_ui->showAnyQuestion(4);
		ui->stackedWidget->setCurrentIndex(ui3);
		setWindowTitle(QString("简答题作答"));
	});

	// 侧边栏选择题按钮信号与槽
	QList<QPushButton*> c_btnList = ui->choiceWidget->findChildren<QPushButton*>();
	for (int i = 0; i < c_btnList.size(); i++) {
		connect(c_btnList[i], &QPushButton::clicked, [=]() {
			changeColor(-1);
			if (ui->stackedWidget->currentIndex() == ui4) {
				ui->stackedWidget->setMinimumSize(QSize(712, 500));
				ui->stackedWidget->setMaximumSize(QSize(712, 500));
				this->setMinimumSize(QSize(990, 560));
				this->setMaximumSize(QSize(990, 560));
			}
			choice_ui->showAnyQuestion(i);
			ui->stackedWidget->setCurrentIndex(ui1);
			setWindowTitle(QString("选择题作答"));
		});
	}

	// 侧边栏填空题按钮信号与槽
	QList<QPushButton*> f_btnList = ui->fillWidget->findChildren<QPushButton*>();
	for (int i = 0; i < f_btnList.size(); i++) {
		connect(f_btnList[i], &QPushButton::clicked, [=]() {
			changeColor(-1);
			if (ui->stackedWidget->currentIndex() == ui4) {
				ui->stackedWidget->setMinimumSize(QSize(712, 500));
				ui->stackedWidget->setMaximumSize(QSize(712, 500));
				this->setMinimumSize(QSize(990, 560));
				this->setMaximumSize(QSize(990, 560));
			}
			fill_ui->showAnyQuestion(i);
			ui->stackedWidget->setCurrentIndex(ui2);
			setWindowTitle(QString("填空题作答"));
		});
	}

	// 侧边栏简答题按钮信号与槽
	QList<QPushButton*> s_btnList = ui->shortWidget->findChildren<QPushButton*>();
	for (int i = 0; i < s_btnList.size(); i++) {
		connect(s_btnList[i], &QPushButton::clicked, [=]() {
			changeColor(-1);
			if (ui->stackedWidget->currentIndex() == ui4) {
				ui->stackedWidget->setMinimumSize(QSize(712, 500));
				ui->stackedWidget->setMaximumSize(QSize(712, 500));
				this->setMinimumSize(QSize(990, 560));
				this->setMaximumSize(QSize(990, 560));
			}
			short_ui->showAnyQuestion(i);
			ui->stackedWidget->setCurrentIndex(ui3);
			setWindowTitle(QString("简答题作答"));
		});
	}

	// 侧边栏编程题按钮信号与槽
	QList<QPushButton*> b_btnList = ui->programWidget->findChildren<QPushButton*>();
	for (int i = 0; i < b_btnList.size(); i++) {
		connect(b_btnList[i], &QPushButton::clicked, [=]() {
			changeColor(-1);
			ui->stackedWidget->setMinimumSize(QSize(712, 690));
			ui->stackedWidget->setMaximumSize(QSize(712, 690));
			this->setMinimumSize(QSize(990, 700));
			this->setMaximumSize(QSize(990, 700));
			program_ui->showAnyQuestion(i);
			ui->stackedWidget->setCurrentIndex(ui4);
			setWindowTitle(QString("编程题作答"));
		});
	}

	connect(ui->pushButton, &QPushButton::clicked, [=]() {
		ExaminationSystem::paper->calAllScores();
		std::vector<std::string> filenames = ExaminationSystem::paper->saveAsFiles();
		ExaminationSystem::paper->uploadAllFiles(filenames);
	});

}

StackWidget::~StackWidget()
{
	delete ui;
}

void StackWidget::initialization() {
	ui->stackedWidget->setCurrentIndex(ui1);
	choice_ui->showAnyQuestion(0);
	setWindowTitle(QString("选择题作答"));
}

void StackWidget::changeColor(int id) {
	int idx = ui->stackedWidget->currentIndex();

	switch (idx) {
	case ui1:
		if (id == -1) {
			id = choice_ui->currentId();
		}
		if (choice_ui->currentStatus()[id]) {
			QList<QPushButton*> c_btnList = ui->choiceWidget->findChildren<QPushButton*>();
			c_btnList[choice_ui->currentId()]->setStyleSheet("background-color: rgb(91, 231, 196);");
		}
		break;
	case ui2:
		if (id == -1) {
			id = fill_ui->currentId();
		}
		if (fill_ui->currentStatus()[id]) {
			QList<QPushButton*> f_btnList = ui->fillWidget->findChildren<QPushButton*>();
			f_btnList[fill_ui->currentId()]->setStyleSheet("background-color: rgb(91, 231, 196);");
		}
		break;
	case ui3:
		if (id == -1) {
			id = short_ui->currentId();
		}
		if (short_ui->currentStatus()[id]) {
			QList<QPushButton*> s_btnList = ui->shortWidget->findChildren<QPushButton*>();
			s_btnList[short_ui->currentId()]->setStyleSheet("background-color: rgb(91, 231, 196);");
		}
		break;
	default:
		if (id == -1) {
			id = program_ui->currentId();
		}
		if (program_ui->currentStatus()[id]) {
			QList<QPushButton*> b_btnList = ui->programWidget->findChildren<QPushButton*>();
			b_btnList[program_ui->currentId()]->setStyleSheet("background-color: rgb(91, 231, 196);");
		}
		break;
	}
}