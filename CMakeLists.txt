cmake_minimum_required (VERSION 2.6) 
project (goagent-client)

include(${PROJECT_SOURCE_DIR}/cmake/FindLibEvent.cmake)


#if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#    SET (CMAKE_C_COMPILER             "/usr/bin/clang")
#    SET (CMAKE_C_FLAGS                "-Wall -std=c99")
#    SET (CMAKE_C_FLAGS_DEBUG          "-g")
#    SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#    SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
#    SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
#
#    SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
#    SET (CMAKE_CXX_FLAGS                "-Wall")
#    SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
#    SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#    SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
#    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
#
#    SET (CMAKE_AR      "/usr/bin/llvm-ar")
#    SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
#    SET (CMAKE_NM      "/usr/bin/llvm-nm")
#    SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
#    SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
#endif()

find_package (Threads)
find_package(OpenSSL REQUIRED)

ENABLE_TESTING()

INCLUDE (CheckIncludeFiles)
INCLUDE (CheckTypeSize)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

#ADD_SUBDIRECTORY(gtest)
ADD_SUBDIRECTORY(src)
