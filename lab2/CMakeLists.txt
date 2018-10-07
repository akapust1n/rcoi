cmake_minimum_required(VERSION 3.8)

project(lab2 CXX)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

SET( CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Debug ... FORCE )
ENDIF()


file(GLOB shared_SRC
    "./Shared/*.h"
    "./Shared/*.cpp"
)

file(GLOB 3rdpart_SRC
    "./3rdpart/*.h"
    "./3rdpart/*.cpp"
)

file(GLOB gateway_SRC
    "./Gateway/*.h"
    "./Gateway/*.cpp"
)


add_executable(gateway ${gateway_SRC}  ${shared_SRC} ${3rdpart_SRC})
target_link_libraries(gateway ${CONAN_LIBS})

file(GLOB news_SRC
    "./News/*.h"
    "./News/*.cpp"
)

add_executable(news ${news_SRC} ${shared_SRC} ${3rdpart_SRC})
target_link_libraries(news ${CONAN_LIBS} mysqlclient )

file(GLOB users_SRC
    "./Users/*.h"
    "./Users/*.cpp"
)

add_executable(users ${users_SRC} ${shared_SRC} ${3rdpart_SRC})
target_link_libraries(users ${CONAN_LIBS} mysqlclient)

file(GLOB comments_SRC
    "./Comments/*.h"
    "./Comments/*.cpp"
)

add_executable(comments ${comments_SRC} ${shared_SRC} ${3rdpart_SRC})
target_link_libraries(comments ${CONAN_LIBS}  mysqlclient)

