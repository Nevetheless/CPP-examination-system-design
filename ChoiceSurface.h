#pragma once

#include <QMainWindow>
#include "ui_ChoiceSurface.h"
#include "FillSurface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChoiceSurfaceClass; };
QT_END_NAMESPACE

class ChoiceSurface : public QMainWindow
{
	Q_OBJECT

public:
	ChoiceSurface(QWidget *parent = nullptr);
	~ChoiceSurface();

	void showAnyQuestion(const int& id);
	int currentId() { return curr_id; }
	bool* currentStatus() { return isChanged; }

public slots:
	void preQuestion();  // "上一题"按钮对应的槽函数
	void nextQuestion();  // "下一题"按钮对应的槽函数

signals:
	void changeToFill();  // 切换至填空题界面
	void changeCurrentColor(const int& id);  // 完成当前题目，改变左侧对应题目按钮的颜色

private:
	bool isChanged[10];  // 记录每一题是否作答完成的flag
	int curr_id;  // 记录当前题目的题号
	char ans[10];  // 记录用户作答的答案
	Ui::ChoiceSurfaceClass *ui;
};
