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
	void preQuestion();  // "��һ��"��ť��Ӧ�Ĳۺ���
	void nextQuestion();  // "��һ��"��ť��Ӧ�Ĳۺ���

signals:
	void changeToFill();  // �л�����������
	void changeCurrentColor(const int& id);  // ��ɵ�ǰ��Ŀ���ı�����Ӧ��Ŀ��ť����ɫ

private:
	bool isChanged[10];  // ��¼ÿһ���Ƿ�������ɵ�flag
	int curr_id;  // ��¼��ǰ��Ŀ�����
	char ans[10];  // ��¼�û�����Ĵ�
	Ui::ChoiceSurfaceClass *ui;
};
