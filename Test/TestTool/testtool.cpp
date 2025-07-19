#include "testtool.h"

#include <QFile>

const static QString token = "";
const static QString api_base_url = "";
Q_GLOBAL_STATIC(QOpenAI, ai, token, "QOpenAI", api_base_url);

QOpenAI *aiInstance()
{
    return ai();
}
