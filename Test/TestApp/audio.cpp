#include "testtool.h"
#include "qopenai.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    {
        QVariantMap response = aiInstance()->audio.transcribe({
            { "model", "FunAudioLLM/SenseVoiceSmall" },
            { "file", "audio.mp3" },
        });
        qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();
    }
    {
        QVariantMap response = aiInstance()->audio.translate({
            { "model", "FunAudioLLM/SenseVoiceSmall" },
            { "file", "audio.mp3" },
        });
        qDebug().noquote() << QJsonDocument::fromVariant(response).toJson();
    }

    return a.exec();
}
