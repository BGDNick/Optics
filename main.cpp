#include "window.hpp"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    try
    {
        QApplication a(argc, argv);
        Window w;
        w.show();
        return a.exec();
    }
    catch(...)
    {
        qDebug() << QString("Failed to launch");
    }

}
