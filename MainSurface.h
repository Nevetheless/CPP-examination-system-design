#pragma once

#include <QMainWindow>
#include "ui_MainSurface.h"
#include "StackWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainSurfaceClass; };
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class ExaminationSystemClass; };
QT_END_NAMESPACE

class MainSurface : public QMainWindow
{
	Q_OBJECT

public:
	MainSurface(QWidget *parent = nullptr);
	~MainSurface();


public slots:
	void openStackWidget();

private:
	StackWidget* sw_ui;
	Ui::MainSurfaceClass *ui;
};
