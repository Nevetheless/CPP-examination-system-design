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
        // ��ȡѧ�����������
        QString name = ui->name->text();
        // ��ȡѧ�������ѧ��
        QString id = ui->id->text();

        // ͨ��ODBC������MySQL����
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");    

        db.setHostName("localhost");         
        db.setDatabaseName("examinationdb");  // �������ݿ� --> examinationdb 
        db.setUserName("root");  // ���ݿ��û���         
        db.setPassword("your password");   // ���ݿ�����
        db.setPort(3306);                   
        if (db.open()) {
            if (name == "") {
                QMessageBox::warning(this, "��ʾ", "��������Ϊ��!");
                return;
            }

            if (id == "") {
                QMessageBox::warning(this, "��ʾ", "ѧ�Ų���Ϊ��!");
                return;
            }

            QSqlQuery query("select * from studentinfo where ѧ�� = '" + id + "'");  // ����ѧ�Ų�ѯѧ�������� ���Ƿ�����������ƥ��

            while (query.next()) {
                QString exact_name = query.value(1).toString();  // studentinfo���е�2���ֶ�Ϊ����ѧ��ѧ��������
                
                if (name == exact_name) {
                    QMessageBox::information(this, "��ʾ", "��¼�ɹ�!");
                    this->close();
                    main_ui->show();
                } else {
                    QMessageBox::warning(this, "��ʾ", "������ѧ�Ų�ƥ�䣬����������!");
                }
            }

        } else {
            QMessageBox::warning(this, "��ʾ", "����ѧ����Ϣ���ݿ�ʧ��!", QMessageBox::Yes);
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
