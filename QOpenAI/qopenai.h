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

// QOpenAI
class QOPENAI_EXPORT QOpenAI {
public:
    QOpenAI(const QString& token = "", const QString& organization = "", const QString& api_base_url = "", const QString& beta = "");

    void setUrl(const QString& url);

    void setToken(const QString& token, const QString& organization);

    void setBeta(const QString& beta);

    QVariantMap makeRequest(const QString& suffix, const QString& data, const QString& contentType);

    QVariantMap post(const QString& suffix, const QString& data, const QString& contentType);

    QVariantMap post(const QString& suffix, const QVariantMap& json, const QString& contentType = "application/json");

public:
    CategoryCompletion      completion{*this};
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
