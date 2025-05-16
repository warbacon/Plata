#include "plata.h"

#include <QApplication>
#include <QToolBar>
#include <QLabel>
#include <QToolButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Plata w;
    w.show();

    return a.exec();
}
