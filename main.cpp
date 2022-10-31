#include "widget.h"
#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
    MainWidget mw;
    mw.show();
    return a.exec();
}
