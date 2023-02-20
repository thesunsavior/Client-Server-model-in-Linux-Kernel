cmake_minimum_required(VERSION 3.0.0)
project(Client_server VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Client_server src/main.cpp
                             src/admin.h
                             src/admin.cpp
                             src/client.h
                             src/client.cpp
                             src/util/logger.cpp
                             src/util/logger.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
