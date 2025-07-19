#include "qopenai.h"

#include <QFileInfo>

QHttpPart createFileHttpPart(const QString& name, const QString& filePath) {
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"" + name + "\"; filename=\"" + QFileInfo(filePath).fileName() + "\""));
    filePart.setBody(file.readAll());
    return filePart;
}

QHttpPart createTextHttpPart(const QString& name, const QString& value) {
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"" + name + "\""));
    textPart.setBody(value.toUtf8());
    return textPart;
}

QOpenAI::QOpenAI(const QString &token, const QString &organization, const QString &api_base_url, const QString &beta)
    : token_{token}, organization_{organization}
{
    manager = new QNetworkAccessManager;
    if (token.isEmpty())
    {
        if (const char* env_p = qgetenv("OPENAI_API_KEY"))
        {
            token_ = env_p;
        }
    }
    if (api_base_url.isEmpty()) {
        if (const char* env_p = qgetenv("OPENAI_API_BASE")) {
            base_url = QString(env_p) + "/";
        }
        else
        {
            base_url = "https://api.openai.com/v1/";
        }
    }
    else
    {
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
    QHttpMultiPart multipart(QHttpMultiPart::FormDataType);

    QString imagePath = input["image"].toString();
    multipart.append(createFileHttpPart("image", imagePath));

    if (input.contains("mask") && !input["mask"].toString().isEmpty()) {
        multipart.append(createFileHttpPart("mask", input["mask"].toString()));
    }

    const QStringList params = {"prompt", "n", "size", "response_format", "user"};
    for (const QString &key : params) {
        if (input.contains(key)) {
            multipart.append(createTextHttpPart(key, input[key].toString()));
        }
    }

    return openai_.makeRequest("images/edits", &multipart);
}

QVariantMap CategoryImage::variation(QVariantMap input)
{
    QHttpMultiPart multipart(QHttpMultiPart::FormDataType);

    QString imagePath = input["image"].toString();
    multipart.append(createFileHttpPart("image", imagePath));

    const QStringList params = {"n", "size", "response_format", "user"};
    for (const QString &key : params) {
        if (input.contains(key)) {
            multipart.append(createTextHttpPart(key, input[key].toString()));
        }
    }

    return openai_.makeRequest("images/variations", &multipart);
}

QVariantMap CategoryAudio::transcribe(QVariantMap input) {
    QHttpMultiPart multipart(QHttpMultiPart::FormDataType);

    multipart.append(createFileHttpPart("file", input["file"].toString()));

    const QStringList params = { "model", "language", "prompt", "response_format", "temperature" };
    for (const QString &key : params) {
        if (input.contains(key)) {
            multipart.append(createTextHttpPart(key, input[key].toString()));
        }
    }

    return openai_.makeRequest("audio/transcriptions", &multipart, "");
}

QVariantMap CategoryAudio::translate(QVariantMap input) {
    QHttpMultiPart multipart(QHttpMultiPart::FormDataType);

    multipart.append(createFileHttpPart("file", input["file"].toString()));

    const QStringList params = { "model", "prompt", "response_format", "temperature" };
    for (const QString &key : params) {
        if (input.contains(key)) {
            multipart.append(createTextHttpPart(key, input[key].toString()));
        }
    }

    return openai_.makeRequest("audio/translations", &multipart, "");
}
