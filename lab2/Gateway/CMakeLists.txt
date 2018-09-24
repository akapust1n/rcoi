cmake_minimum_required(VERSION 2.8)

project(Gateway CXX)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


file(GLOB lab2_SRC
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${lab2_SRC})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

