#include <QApplication>
#include "widget.h"
#include "configuration.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();

    Configuration s;
    s.show();
    return a.exec();
}
