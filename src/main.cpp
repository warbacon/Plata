#include "plata.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Plata w;
    w.show();

    return a.exec();
}
