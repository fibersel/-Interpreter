cmake_minimum_required(VERSION 3.10)
project(LexAnalyser)

set(CMAKE_CXX_STANDARD 11)

add_executable(LexAnalyser main.cpp cmake-build-debug/LexGetter.h cmake-build-debug/Parser.h cmake-build-debug/Executer.h cmake-build-debug/Stack.h)