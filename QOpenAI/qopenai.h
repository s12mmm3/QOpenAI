#ifndef QOPENAI_H
#define QOPENAI_H

#include "QOpenAI_global.h"

#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QUrl>
#include <QUrlQuery>

class QOpenAI;

// https://platform.openai.com/docs/api-reference/models
// List and describe the various models available in the API. You can refer to the Models documentation to understand what models are available and the differences between them.
struct QOPENAI_EXPORT CategoryModel {
    QVariantMap list();
    QVariantMap retrieve(const QString& model);

    CategoryModel(QOpenAI& openai) : openai_{openai} {}
private:
    QOpenAI& openai_;
};

// https://platform.openai.com/docs/api-reference/completions
// Given a prompt, the model will return one or more predicted completions, and can also return the probabilities of alternative tokens at each position.
struct QOPENAI_EXPORT CategoryCompletion {
    QVariantMap create(QVariantMap input);

    CategoryCompletion(QOpenAI& openai) : openai_{openai} {}

private:
    QOpenAI& openai_;
};

// https://platform.openai.com/docs/api-reference/chat
// Given a prompt, the model will return one or more predicted chat completions.
struct QOPENAI_EXPORT CategoryChat {
    QVariantMap create(QVariantMap input);

    CategoryChat(QOpenAI& openai) : openai_{openai} {}

private:
    QOpenAI& openai_;
};

// https://platform.openai.com/docs/api-reference/audio
// Learn how to turn audio into text.
struct QOPENAI_EXPORT CategoryAudio {
    QVariantMap transcribe(QVariantMap input);
    QVariantMap translate(QVariantMap input);

    CategoryAudio(QOpenAI& openai) : openai_{openai} {}

private:
    QOpenAI& openai_;
};


// https://platform.openai.com/docs/api-reference/images
// Given a prompt and/or an input image, the model will generate a new image.
struct QOPENAI_EXPORT CategoryImage {
    QVariantMap create(QVariantMap input);
    QVariantMap edit(QVariantMap input);
    QVariantMap variation(QVariantMap input);

    CategoryImage(QOpenAI& openai) : openai_{openai} {}

private:
    QOpenAI& openai_;
};

// QOpenAI
class QOPENAI_EXPORT QOpenAI {
public:
    QOpenAI(const QString& token = "", const QString& organization = "", const QString& api_base_url = "", const QString& beta = "");

    void setToken(const QString& token, const QString& organization);

    void setProxy(const QString& proxy);

    void setBeta(const QString& beta);

    template<typename T>
    QVariantMap makeRequest(const QString& suffix,
                            const T& data,
                            const QString& contentType = "",
                            QNetworkAccessManager::Operation method = QNetworkAccessManager::PostOperation
                            )
    {
        auto complete_url = base_url + "/" + suffix;
        QNetworkRequest request(complete_url);
        if (!contentType.isEmpty())
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
        }
        request.setRawHeader("Authorization", "Bearer " + token_.toUtf8());
        if (!organization_.isEmpty())
        {
            request.setRawHeader("OpenAI-Organization", organization_.toUtf8());
        }
        if (!beta_.isEmpty())
        {
            request.setRawHeader("OpenAI-Beta", beta_.toUtf8());
        }
        QNetworkReply* reply;
        if (method == QNetworkAccessManager::PostOperation) {
            reply = manager->post(request, data);
        }
        else {
            reply = manager->get(request);
        }
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QByteArray response = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response);
        reply->deleteLater();
        return json.toVariant().toMap();
    }

    QVariantMap post(const QString& suffix, const QString& data, const QString& contentType);

    QVariantMap get(const QString& suffix, const QString& data = "");

    QVariantMap post(const QString& suffix, const QVariantMap& json, const QString& contentType = "application/json");

    void setBaseUrl(const QString &url);

    QString getBaseUrl() const;

public:
    CategoryModel           model     {*this};
    CategoryCompletion      completion{*this};
    CategoryImage           image     {*this};
    CategoryChat            chat      {*this};
    CategoryAudio           audio     {*this};

private:
    QNetworkAccessManager *manager;

    QString base_url;
    QString url_;
    QString proxy_url_;
    QString token_;
    QString organization_;
    QString beta_;
};

#endif // QOPENAI_H
