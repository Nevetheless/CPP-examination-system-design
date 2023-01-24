#include "ExaminationSystem.h"
#include <QtWidgets/QApplication>

Paper* ExaminationSystem::paper = nullptr;

int main(int argc, char *argv[])
{
    system("chcp 65001");
    QApplication a(argc, argv);
    ExaminationSystem w;
    w.show();
    return a.exec();
}
