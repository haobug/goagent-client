cmake_minimum_required (VERSION 2.6) 
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${LIBEVENT_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

add_definitions(-DEVHTP_DISABLE_REGEX -D_CRT_SECURE_NO_WARNINGS -DHAVE_CONFIG_H)
add_executable(goagent-client goagent-client.c ini.c parse_config.c goagent-client.h ini.h http_parser.h http_parser.cpp)
TARGET_LINK_LIBRARIES(goagent-client  ${LIBEVENT_CORE_LIBRARY} ${LIBEVENT_OPENSSL_LIBRARY} ${OPENSSL_LIBRARIES})


