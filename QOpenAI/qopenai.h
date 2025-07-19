#ifndef QOPENAI_H
#define QOPENAI_H

#include "QOpenAI_global.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QEventLoop>
#include <QUrlQuery>
#include <QUrl>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QNetworkProxy>

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

    void setUrl(const QString& url);

    void setToken(const QString& token, const QString& organization);

    void setBeta(const QString& beta);

    QVariantMap makeRequest(const QString& suffix, const QString& data, const QString& contentType = "");

    QVariantMap post(const QString& suffix, const QString& data, const QString& contentType);

    QVariantMap get(const QString& suffix, const QString& data = "");

    QVariantMap post(const QString& suffix, const QVariantMap& json, const QString& contentType = "application/json");

public:
    CategoryModel           model     {*this};
    CategoryCompletion      completion{*this};
    CategoryImage           image     {*this};
    CategoryChat            chat      {*this};

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
