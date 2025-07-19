#include "testtool.h"
#include "qopenai.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString token = "";
    QString api_base_url = "";
    QOpenAI ai(token, "QOpenAI", api_base_url);

    QVariantMap request{
        { "model", "Qwen/Qwen3-8B" },

        // { "enable_thinking", true },
        { "thinking_budget", 8192 },
        { "min_p", 0.05 },
        { "temperature", 0.7 },
        { "top_p", 0.7 },
        { "top_k", 50 },
        { "frequency_penalty", 0.5 },
        { "n", 1 },
    };

    QVariantList messages;
    QVariantMap systemMsg;
    systemMsg["role"] = "system";
    systemMsg["content"] =
        R"(
Act as a helpful and friendly AI assistant. Provide clear, concise, and accurate responses.
Prioritize user needs by offering relevant information, practical solutions, and polite suggestions.
If unsure, ask clarifying questions. Maintain a positive tone and adapt to the user's communication style.
Focus on being reliable, empathetic, and professional in all interactions.
)";
    messages.append(systemMsg);

    qDebug().noquote() << "Please input:";
    while (1)
    {
        QTextStream in(stdin);
        QString msg;
        in >> msg;

        QVariantMap userMsg;
        userMsg["role"] = "user";
        userMsg["content"] = msg;
        messages.append(userMsg);

        request["messages"] = messages;

        qDebug().noquote() << "Please wait...";

        QVariantMap response = ai.chat.create(request);
        QVariantList choices = response["choices"].toList();
        for (const auto &choice : choices) {
            QVariantMap msg = choice.toMap()["message"].toMap();

            QVariantMap assistantMsg;
            assistantMsg["role"] = msg["role"].toString();
            assistantMsg["content"] = msg["content"].toString();
            messages.append(assistantMsg);

            qDebug().noquote() << msg["role"].toString() + ": " + msg["content"].toString();
        }
    }

    return a.exec();
}
