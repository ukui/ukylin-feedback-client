#include "feedback.h"

#include <QApplication>
#include "QDebug"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    feedback w;
    w.show();
    qDebug()<<w.frameGeometry().width()<<"aaaaa";
    return a.exec();
}
