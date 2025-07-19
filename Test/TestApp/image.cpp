#include "testtool.h"
#include "qopenai.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString model = "Kwai-Kolors/Kolors";
    int n = 1;
    QString size = "1024x1024";

    {
        QVariantMap response = aiInstance()->image.create({
            { "model", model },
            { "prompt", "an island near sea" },
            { "n", n },
            { "size", size },
        });
        qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();
    }

    {
        QVariantMap response = aiInstance()->image.edit({
            { "model", model },
            { "image", "pic.png" },
            { "prompt", "an island near sea, with seagulls, moon shining over the sea, light house, boats int he background, fish flying over the sea" },
            { "n", n },
            { "size", size },
        });
        qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();
    }

    {
        QVariantMap response = aiInstance()->image.variation({
            { "model", model },
            { "image", "pic.png" },
            { "n", n },
            { "size", size },
        });
        qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();
    }

    return a.exec();
}
