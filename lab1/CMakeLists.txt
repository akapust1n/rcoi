cmake_minimum_required(VERSION 2.8)
project(lab1 CXX)
 add_definitions("-std=c++17")

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

