cmake_minimum_required(VERSION 3.16)

project(QOpenAIProject LANGUAGES CXX)

add_subdirectory(QOpenAI)
include_directories(QOpenAI)
add_subdirectory(Test)
