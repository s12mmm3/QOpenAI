#include "testtool.h"
#include "qopenai.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    {
        QVariantMap response = aiInstance()->model.list();
        qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();
    }

    return a.exec();
}
