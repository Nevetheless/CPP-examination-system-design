#pragma once

#include <QtWidgets/QMainWindow>
#include "Paper.h"
#include "ui_ExaminationSystem.h"
#include "MainSurface.h"

#include <qaction.h>
#include <qpushbutton.h>
#include <qdebug.h>

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QMessageBox>

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class ExaminationSystemClass; };
QT_END_NAMESPACE

class ExaminationSystem : public QMainWindow
{
    Q_OBJECT

public:
    ExaminationSystem(QWidget *parent = nullptr);
    ~ExaminationSystem();

    static Paper* paper;

public slots:
    //void openMainUI();

private:
    Ui::ExaminationSystemClass *ui;
    MainSurface* main_ui;
};
