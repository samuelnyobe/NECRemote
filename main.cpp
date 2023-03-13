#include "remote.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Remote w;
    w.setWindowTitle("NEC Remote System");
    w.show();
    return a.exec();
}
