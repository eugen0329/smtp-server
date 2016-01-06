cmake_minimum_required(VERSION 2.8.3)
project(tcp_client)

include_directories(../shared)

set(TARGET_NAME "client")

file(GLOB_RECURSE SRC_NAMES src/*.c)
file(GLOB_RECURSE SHARED_SRC_NAMES ../shared/*.c)

add_executable(${TARGET_NAME} ${SRC_NAMES} ${SHARED_SRC_NAMES} )
target_link_libraries(${TARGET_NAME} dl)
