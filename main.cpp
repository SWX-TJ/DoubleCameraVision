#include "visionmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VisionMainWindow w;
    w.show();

    return a.exec();
}
