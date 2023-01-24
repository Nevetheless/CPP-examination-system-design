#pragma once

#include <QMainWindow>
#include "ui_StackWidget.h"
#include "ChoiceSurface.h"
#include "FillSurface.h"
#include "ShortSurface.h"
#include "ProgramSurface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StackWidgetClass; };
QT_END_NAMESPACE

class StackWidget : public QMainWindow
{
	Q_OBJECT

public:
	StackWidget(QWidget *parent = nullptr);
	~StackWidget();

	void initialization();

public slots:
	void changeColor(int id);

private:
	enum stackWidgetID { ui1 = 0, ui2, ui3, ui4 };
	ChoiceSurface* choice_ui;
	FillSurface* fill_ui;
	ShortSurface* short_ui;
	ProgramSurface* program_ui;
	Ui::StackWidgetClass *ui;
};
