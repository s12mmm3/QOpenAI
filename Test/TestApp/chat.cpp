#include "testtool.h"
#include "qopenai.h"
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct RegisteredFunction {
    QString name;
    QString description;
    QVariantMap parameters;
    std::function<QVariant(QVariantMap)> function = Q_NULLPTR;
};

QMap<QString, RegisteredFunction> registeredFunctions; // Stores registered functions
void registerFunction(const QString& name,
                      const std::function<QVariant(QVariantMap)> func,
                      const QString& description,
                      const QVariantMap& parameters) {
    RegisteredFunction regFunc;
    regFunc.name = name;
    regFunc.description = description;
    regFunc.parameters = parameters;
    regFunc.function = func;
    registeredFunctions[name] = regFunc;
};

int main(int argc, char *argv[])
{
    registerFunction(
        "get_weather",
        [](QVariantMap data) {
            auto location = data["location"].toString();
            return QString("The weather in %1 is 25°C, sunny").arg(location);
        },
        "Get weather information for a specified area",
        QVariantMap{
            { "type", "object" },
            { "properties", QVariantMap{
                               { "location", QVariantMap{
                                                { "type", "string" },
                                                { "description", "City name, such as 'Beijing' or 'New York'" }
                                            }}
                           }},
            { "required", QVariantList{ "location" } }
        }
        );
    registerFunction(
        "get_qt_version",
        [](QVariantMap data) {
            return qVersion();
        },
        "Get the Qt version of the current software",
        QVariantMap{
            { "type", "object" },
            { "properties", QVariantMap{}},
            { "required", QVariantList{ } }
        }
        );

    QCoreApplication a(argc, argv);

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

    // Build tools list (function definitions)
    QVariantList tools;
    for (const auto& func : registeredFunctions) {
        QVariantMap funcInfo;
        funcInfo["name"] = func.name;
        funcInfo["description"] = func.description;
        funcInfo["parameters"] = func.parameters;

        QVariantMap tool{
            { "type", "function" },
            { "function", funcInfo },
        };
        tools.append(tool);
    }
    if (!tools.isEmpty()) request["tools"] = tools;

    qDebug().noquote() << "Please input:";
    while (true)
    {
        QTextStream in(stdin);
        QString msg;
        in >> msg;
        if (msg.isEmpty()) continue;

        QVariantMap userMsg;
        userMsg["role"] = "user";
        userMsg["content"] = msg;
        messages.append(userMsg);

        bool hasFunctionCall = true;
        int maxIterations = 3; // Prevent infinite loops
        int iterationCount = 0;

        while (hasFunctionCall && iterationCount < maxIterations)
        {
            hasFunctionCall = false;
            iterationCount++;

            request["messages"] = messages;
            qDebug().noquote() << "Sending request (iteration" << iterationCount << ")...";

            QVariantMap response = aiInstance()->chat.create(request);

            QVariantList choices = response["choices"].toList();
            for (auto& choice : choices) {
                QVariantMap choiceMap = choice.toMap();
                QVariantMap msg = choiceMap["message"].toMap();

                // Check for tool calls
                if (!msg.contains("tool_calls")) {
                    qDebug().noquote() << "\nAssistant:" << msg["content"].toString();
                    messages.append(msg); // Save final response
                    continue;
                }
                hasFunctionCall = true;
                messages.append(msg); // Save tool call request

                QVariantList toolCalls = msg["tool_calls"].toList();
                for (const auto& toolCall : toolCalls) {
                    QVariantMap toolCallMap = toolCall.toMap();
                    QVariantMap func = toolCall.toMap()["function"].toMap();
                    QString name = func["name"].toString();
                    QString arguments = func["arguments"].toString();

                    qDebug().noquote() << "Function call detected:" << name
                                       << "with args:" << arguments;

                    if (!registeredFunctions.contains(name))
                    {
                        qWarning() << "Unregistered function:" << name;
                        continue;
                    }

                    // Execute function
                    QVariant functionResult = registeredFunctions[name].function(
                        QJsonDocument::fromJson(arguments.toUtf8()).toVariant().toMap()
                        );

                    // Add tool call result message
                    QVariantMap toolMsg;
                    toolMsg["role"] = "tool";
                    toolMsg["tool_call_id"] = toolCallMap["id"].toString();
                    toolMsg["name"] = name;
                    toolMsg["content"] = functionResult;

                    messages.append(toolMsg);
                    qDebug() << "Function result:" << toolMsg["content"];
                }
            }
        }

        if (iterationCount >= maxIterations) {
            qDebug() << "Reached maximum function call iterations";
        }
    }

    return a.exec();
}
