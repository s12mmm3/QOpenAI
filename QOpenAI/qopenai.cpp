#include "qopenai.h"

#include <QFileInfo>

QHttpPart createFileHttpPart(const QString& filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(filePath).fileName() + "\""));
    filePart.setBody(file.readAll());
    return filePart;
}

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

QVariantMap QOpenAI::post(const QString &suffix, const QString &data, const QString &contentType)
{
    return makeRequest(suffix, data.toUtf8(), contentType);
}

QVariantMap QOpenAI::get(const QString &suffix, const QString &data)
{
    return makeRequest(suffix, data.toUtf8());
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

QVariantMap CategoryModel::list()
{
    return openai_.get("models");
}

QVariantMap CategoryModel::retrieve(const QString &model)
{
    return openai_.get("models/" + model);
}

QVariantMap CategoryImage::create(QVariantMap input)
{
    return openai_.post("images/generations", input);
}

QVariantMap CategoryImage::edit(QVariantMap input)
{
    return {};
}

QVariantMap CategoryImage::variation(QVariantMap input)
{
    return {};
}

QVariantMap CategoryAudio::transcribe(QVariantMap input) {
    QHttpMultiPart multipart(QHttpMultiPart::FormDataType);

    multipart.append(createFileHttpPart(input["file"].toString()));

    const QStringList params = { "model", "language", "prompt", "response_format", "temperature" };
    for (const QString &key : params) {
        if (!input.contains(key)) continue;
        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"" + key + "\""));
        textPart.setBody(input[key].toString().toUtf8());
        multipart.append(textPart);
    }

    return openai_.makeRequest("audio/transcriptions", &multipart, "");
}

QVariantMap CategoryAudio::translate(QVariantMap input) {
    QHttpMultiPart multipart(QHttpMultiPart::FormDataType);

    multipart.append(createFileHttpPart(input["file"].toString()));

    const QStringList params = { "model", "prompt", "response_format", "temperature" };
    for (const QString &key : params) {
        if (!input.contains(key)) continue;
        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"" + key + "\""));
        textPart.setBody(input[key].toString().toUtf8());
        multipart.append(textPart);
    }

    return openai_.makeRequest("audio/translations", &multipart, "");
}
