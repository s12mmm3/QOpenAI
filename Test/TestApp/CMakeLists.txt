cmake_minimum_required(VERSION 3.5)

project(TestApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Concurrent)

function(create_test test_name)
    FILE(GLOB HEADERS ./${test_name}.h)
    FILE(GLOB SOURCES ./${test_name}.cpp)
    FILE(GLOB UIS ./${test_name}.ui)
    FILE(GLOB QRCS ./${test_name}.qrc)
    set(PROJECT_SOURCES
        ${HEADERS}
        ${SOURCES}
        ${UIS}
        ${QRCS}
    )

    add_executable(${test_name}
        ${PROJECT_SOURCES}
    )

    target_link_libraries(${test_name} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Concurrent
        TestTool
        QOpenAI
    )

    target_compile_definitions(${test_name} PRIVATE
        PROJECT_NAME="${test_name}"
    )
endfunction()

set(test_name_list
    chat
    image
)

foreach(test_name IN LISTS test_name_list)
    create_test(${test_name})
endforeach()
