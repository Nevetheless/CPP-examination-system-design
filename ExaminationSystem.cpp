#include "ExaminationSystem.h"

using namespace Ui;

ExaminationSystem::ExaminationSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExaminationSystemClass())
{
    ui->setupUi(this);

    main_ui = new MainSurface();

    QAction* action1 = new QAction(ui->name);
    action1->setIcon(QIcon(":/resources/user.png"));
    ui->name->addAction(action1, QLineEdit::LeadingPosition);

    QAction* action2 = new QAction(ui->id);
    action2->setIcon(QIcon(":/resources/id.png"));
    ui->id->addAction(action2, QLineEdit::LeadingPosition);

    /*ui->xiaozi->setPixmap(QPixmap(":/resources/xiaohui-w.png").scaled(211, 61));*/

    connect(ui->pushButton, &QPushButton::clicked, [=]() {
        // 获取学生输入的姓名
        QString name = ui->name->text();
        // 获取学生输入的学号
        QString id = ui->id->text();

        // 通过ODBC来创建MySQL连接
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");    

        db.setHostName("localhost");         
        db.setDatabaseName("examinationdb");  // 连接数据库 --> examinationdb 
        db.setUserName("root");  // 数据库用户名         
        db.setPassword("your password");   // 数据库密码
        db.setPort(3306);                   
        if (db.open()) {
            if (name == "") {
                QMessageBox::warning(this, "提示", "姓名不能为空!");
                return;
            }

            if (id == "") {
                QMessageBox::warning(this, "提示", "学号不能为空!");
                return;
            }

            QSqlQuery query("select * from studentinfo where 学号 = '" + id + "'");  // 根据学号查询学生的姓名 看是否与输入姓名匹配

            while (query.next()) {
                QString exact_name = query.value(1).toString();  // studentinfo表中第2个字段为输入学号学生的姓名
                
                if (name == exact_name) {
                    QMessageBox::information(this, "提示", "登录成功!");
                    this->close();
                    main_ui->show();
                } else {
                    QMessageBox::warning(this, "提示", "姓名与学号不匹配，请重新输入!");
                }
            }

        } else {
            QMessageBox::warning(this, "提示", "连接学生信息数据库失败!", QMessageBox::Yes);
        }
        db.close();

        this->close();
        main_ui->show();

        ExaminationSystem::paper = new Paper(std::string(id.toLocal8Bit()), std::string(name.toLocal8Bit()));
        ExaminationSystem::paper->initialization();
    });
}

ExaminationSystem::~ExaminationSystem()
{
    delete ui;
}
