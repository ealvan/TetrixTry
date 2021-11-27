#include "myqframe.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyQFrame w;
    w.show();
    return a.exec();
}
