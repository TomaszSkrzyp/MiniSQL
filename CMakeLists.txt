cmake_minimum_required(VERSION 3.10.0)
project(MiniSQL VERSION 0.1.0 LANGUAGES C CXX)

add_executable(minisql_exe minisql/main.cpp minisql/storage.cpp minisql/parser.cpp minisql/tokenize.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)