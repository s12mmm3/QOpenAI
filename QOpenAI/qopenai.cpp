#include "qopenai.h"

QOpenAI::QOpenAI(const QString &token, const QString &organization, const QString &api_base_url, const QString &beta)
    : token_{token}, organization_{organization}
{
    manager = new QNetworkAccessManager;
    if (token.isEmpty()) {
        if (const char* env_p = qgetenv("OPENAI_API_KEY")) {
            token_ = env_p;
        }
    }
    if (api_base_url.isEmpty()) {
        if(const char* env_p = qgetenv("OPENAI_API_BASE")) {
            base_url = QString(env_p) + "/";
        }
        else {
            base_url = "https://api.openai.com/v1/";
        }
    }
    else {
        base_url = api_base_url;
    }
    setUrl(base_url);
    setToken(token_, organization_);
    setBeta(beta);
}

void QOpenAI::setUrl(const QString &url)
{
    url_ = url;
}

void QOpenAI::setToken(const QString &token, const QString &organization)
{
    token_ = token;
    organization_ = organization;
}

void QOpenAI::setBeta(const QString &beta)
{
    beta_ = beta;
}

QVariantMap QOpenAI::makeRequest(const QString &suffix, const QString &data, const QString &contentType)
{
    auto complete_url = base_url + suffix;

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
    QNetworkReply *reply = manager->post(request, data.toUtf8());
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray response = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response);
    reply->deleteLater();
    return json.toVariant().toMap();
}

QVariantMap QOpenAI::post(const QString &suffix, const QString &data, const QString &contentType)
{
    return makeRequest(suffix, data, contentType);
}

QVariantMap QOpenAI::post(const QString &suffix, const QVariantMap &json, const QString &contentType)
{
    return post(suffix, QJsonDocument::fromVariant(json).toJson(), contentType);
}

QVariantMap CategoryCompletion::create(QVariantMap input)
{
    return openai_.post("completions", input);
}

QVariantMap CategoryChat::create(QVariantMap input)
{
    return openai_.post("chat/completions", input);
}
