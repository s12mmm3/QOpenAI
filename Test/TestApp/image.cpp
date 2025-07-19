#include "testtool.h"
#include "qopenai.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVariantMap response = aiInstance()->image.create({
        { "model", "Kwai-Kolors/Kolors" },
        { "prompt", "an island near sea, with seagulls, moon shining over the sea, light house, boats int he background, fish flying over the sea" },
        { "n", 1 },
        { "size", "1024x1024" },
    });
    qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();

    return a.exec();
}
