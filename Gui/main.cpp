#include <QtWidgets/QApplication>

#include "Property.h"
#include "MainWindow.h"
#include "Common.h"

int main(int argc, char *argv[])
{
    lang = new Property;
    lang->read("languages.lang");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
