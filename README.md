# QOpenAI - A Qt-based OpenAI-like API Library / 基于Qt实现的类OpenAI API库

[![GitHub Actions CI Status](https://github.com/s12mmm3/QOpenAI/actions/workflows/windows.yml/badge.svg)](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/windows.yml)
[![GitHub Actions CI Status](https://github.com/s12mmm3/QOpenAI/actions/workflows/macos.yml/badge.svg)](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/macos.yml)
[![GitHub Actions CI Status](https://github.com/s12mmm3/QOpenAI/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/s12mmm3/QCloudMusicApi/actions/workflows/ubuntu.yml)

![C++ version](https://img.shields.io/badge/C++-11-00599C?logo=++)
[![Qt version](https://img.shields.io/badge/Qt-5.12+-41CD52?logo=qt)](https://www.qt.io)
[![Qt version](https://img.shields.io/badge/Qt-6.0+-41CD52?logo=qt)](https://www.qt.io)
![GitHub license](https://img.shields.io/github/license/s12mmm3/QOpenAI)

## 简介

This project is inspired by [olrea/openai-cpp](https://github.com/olrea/openai-cpp/tree/main), rewritten using Qt libraries. No need to configure curl - achieve cross-platform functionality with the powerful Qt framework.  
本项目参考了[olrea/openai-cpp](https://github.com/olrea/openai-cpp/tree/main)，基于Qt库重写，无需配置curl，即可基于强大的Qt库实现跨平台。

### 样例项目

*

### 目录

- [需求和依赖](#需求和依赖)
- [使用说明](#使用说明)
- [编译方式](#编译方式)
- [License](#License)

---

## Requirements and Dependencies / 需求和依赖

- [Qt 5.12+, Qt 6.0+](https://www.qt.io/download-qt-installer)

## Usage Instructions / 使用说明

Simple chat program: You need to configure token and base_url before calling. You can set the OPENAI_API_KEY environment variable before use (or set it directly in the code).  
简单的聊天程序：调用前需要配置token和base_url，可在使用之前设置OPENAI_API_KEY环境变量（也可以直接在代码中设置）

```C++
#include "qopenai.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString token = ""; // your token
    QString api_base_url = ""; // the base url like https://api.openai.com/v1/
    QOpenAI ai(token, "QOpenAI", api_base_url);
    QVariantMap request{
        { "model", "Qwen/Qwen3-8B" }, // the model name
        { "temperature", 0.7 },
    };
    QVariantList messages;
    QVariantMap systemMsg;
    systemMsg["role"] = "system";
    systemMsg["content"] = R"(Say this is a test)";
    messages.append(systemMsg);

    QVariantMap userMsg;
    userMsg["role"] = "user";
    userMsg["content"] = "hello";
    messages.append(userMsg);
    request["messages"] = messages;

    QVariantMap response = ai.chat.create(request);
    qDebug().noquote() << response;

    return a.exec();
}

```

### Embedding in CMake Projects / 在CMake项目中嵌入本项目

Add to your CMakeLists.txt file:
```CMake
add_subdirectory(QOpenAI)
include_directories(QOpenAI)
add_executable(${PROJECT_NAME}
    main.cpp
)
target_link_libraries(${PROJECT_NAME} QOpenAI)
```
Include the header file:
```C++
#include "qopenai.h"
```

## Compilation Methods / 编译方式

```Shell
git clone --recursive https://github.com/s12mmm3/QOpenAI.git
cd QCloudMusicApi
cmake -B build
cmake --build build -j
```

## License

[The MIT License (MIT)](LICENSE)