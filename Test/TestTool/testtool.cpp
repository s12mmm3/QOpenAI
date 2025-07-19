#include "testtool.h"

#include <QGlobalStatic>

const static QString token = "";
const static QString api_base_url = "";
Q_GLOBAL_STATIC_WITH_ARGS(QOpenAI, ai, (token, "QOpenAI", api_base_url));

QOpenAI *aiInstance()
{
    return ai();
}
