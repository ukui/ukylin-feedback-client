#include "feedback.h"
#include "database.h"

#include <QApplication>
#include "QDebug"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createConnection()) return 1;

    feedback w;

    w.show();
    return a.exec();
}
