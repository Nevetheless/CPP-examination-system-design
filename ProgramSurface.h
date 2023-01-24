#pragma once

#include <QMainWindow>
#include <qtextstream.h>
#include <qprocess.h>
#include <qstring.h>
#include <qdebug.h>

#include "ui_ProgramSurface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProgramSurfaceClass; };
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class ExaminationSystemClass; };
QT_END_NAMESPACE

//#ifdef WIN32  
//#pragma execution_character_set("utf-8")  
//#endif

class ProgramSurface : public QMainWindow
{
	Q_OBJECT

public:
	ProgramSurface(QWidget *parent = nullptr);
	~ProgramSurface();

	void save();
	void saveCurrAns(const int& id);
	void showAnyQuestion(const int& id);

public slots:
	void runCode();
	void updateOutput();
	void updateError();

	void preQuestion();
	void nextQuestion();

	int currentId() { return curr_id; }
	bool* currentStatus() { return isChanged; }

signals:
	void changeToShort();
	void changeCurrentColor(const int& id);

private:
	Ui::ProgramSurfaceClass *ui;
	QProcess process;
	bool isSaved;

	QString output;
	QString error;

	bool isChanged[3];
	int curr_id;
	std::string ans[3];
};