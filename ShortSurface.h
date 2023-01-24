#pragma once

#include <QMainWindow>
#include "ui_ShortSurface.h"
#include "ProgramSurface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ShortSurfaceClass; };
QT_END_NAMESPACE

class ShortSurface : public QMainWindow
{
	Q_OBJECT

public:
	ShortSurface(QWidget *parent = nullptr);
	~ShortSurface();

	void saveCurrAns(const int& id);
	void showAnyQuestion(const int& id);
	int currentId() { return curr_id; }
	bool* currentStatus() { return isChanged; }

public slots:
	void preQuestion();
	void nextQuestion();

signals:
	void changeToFill();
	void changeToProgram();
	void changeCurrentColor(const int& id);

private:
	bool isChanged[5];
	int curr_id;
	std::string ans[5];
	Ui::ShortSurfaceClass *ui;
};
