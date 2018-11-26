cmake_minimum_required(VERSION 3.1)

project(lab2 CXX)

include_directories(3rdpart/cpp-jwt/include)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

SET( CMAKE_BUILD_TYPE "Debug")



set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")


if(IS_TEST_BUILD)
    message("TEST BUILD")
    add_definitions(-DIS_TEST_BUILD)
endif()

if(DOCKER)
    message("DOCKER BUILD")
    add_definitions(-DDOCKER)
endif()

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

file(GLOB tests_SRC
    "./Tests/*.h"
    "./Tests/*.cpp"
)

add_executable(tests ${tests_SRC} ${gateway_SRC} ${shared_SRC} ${3rdpart_SRC})
target_link_libraries(tests ${CONAN_LIBS} mysqlclient)


file(GLOB likehistory_SRC
    "./LikeHistory/*.h"
    "./LikeHistory/*.cpp"
)

add_executable(likehistory ${likehistory_SRC} ${shared_SRC} ${3rdpart_SRC})
target_link_libraries(likehistory ${CONAN_LIBS}  mysqlclient)

