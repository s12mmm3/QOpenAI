#include "testtool.h"
#include "qopenai.h"

#include <QApplication>
#include <QLoggingCategory>

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
