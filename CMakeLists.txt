cmake_minimum_required(VERSION 3.0)
project(asio_echo)

set(CMAKE_BUILD_TYPE "RelWithDebInfo")

include_directories(${PROJECT_SOURCE_DIR})

link_directories(${PROJECT_SOURCE_DIR}/libs)

set(CLIENT_SRC echo_client.cpp echo_client.h main_client.cpp)

set(SERVER_SRC connection.cpp connection.h main_server.cpp)

add_executable(client ${CLIENT_SRC})
add_executable(server ${SERVER_SRC})