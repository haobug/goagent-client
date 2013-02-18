cmake_minimum_required (VERSION 2.6) 
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)


add_executable(goagent-client goagent-client.c ini.c parse_config.c goagent-client.h ini.h)

