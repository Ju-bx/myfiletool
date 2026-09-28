cmake_minimum_required(VERSION 3.16)
project(myfiletool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myfiletool src/main.cpp)
target_compile_options(myfiletool PRIVATE -Wall -Wextra)