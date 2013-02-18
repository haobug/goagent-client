CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/event-config.h.in ${PROJECT_BINARY_DIR}/event2/event-config.h)

add_definitions(-DHAVE_CONFIG_H)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/compat)

if(WIN32)
add_definitions(-DUNICODE -D_UNICODE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/WIN32-Code)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif()

include_directories(OPENSSL_INCLUDE_DIR)

SET(CORE_SRCS event.c buffer.c bufferevent.c bufferevent_sock.c  	bufferevent_pair.c listener.c evmap.c log.c evutil.c  	strlcpy.c signal.c bufferevent_filter.c evthread.c  	bufferevent_ratelim.c evutil_rand.c)
if(WIN32)
SET(WIN_SRCS win32select.c evthread_win32.c buffer_iocp.c  	event_iocp.c bufferevent_async.c)
endif()
SET(EXTRA_SRCS event_tagging.c http.c evdns.c evrpc.c)
SET(SSL_SRCS bufferevent_openssl.c)

ADD_LIBRARY(event_core STATIC ${CORE_SRCS} ${WIN_SRCS})
#ADD_LIBRARY(event_extras STATIC ${EXTRA_SRCS})
#ADD_LIBRARY(event STATIC ${CORE_SRCS} ${EXTRA_SRCS} ${WIN_SRCS})
#ADD_LIBRARY(event_openssl STATIC ${SSL_SRCS})