cmake_minimum_required(VERSION 2.8)

project(lab1 CXX)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


file(GLOB lab1_SRC
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${lab1_SRC})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

