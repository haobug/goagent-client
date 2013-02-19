cmake_minimum_required (VERSION 2.6) 
project (goagent-client)
find_package (Threads)
find_package(OpenSSL REQUIRED)

ENABLE_TESTING()

INCLUDE (CheckIncludeFiles)
INCLUDE (CheckTypeSize)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

#ADD_SUBDIRECTORY(gtest)
ADD_SUBDIRECTORY(src)
