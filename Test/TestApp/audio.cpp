#include "testtool.h"
#include "qopenai.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString model = "FunAudioLLM/SenseVoiceSmall";
    QString file = "";
    {
        QVariantMap response = aiInstance()->audio.transcribe({
            { "model", model },
            { "file", file },
        });
        qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();
    }
    {
        QVariantMap response = aiInstance()->audio.translate({
            { "model", model },
            { "file", file },
        });
        qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();
    }

    return a.exec();
}
