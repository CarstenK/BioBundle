#include "translate.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    translate w;
    w.show();

    return a.exec();
}
