cmake_minimum_required(VERSION 2.8)

project(lab1v2)
INCLUDE_DIRECTORIES(/usr/local/include/Wt)
file(GLOB lab1v2_SRC
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${lab1v2_SRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} wt wthttp)

