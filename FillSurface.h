#pragma once

#include <QMainWindow>
#include "ui_FillSurface.h"
#include "ShortSurface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FillSurfaceClass; };
QT_END_NAMESPACE

class FillSurface : public QMainWindow
{
	Q_OBJECT

public:
	FillSurface(QWidget *parent = nullptr);
	~FillSurface();

	void saveCurrAns();
	void showAnyQuestion(const int& id);
	int currentId() { return curr_id; }
	bool* currentStatus() { return isChanged; }

public slots:
	void preQuestion();
	void nextQuestion();

signals:
	void changeToChoice();
	void changeToShort();
	void changeCurrentColor(const int& id);

private:
	bool isChanged[10];
	int curr_id;
	std::vector<std::string> ans[10];
	Ui::FillSurfaceClass *ui;
};
